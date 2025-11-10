#include <stdbool.h>
#include <stdint.h>

#include "common.h"

/**
 * @brief To find the starting sector of a given type of partition
 *
 * This function finds the lba for the partition that matches the partition type
 * provided in the first sector. Put lba as 0 if the corresponding partition
 * cannot be found.
 *
 * @param first_sector 512 byte buffer contains content of first sector
 * @param partition_type The partition type trying to find
 * @param sector_lba Pointer to the start offset of the sector
 * @return w_status_t Returns W_SUCCESS on success, W_FAILURE if partition cannot be found
 */
w_status_t mbr_parse(uint8_t *first_sector, uint8_t partition_type, uint32_t *sector_lba);
