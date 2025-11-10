#include <mbr.h>

#define MBR_PT_OFFSET 0x1BE
#define MBR_PT_SIZE 16
#define MBR_TYPE_OFF 4
#define MBR_LBA_OFF 8

w_status_t mbr_parse(uint8_t *first_sector, uint8_t partition_type, uint32_t *sector_lba) {
	if (!first_sector || !sector_lba) {
		return W_INVALID_PARAM;
	}

	// Set sector_lba to 0 as default
	*sector_lba = 0;

	// First partition entry starts at 0x01BE in the first sector
	uint8_t *entry = first_sector + MBR_PT_OFFSET;

	// Each partition entry has a size of 16 bytes
	for (int i = 0; i < 4; i++, entry += MBR_PT_SIZE) {
		// Partition type locates at 0x04 in each partition
		uint8_t type = entry[MBR_TYPE_OFF];
		if (type == partition_type) {
			*sector_lba = (uint32_t)entry[MBR_LBA_OFF] | ((uint32_t)entry[MBR_LBA_OFF + 1] << 8) |
						  ((uint32_t)entry[MBR_LBA_OFF + 2] << 16) |
						  ((uint32_t)entry[MBR_LBA_OFF + 3] << 24);
			return W_SUCCESS;
		}
	}
	return W_FAILURE;
}
