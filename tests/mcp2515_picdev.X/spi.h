#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_init(void);
void spi_write(uint8_t data);
uint8_t spi_read(void);
void cs_drive(uint8_t state);

#endif
