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


#include <ZaberShield.h>

// Prevent multiple definition if someone is using both the ASCII and Binary libs.
#ifdef _USING_ZABERSHIELD_IN_ASCII_LIBRARY_

#include <Wire.h>

// Uncomment this line to enable debug printing over the default serial port.
//#define ZABERSHIELD_DEBUG_PRINT_ENABLE

#define     SC16IS740_CRYSTAL_FREQ (24000000UL) 

// Names for the UART bridge chip's registers.
#define     SC16IS740_REG_RHR        (0x00) // Receive Holding Register (head of Rx FIFO)
#define     SC16IS740_REG_THR        (0X00) // Transmit Holding Register (tail + 1 of Tx FIFO)
#define     SC16IS740_REG_FCR        (0X02) // FIFO Control Register
#define     SC16IS740_REG_IIR        (0X02) // Interrupt Identification Register
#define     SC16IS740_REG_LCR        (0X03) // Line Control Register
#define     SC16IS740_REG_MCR        (0X04) // Modem Control Register
#define     SC16IS740_REG_LSR        (0X05) // Line Status Register
#define     SC16IS740_REG_TXLVL      (0X08) // Transmit FIFO Level Register
#define     SC16IS740_REG_RXLVL      (0X09) // Receive FIFO Level Register
#define     SC16IS740_REG_DLL        (0x00) // Divisor Latch LSB
#define     SC16IS740_REG_DLH        (0X01) // Divisor Latch MSB

// Names for specific bits in some of the registers.
#define     SC16IS740_BIT_LSR_THR_EMPTY         (0x20) // Transmit Hold Register empty bit in LSR.
#define     SC16IS740_BIT_MSR_CLOCK_DIVISOR     (0x80) // Clock divisor mode bit in MSR.
#define     SC16IS740_BIT_LCR_LATCH_ENABLE      (0x80) // Divisor Latch Enable bit in LCR.
#define     SC16IS740_BIT_LCR_SET_BREAK         (0x40) // Set Break bit in LCR.
#define     SC16IS740_BIT_LCR_WORD_LENGTH_8     (0x03) // 8-bit word length in LCR.
#define     SC16IS740_BIT_IIR_FIFO_ENABLE       (0x80) // FIFO Enabled state in IIR.
#define     SC16IS740_BIT_FCR_TX_FIFO_RESET     (0x04) // Transmit FIFO Reset bit in FCR.
#define     SC16IS740_BIT_FCR_RX_FIFO_RESET     (0x02) // Receive FIFO Reset bit in FCR.
#define     SC16IS740_BIT_FCR_FIFO_ENABLE       (0x01) // FIFO Enable bit in FCR.


ZaberShield::ZaberShield(uint8_t address)
: _i2cAddress(address >> 1),
  _peekBuffer(0),
  _peekFlag(false)
{
}


void ZaberShield::begin(uint32_t baudrate)
{
    Wire.begin();
    ResetDevice();
    FIFOEnable(1);
    SetBaudrate(baudrate);
    SetLCR();
}


void ZaberShield::end(void)
{
    Wire.end();
}


int ZaberShield::available(void)
{
    int count = FIFOAvailableData();
    if (_peekFlag)
    {
        count++;
    }

    return count;
}


int ZaberShield::peek()
{
    if (!_peekFlag)
    {
        _peekBuffer = ReadByte();
        if (_peekBuffer >= 0)
        {
            _peekFlag = true;
        }
    }

    return _peekBuffer;
}


int ZaberShield::read(void)
{
    if (!_peekFlag)
    {
        return ReadByte();
    }
    else
    {
        _peekFlag = false;
        return _peekBuffer;
    }
}


int ZaberShield::availableForWrite(void)
{
    return FIFOAvailableSpace();
}


void ZaberShield::flush()
{
    uint8_t tmp_lsr;

    do
    {
        tmp_lsr = ReadRegister(SC16IS740_REG_LSR);
    } while ((tmp_lsr & SC16IS740_BIT_LSR_THR_EMPTY) == 0);
}


size_t ZaberShield::write(uint8_t data)
{
    WriteByte(data);
    return 1;
}



uint8_t ZaberShield::ReadRegister(uint8_t reg_addr)
{
    uint8_t result;
    Wire.beginTransmission(_i2cAddress);
    Wire.write(reg_addr << 3);
    Wire.endTransmission(0);
    Wire.requestFrom(_i2cAddress, (uint8_t)1);
    result = Wire.read();

    return result;
}


void ZaberShield::WriteRegister(uint8_t reg_addr, uint8_t val)
{
    Wire.beginTransmission(_i2cAddress);
    Wire.write(reg_addr << 3);
    Wire.write(val);
    Wire.endTransmission(1);

    return;
}


