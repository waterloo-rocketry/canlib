#ifndef ROCKETLIB_CRC8_H
#define ROCKETLIB_CRC8_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t crc8_checksum(uint8_t *pdata, size_t nbytes, uint8_t crc);

#ifdef __cplusplus
}
#endif

#endif
