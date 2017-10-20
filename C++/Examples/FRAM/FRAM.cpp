#include <stdio.h>
#include <unistd.h>
#include <Common/I2Cdev.h>
#include <Common/Util.h>
#include <Navio+/MB85RC256.h>

// 32768 bytes >> 0x8000 >> 15 bit address

int main()
{
    if (get_navio_version() == NAVIO2) {

        fprintf(stderr, "No FRAM on NAVIO2\n");
        return 1;
    }


	uint8_t dev_address = 0b1010000;
	uint16_t reg_address = 0;
	uint8_t data = 0xCC;
	bool flag = true;

    if (check_apm()) {
        return 1;
    }

	printf("Fram memory Write/Read test!\nWe will write value 0xCC to the address 0 of fram memory, and then read it:\n");

	MB85RC256 fram;

	printf("Writing data...\n");

    fram.writeByte(reg_address, data);

	data = 0x00;

	printf("Reading data...\n");

	fram.readByte(reg_address, &data);

	printf("Data, read from address 0 is %x\n", data);

	if (data != 0xCC) flag = false;

	// multiple write and read functionality test

	printf("Multiple read/write functionality test!\nWe will write values 0xAA, 0xBB, 0xCC to the addresses 0, 1, 2 of the fram memory \nand then read it:\n");

	uint8_t a[3] = {0xAA, 0xBB, 0xCC};

	printf("Writing data...\n");

	fram.writeBytes(reg_address, 3, a);

	for (int i=0; i<3; i++) a[i] = 0;

	printf("Reading data...\n");

	fram.readBytes(reg_address, 3, a);

	printf("Data, read from the addresses 0, 1, 2:\n%x %x %x\n", a[0], a[1], a[2]);

	if ((a[0] != 0xAA) || (a[1] != 0xBB) || (a[2] != 0xCC)) flag = false;

	if (flag == true) {
		printf("Memory test passed succesfully\n");
	} else {
		printf("Memory test not passed\n");
	}

	return 0;
}
