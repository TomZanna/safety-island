#if !defined(_SYSTEM_HEADER_H_)
#define _SYSTEM_HEADER_H_

#define DBGFMT	"Debug %s:%s:%d "
#define DBGARGS	__FILE__,__FUNCTION__,__LINE__

#define DBG_NONE	0x00
#define DBG_DEFAULT	0x01
#define DBG_PDO	0x02

#endif // _SYSTEM_HEADER_H_
