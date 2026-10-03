#ifndef ROCKETLIB_CRC8_H
#define ROCKETLIB_CRC8_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Compute CRC8
 *
 * Computes the CRC8 checksum of input data
 *
 * @param pdata Input data buffer
 * @param nbytes Buffer size in bytes
 * @param crc CRC8 checksum of previous CRC8 calculation if calculating checksum on multiple
 * discontinuous buffers, otherwise 0
 * @return Computed CRC8 checksum
 */
uint8_t crc8_checksum(const uint8_t *pdata, size_t nbytes, uint8_t crc);

#ifdef __cplusplus
}
#endif

#endif
