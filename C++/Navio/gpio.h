
#ifndef __NAVIO_GPIO_H__
#define __NAVIO_GPIO_H__

#include <stdint.h>

namespace Navio {

/* Raspberry Pi GPIO mapping */
#define RPI_GPIO_2    2    // Pin 3     SDA
#define RPI_GPIO_3    3    // Pin 5     SCL
#define RPI_GPIO_4    4    // Pin 7             NAVIO_PPM_INPUT
#define RPI_GPIO_6    6
#define RPI_GPIO_7    7    // Pin 26    CE1     NAVIO_MPU9250_CS
#define RPI_GPIO_8    8    // Pin 24    CE0     NAVIO_UBLOX_CS
#define RPI_GPIO_9    9    // Pin 21    MISO
#define RPI_GPIO_10   10   // Pin 19    MOSI
#define RPI_GPIO_11   11   // Pin 23    SCLK
#define RPI_GPIO_14   14   // Pin 8     TxD
#define RPI_GPIO_15   15   // Pin 10    RxD
#define RPI_GPIO_17   17   // Pin 11            NAVIO_UART_PORT_5
#define RPI_GPIO_18   18   // Pin 12            NAVIO_UART_PORT_4
#define RPI_GPIO_22   22   // Pin 15            NAVIO_UBLOX_PPS
#define RPI_GPIO_23   23   // Pin 16            NAVIO_MPU9250_DRDY
#define RPI_GPIO_24   24   // Pin 18            NAVIO_SPI_PORT_6
#define RPI_GPIO_25   25   // Pin 22            NAVIO_SPI_PORT_5
#define RPI_GPIO_27   27   // Pin 13            NAVIO_PCA9685_OE
#define RPI_GPIO_28   28   // Pin 3
#define RPI_GPIO_29   29   // Pin 4
#define RPI_GPIO_30   30   // Pin 5
#define RPI_GPIO_31   31   // Pin 6

class Pin {
public:
    typedef enum {
        GpioModeInput,
        GpioModeOutput
    } GpioMode;

    Pin(uint8_t pin);
    ~Pin();

    bool    init();
    void    setMode(GpioMode mode);
    uint8_t read() const;
    void    write(uint8_t value);
    void    toggle();

private:
    int getRaspberryPiVersion() const;
    Pin (const Pin&);
    Pin& operator=(const Pin&);

    uint8_t _pin;
    volatile uint32_t *_gpio;
    GpioMode _mode;

    bool    _deinit();
};

}

#endif // __NAVIO_GPIO_H__
