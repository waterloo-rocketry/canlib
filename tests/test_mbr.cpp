#include <stdint.h>

#include "common.h"
#include "mbr.h"

#include "rockettest.hpp"

class mbr_test : rockettest_test {
public:
	mbr_test() : rockettest_test("mbr_test") {}

	bool run_test() override {
		bool test_passed = true;

		// Example partition type
		uint8_t partition_type = 0x07;

		// Declare sector lba
		uint32_t sector_lba;

		// Example first sector
		uint8_t first_sector[512] = {0};
		uint8_t *entry = first_sector + 0x1BE;

		entry[0] = 0x80; // boot flag
		entry[4] = partition_type; // partition type
		// Imaginary LBA: 0x001100000
		entry[8] = 0x00; // LBA low byte (LBA is little-endian)
		entry[9] = 0x00;
		entry[10] = 0x11;
		entry[11] = 0x00; // LBA high byte

		// Test with invalid boot signature
		rockettest_assert(mbr_parse(first_sector, partition_type, &sector_lba) == W_FAILURE);

		uint8_t *boot_signature = first_sector + 0x1FE;
		boot_signature[0] = 0x55;
		boot_signature[1] = 0xAA;

		// Test with valid input
		rockettest_assert(mbr_parse(first_sector, partition_type, &sector_lba) == W_SUCCESS);
		// Test with invalid input (NULL first sector)
		rockettest_assert(mbr_parse(nullptr, partition_type, &sector_lba) == W_INVALID_PARAM);
		// Test with invalid input (NULL LBA)
		rockettest_assert(mbr_parse(first_sector, partition_type, nullptr) == W_INVALID_PARAM);
		// Test with a partition type that cannot be found
		rockettest_assert(mbr_parse(first_sector, 0x06, &sector_lba) == W_FAILURE);

		return test_passed;
	}
};

mbr_test mbr_test_inst;