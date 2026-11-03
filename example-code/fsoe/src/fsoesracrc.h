/**
 * \file
 * \brief SRA CRC
 *
 * Generated using the configuration:
 *  - Width         = 32
 *  - Poly          = 0x814141ab
 *  - XorIn         = 0x00000000
 *  - ReflectIn     = False
 *  - XorOut        = 0x00000000
 *  - ReflectOut    = False
 *  - Algorithm     = table-driven
 *
 * This file defines the function fsoesracrc_update().
 *
 * The fsoesracrc_update() function can be called any number of times (including zero
 * times).
 *
 * This pseudo-code shows an example usage of the API:
 * \code{.c}
 * fsoesracrc_t crc;
 * unsigned char data[MAX_DATA_LEN];
 * size_t data_len;
 *
 * crc = 0;
 * while ((data_len = read_data(data, MAX_DATA_LEN)) > 0) {
 *     crc = fsoesracrc_update(crc, data, data_len);
 * }
 * \endcode
 */
#ifndef FSOESRACRC_H
#define FSOESRACRC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 */
typedef uint32_t fsoesracrc_t;

/**
 * Update the crc value with new data.
 *
 * \param[in] crc_in   The current crc value.
 * \param[in] data     Pointer to a buffer of \a size bytes.
 * \param[in] size     Number of bytes in the \a data buffer.
 * \return             The updated crc value.
 */
fsoesracrc_t fsoesracrc_update (fsoesracrc_t crc_in, const void * data, size_t size);

#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* FSOESRACRC_H */
