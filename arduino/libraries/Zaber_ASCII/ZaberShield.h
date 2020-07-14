/*
    ZaberShield - Adapter class to use the Zaber Arduino shield as a serial port.

    Description: This class handles I2C communication with the SC16IS740 UART
    bridge chip, as found on the Zaber Arduino shield, and makes the shield appear
    as a serial port to your code. You can use this class as a substitute for
    the regular Serial class when using the Zaber Arduino library with the
    Zaber Arduino shield. It may also work with other shields that use the same
    chip.

    This code is derived from Sandbox Electronics' UART_Bridge example code by
    Tiequan Shao, and is adapted and redistributed under the Apache 2 license
    with their kind permission. The original code (which also works with the
    Zaber shield) is available here: https://github.com/SandboxElectronics/UART_Bridge

    Note the original Sandbox implementation supports both I2C and SPI to serial
    bridging, ann the GPIO pins of the SC16IS750 chip. This modified Zaber version
    only supports I2C because the Zaber shield only uses an I2C connection and
    does not have the GPIO functionality. If you are also using an SPI connection
    on another shield, consider using the Sandbox library instead of this class.

    The license statement provided by Sandbox Electronics appears below:
    ---------------------------

    Description:
    This is a example code for Sandbox Electronics' I2C/SPI to UART bridge module.
    You can get one of those products on
    http://sandboxelectronics.com

    Version:
    V0.1

    Release Date:
    2014-02-16

    Author:
    Tiequan Shao          info@sandboxelectronics.com

    Lisence:
    Apache License Version 2.0

    Please keep the above information when you use this code in your project.
*/


#ifndef _ZABERSHIELD_H_
#define _ZABERSHIELD_H_

// Prevent multiple definition of this class if using both the ASCII and Binary Zaber libraries.
#define _USING_ZABERSHIELD_IN_ASCII_LIBRARY_

#include <Arduino.h>

// Device Address
// The I2C address of the UART bridge chip on the Zaber shield is configured
// with two jumpers on the shield. The address you use in your sketch with 
// the ZaberShield class must match the jumper settings.
// Note that if you change the jumpers on the shield, a power cycle is needed
// for the shield to pick up the new address.
#define     ZABERSHIELD_ADDRESS_AA     (0x90)
#define     ZABERSHIELD_ADDRESS_AB     (0x92)
#define     ZABERSHIELD_ADDRESS_BA     (0x98)
#define     ZABERSHIELD_ADDRESS_BB     (0x9A)


/*
@brief Helper class that presents the Zaber shield as a Stream.

This class hides the I2C communication with the Zaber shield so it can
be used like a serial port. This class is only needed if you are using the
Zaber shield, not with other types of serial ports.
*/
class ZaberShield : public Stream
{
public:

    /*
    @brief Initializes the ZaberShield class.

    @param address The I2C address of the shield. This is configured with
    jumpers on the shield, and you must provide the correct address here in
    order to be able to communicate with the shield.
    */
    ZaberShield(uint8_t address = ZABERSHIELD_ADDRESS_AA);

    /*
    @brief Begins a serial session with a specified baud rate.

    @param baudrate The desired baud rate to use.
    */
    void begin(uint32_t baudrate);

    /*
    @brief Ends the serial session.
    */
    void end(void);

    /*
    @brief Determines how many bytes are available to be read from the receive buffer.

    @return The number of bytes unconsumed in the receive buffer. The Zaber
    shield has a 64-byte receive buffer - if this function returns 64 you
    may not be consuming data fast enough, and some may have been lost.
    */
    virtual int available(void);

    /*
    @brief Peek at the next byte in the receive buffer without consuming it.

    @return The next byte that will be returned by read(), or -1 if there aren't any.
    */
    virtual int peek(void);

    /*
    @brief Read a byte from the receive buffer.

    @return The next byte from the receive FIFO, or -1 if it's empty.
    */
    virtual int read(void);

    /*
    @brief Determine how much space is available in the transmit buffer.

    @return The number of bytes that can be written without data loss.
    */
    virtual int availableForWrite(void);

    /*
    @brief Block until all data in the transmit buffer has been sent.
    */
    virtual void flush(void);

    /*
    @brief Write a byte to the transmit buffer.

    @param data The byte to output.
    @return The number of bytes written (always 1).
    */
    virtual size_t write(uint8_t data);

private:

    int16_t SetBaudrate(uint32_t baudrate);
    uint8_t ReadRegister(uint8_t reg_addr);
    void    WriteRegister(uint8_t reg_addr, uint8_t val);
    void    SetLCR();
    void    ResetDevice(void);

    void    FIFOEnable(uint8_t fifo_enable);
    uint8_t FIFOAvailableData(void);
    uint8_t FIFOAvailableSpace(void);
    void    WriteByte(uint8_t val);
    int     ReadByte(void);

    uint8_t _i2cAddress;
    int 	_peekBuffer;
    uint8_t _peekFlag;
};

#endif







