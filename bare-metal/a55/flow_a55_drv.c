/*
 * flow_a55_drv.c
 *
 * Linux driver for the A55 side of the shared memory mailbox.
 *
 * The driver is a transport only: it maps the reserved SHMEM region, validates
 * the mailbox ABI and exposes the already protected FSoE PDUs through a char
 * device.  The EtherCAT/FSoE fieldbus stack stays in user space; the kernel
 * module contains no safety decision.
 *
 * The PDU is protected end-to-end by FSoE (CRC, sequence, watchdog).  The
 * driver only checks the mailbox metadata and never modifies the PDU.
 */

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#include "flow_a55.h"
#include "flow_a55_uapi.h"
#include "flow_shm.h"

#ifndef FLOW_A55_DEBUG
#define FLOW_A55_DEBUG 0
#endif

struct flow_a55_device
{
   void __iomem * base;
   flow_shm_t * shm;
   flow_a55_t a55;
   struct miscdevice misc;
   struct mutex lock;
   bool open;
};

static int flow_a55_open (struct inode * inode, struct file * file)
{
   struct miscdevice * misc = (struct miscdevice *)file->private_data;
   struct flow_a55_device * device =
      container_of (misc, struct flow_a55_device, misc);

   (void)inode;

   mutex_lock (&device->lock);
   if (device->open)
   {
      mutex_unlock (&device->lock);
      return -EBUSY;
   }
   device->open = true;
   file->private_data = device;
   mutex_unlock (&device->lock);

   return 0;
}

static int flow_a55_release (struct inode * inode, struct file * file)
{
   struct flow_a55_device * device =
      (struct flow_a55_device *)file->private_data;

   (void)inode;

   mutex_lock (&device->lock);
   device->open = false;
   mutex_unlock (&device->lock);

   return 0;
}

static long flow_a55_ioctl (
   struct file * file,
   unsigned int command,
   unsigned long argument)
{
   struct flow_a55_device * device =
      (struct flow_a55_device *)file->private_data;
   long result = 0;

   mutex_lock (&device->lock);

   switch (command)
   {
   case FLOW_A55_IOCTL_GET_ABI:
   {
      struct flow_a55_abi abi;

      memset (&abi, 0, sizeof (abi));
      abi.abi_version = FLOW_A55_ABI_VERSION;
      abi.protocol_id = device->shm->header.protocol_id;
      abi.layout_hash = device->shm->header.layout_hash;
      abi.conn_count = device->shm->header.conn_count;
      abi.slot_size = device->shm->header.slot_size;
      abi.pdu_max = FLOW_PDU_MAX;

      if (copy_to_user ((void __user *)argument, &abi, sizeof (abi)) != 0u)
      {
         result = -EFAULT;
      }
      break;
   }

   case FLOW_A55_IOCTL_GET_TX:
   {
      struct flow_a55_frame_u out;
      flow_a55_frame_t frame;
      flow_a55_result_t status;

      memset (&out, 0, sizeof (out));
      status = flow_a55_get_tx (&device->a55, 0u, &frame);

      if (status == FLOW_A55_NO_DATA)
      {
         result = -EAGAIN;
         break;
      }
      if (status != FLOW_A55_OK)
      {
         result = -EBADMSG;
         break;
      }

      out.conn = frame.conn;
      out.seq = frame.seq;
      out.fc = frame.fc;
      out.len = frame.len;
      memcpy (out.pdu, frame.pdu, frame.len);

      if (copy_to_user ((void __user *)argument, &out, sizeof (out)) != 0u)
      {
         result = -EFAULT;
      }
      break;
   }

   case FLOW_A55_IOCTL_PUT_RX:
   {
      struct flow_a55_frame_u in;

      if (copy_from_user (&in, (const void __user *)argument, sizeof (in)) !=
          0u)
      {
         result = -EFAULT;
         break;
      }

      if ((in.conn >= FLOW_NUM_CONNECTIONS) ||
          (in.len < FLOW_PDU_MIN_SIZE) || (in.len > FLOW_PDU_MAX))
      {
         result = -EINVAL;
         break;
      }

      if (flow_a55_put_rx (&device->a55, in.conn, in.pdu, in.len) !=
          FLOW_A55_OK)
      {
         result = -EIO;
      }
      break;
   }

   case FLOW_A55_IOCTL_GET_STATUS:
   {
      struct flow_a55_status status;

      memset (&status, 0, sizeof (status));
      status.header_state = device->shm->header.state;
      status.heartbeat[0] = device->shm->header.heartbeat[FLOW_CORE_M7];
      status.heartbeat[1] = device->shm->header.heartbeat[FLOW_CORE_M33];
      status.tx_count[0] =
         flow_shm_count (&device->shm->tx[0][FLOW_SHM_NODE_M7]);
      status.tx_count[1] =
         flow_shm_count (&device->shm->tx[0][FLOW_SHM_NODE_M33]);
      status.rx_count = flow_shm_count (&device->shm->rx[0]);
      status.invalid_frames = device->a55.invalid_frames;

      if (copy_to_user ((void __user *)argument, &status, sizeof (status)) !=
          0u)
      {
         result = -EFAULT;
      }
      break;
   }

   default:
      result = -ENOTTY;
      break;
   }

   mutex_unlock (&device->lock);

   return result;
}