// returns error of baudrate parts per thousand
int16_t ZaberShield::SetBaudrate(uint32_t baudrate)
{
    uint16_t divisor;
    uint8_t prescaler;
    uint32_t actual_baudrate;
    int16_t error;
    uint8_t temp_lcr;

    // Check prescaler setting.
    if ((ReadRegister(SC16IS740_REG_MCR) & SC16IS740_BIT_MSR_CLOCK_DIVISOR) == 0)
    {
        prescaler = 1;
    }
    else
    {
        prescaler = 4;
    }

    divisor = (SC16IS740_CRYSTAL_FREQ / prescaler) / (baudrate * 16);

    // Enable writing to divisor latch.

    temp_lcr = ReadRegister(SC16IS740_REG_LCR);
    temp_lcr |= SC16IS740_BIT_LCR_LATCH_ENABLE;
    WriteRegister(SC16IS740_REG_LCR, temp_lcr);

    // Store divisor as two bytes.
    WriteRegister(SC16IS740_REG_DLL, (uint8_t)divisor);
    WriteRegister(SC16IS740_REG_DLH, (uint8_t)(divisor >> 8));

    // Disble writing to divisor latch.
    temp_lcr &= ~(SC16IS740_BIT_LCR_LATCH_ENABLE);
    WriteRegister(SC16IS740_REG_LCR, temp_lcr);

#ifdef ZABERSHIELD_DEBUG_PRINT_ENABLE
    actual_baudrate = (SC16IS740_CRYSTAL_FREQ / prescaler) / (16 * divisor);
    error = (((float)actual_baudrate - baudrate) * 1000000) / baudrate;
    Serial.print("Desired baudrate: ");
    Serial.println(baudrate, DEC);
    Serial.print("Clock prescale: ");
    Serial.println(prescaler, DEC);
    Serial.print("Calculated divisor: ");
    Serial.println(divisor, DEC);
    Serial.print("Actual baudrate: ");
    Serial.println(actual_baudrate, DEC);
    Serial.print("Baudrate error PPM: ");
    Serial.println(error, DEC);
#endif

    return error;
}


void ZaberShield::SetLCR()
{
    uint8_t temp_lcr;
    temp_lcr = ReadRegister(SC16IS740_REG_LCR);

    // Configure line control for N81.
    // Preserve existing larch enable and set break states.
    temp_lcr &= (SC16IS740_BIT_LCR_LATCH_ENABLE | SC16IS740_BIT_LCR_SET_BREAK);
    // 8 data bits.
    temp_lcr |= SC16IS740_BIT_LCR_WORD_LENGTH_8;
    // All other LCR bits should be 0 for the Zaber shield.

#ifdef ZABERSHIELD_DEBUG_PRINT_ENABLE
    Serial.print("LCR: 0x");
    Serial.println(temp_lcr, HEX);
#endif

    WriteRegister(SC16IS740_REG_LCR, temp_lcr);
}


void ZaberShield::ResetDevice(void)
{
    uint8_t reg = 0;

    // The SC16IS740 doesn't have a software reset function so
    // we clear the FIFOs instead.

    // Preserve existing FIFO enable state.
    if ((ReadRegister(SC16IS740_REG_IIR) & SC16IS740_BIT_IIR_FIFO_ENABLE) != 0)
    {
        reg = 1;
    }

    // Clear the FIFOs - these bits will auto-reset to 0 after.
    reg |= SC16IS740_BIT_FCR_RX_FIFO_RESET | SC16IS740_BIT_FCR_TX_FIFO_RESET;
    WriteRegister(SC16IS740_REG_FCR, reg);
}


void ZaberShield::FIFOEnable(uint8_t fifo_enable)
{
    uint8_t reg = 0;

    if (fifo_enable != 0)
    {
        reg |= SC16IS740_BIT_FCR_FIFO_ENABLE;
    }

    // We want all other FCR bits to be 0 when enabling or disabling the FIFOs.
    WriteRegister(SC16IS740_REG_FCR, reg);
}


uint8_t ZaberShield::FIFOAvailableData(void)
{
    uint8_t level = ReadRegister(SC16IS740_REG_RXLVL);

    return level;
}


uint8_t ZaberShield::FIFOAvailableSpace(void)
{
    return ReadRegister(SC16IS740_REG_TXLVL);
}


void ZaberShield::WriteByte(uint8_t val)
{
    uint8_t tmp_lsr;
    do
    {
        tmp_lsr = ReadRegister(SC16IS740_REG_LSR);
    } while ((tmp_lsr & SC16IS740_BIT_LSR_THR_EMPTY) == 0);

    WriteRegister(SC16IS740_REG_THR, val);
}


int ZaberShield::ReadByte(void)
{
    volatile uint8_t val;
    if (FIFOAvailableData() == 0)
    {
        return -1;
    }
    else
    {
        val = ReadRegister(SC16IS740_REG_RHR);
        return val;
    }
}

#endif // _USING_ZABERSHIELD_IN_ASCII_LIBRARY_
