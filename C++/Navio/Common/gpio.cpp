#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <err.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "gpio.h"

#define LOW                 0
#define HIGH                1

/* Raspberry Pi GPIO memory */
#define BCM2708_PERI_BASE   0x20000000
#define BCM2709_PERI_BASE   0x3F000000
#define BCM2835_PERI_BASE   0x20000000
#define GPIO_BASE(address)  (address + 0x200000)
#define PAGE_SIZE           (4*1024)
#define BLOCK_SIZE          (4*1024)

/* GPIO setup. Always use INP_GPIO(x) before OUT_GPIO(x) or SET_GPIO_ALT(x,y) */
#define GPIO_MODE_IN(g)     *(_gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_MODE_OUT(g)    *(_gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_MODE_ALT(g,a)  *(_gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET_HIGH       *(_gpio+7)  // sets   bits which are 1
#define GPIO_SET_LOW        *(_gpio+10) // clears bits which are 1
#define GPIO_GET(g)         (*(_gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define MAX_SIZE_LINE       50

using namespace Navio;

Pin::Pin(uint8_t pin):
    _pin(pin),
    _gpio(NULL), 
    _mode(GpioModeInput)
{
}

Pin::~Pin()
{
    if (!_deinit()) {
        warnx("deinitialization failed");
    }
}

bool Pin::_deinit() 
{
    if (munmap(const_cast<uint32_t *>(_gpio), BLOCK_SIZE) < 0) {
        warnx("unmap failed");
        return false;
    }
    return true;
}

bool Pin::init()
{
    int mem_fd;
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        warn("/dev/mem cannot be opened");
        return false;
    }

    uint32_t address;
    int version = getRaspberryPiVersion();
    if (version == 1) {
        address = GPIO_BASE(BCM2708_PERI_BASE);
    } else if (version == 2) {
        address = GPIO_BASE(BCM2709_PERI_BASE);
    } else if (version == 3) {
        address = GPIO_BASE(BCM2835_PERI_BASE);
    }

    void *gpio_map = mmap(
        NULL,                 /* any adddress in our space will do */
        BLOCK_SIZE,           /* map length */
        PROT_READ|PROT_WRITE, /* enable reading & writting to mapped memory */
        MAP_SHARED,           /* shared with other processes */
        mem_fd,               /* file to map */
        address               /* offset to GPIO peripheral */
    );

    if (gpio_map == MAP_FAILED) {
        warn("cannot mmap memory");
        return false;
    }

    /* No need to keep mem_fd open after mmap */
    if (close(mem_fd) < 0) {
        warn("cannot close mem_fd");   
        return false;
    } 

    _gpio = reinterpret_cast<volatile uint32_t *>(gpio_map); // Always use volatile pointer!

    return true;
}

void Pin::setMode(GpioMode mode)
{
    if (mode == GpioModeInput) {
        GPIO_MODE_IN(_pin);
    } else {
        GPIO_MODE_IN(_pin);
        GPIO_MODE_OUT(_pin);
    }

    _mode = mode;
}

uint8_t Pin::read() const
{
    uint32_t value = GPIO_GET(_pin);
    return value ? 1: 0;
}

void Pin::write(uint8_t value)
{
    if (_mode != GpioModeOutput) {
        warnx("no effect because mode is not set");
    }

    if (value == LOW) {
        GPIO_SET_LOW = 1 << _pin;
    } else {
        GPIO_SET_HIGH = 1 << _pin;
    }
}

void Pin::toggle()
{
    write(!read());
}

int Pin::getRaspberryPiVersion() const
{
    char buffer[MAX_SIZE_LINE];
    const char* hardware_description_entry = "Hardware";
    const char* v1 = "BCM2708";
    const char* v2 = "BCM2709";
    const char* v3 = "BCM2835";
    char* flag;
    FILE* fd;

    fd = fopen("/proc/cpuinfo", "r");

    while (fgets(buffer, MAX_SIZE_LINE, fd) != NULL) {
        flag = strstr(buffer, hardware_description_entry);

        if (flag != NULL) {
            if (strstr(buffer, v3) != NULL) {
                fclose(fd);
                return 3;
            } else if (strstr(buffer, v2) != NULL) {
                fclose(fd);
                return 2;
            } else if (strstr(buffer, v1) != NULL) {
                fclose(fd);
                return 1;
            }
        }
    }

    /* defaults to 1 */
    fprintf(stderr, "Could not detect RPi version, defaulting to 1\n");
    fclose(fd);
    return 1;
}