static const struct file_operations flow_a55_fops =
{
   .owner = THIS_MODULE,
   .open = flow_a55_open,
   .release = flow_a55_release,
   .unlocked_ioctl = flow_a55_ioctl,
};


static int flow_a55_probe (struct platform_device * platform_device)
{
   struct flow_a55_device * device;
   struct resource * resource;
   resource_size_t size;
   int result;

   resource = platform_get_resource (platform_device, IORESOURCE_MEM, 0);
   if (resource == NULL)
   {
      dev_err (&platform_device->dev, "missing SHMEM memory resource\n");
      return -ENODEV;
   }

   size = resource_size (resource);
   if (size < sizeof (flow_shm_t))
   {
      dev_err (&platform_device->dev, "SHMEM region too small\n");
      return -EINVAL;
   }

   device = devm_kzalloc (
      &platform_device->dev, sizeof (*device), GFP_KERNEL);
   if (device == NULL)
   {
      return -ENOMEM;
   }

   device->base = devm_ioremap (
      &platform_device->dev, resource->start, size);
   if (device->base == NULL)
   {
      dev_err (&platform_device->dev, "cannot map the SHMEM region\n");
      return -ENOMEM;
   }

   device->shm = (flow_shm_t *)device->base;

   if ((device->shm->header.magic != FLOW_SHM_MAGIC) ||
       (device->shm->header.state != FLOW_SHM_STATE_READY))
   {
      /* The safety cores have not finished initialising the mailbox yet. */
      return -EPROBE_DEFER;
   }

   if (!flow_shm_validate (device->shm))
   {
      dev_err (&platform_device->dev,
         "SHMEM ABI mismatch (version %u, layout 0x%08x)\n",
         (unsigned)device->shm->header.version,
         (unsigned)device->shm->header.layout_hash);
      return -EPROTO;
   }

   mutex_init (&device->lock);
   flow_a55_init (&device->a55, device->shm);

   device->misc.minor = MISC_DYNAMIC_MINOR;
   device->misc.name = "flow-a55";
   device->misc.fops = &flow_a55_fops;
   device->misc.parent = &platform_device->dev;

   result = misc_register (&device->misc);
   if (result != 0)
   {
      dev_err (&platform_device->dev, "cannot register /dev/flow-a55\n");
      return result;
   }

   platform_set_drvdata (platform_device, device);

#if FLOW_A55_DEBUG
   dev_info (&platform_device->dev,
      "mapped SHMEM, protocol 0x%08x version %u layout 0x%08x\n",
      (unsigned)device->shm->header.protocol_id,
      (unsigned)device->shm->header.version,
      (unsigned)device->shm->header.layout_hash);
#endif

   return 0;
}

static int flow_a55_remove (struct platform_device * platform_device)
{
   struct flow_a55_device * device =
      (struct flow_a55_device *)platform_get_drvdata (platform_device);

   misc_deregister (&device->misc);
   mutex_destroy (&device->lock);

   return 0;
}

static const struct of_device_id flow_a55_of_match[] =
{
   { .compatible = "fsl,imx943-flow-shm" },
   { }
};
MODULE_DEVICE_TABLE (of, flow_a55_of_match);

static struct platform_driver flow_a55_driver =
{
   .driver =
   {
      .name = "imx943-flow-a55",
      .of_match_table = flow_a55_of_match,
   },
   .probe = flow_a55_probe,
   .remove = flow_a55_remove,
};

module_platform_driver (flow_a55_driver);

MODULE_AUTHOR ("imx943-flow");
MODULE_DESCRIPTION ("A55 shared memory transport for the i.MX943 1oo2 FSoE safety cores");
MODULE_LICENSE ("GPL");

