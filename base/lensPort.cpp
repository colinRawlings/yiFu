#include "lensPort.h"

#include "HWdefs.h"
#include "SWdefs.h"

//-----------------------------------------------------------------
// macros
//-----------------------------------------------------------------

#define clkOut() SPI_DIR |= CLK_byte          // raise this bit, leave others unchanged
#define clkIn() SPI_DIR &= ~CLK_byte          // lower this bit, leave others unchanged
#define clockIsHigh() (SPI_INPORT & CLK_byte) // read clock state
#define clkHigh() SPI_OUTPORT |= CLK_byte
#define clkLow() SPI_OUTPORT &= ~CLK_byte
#define MOSI_High() SPI_OUTPORT = ((SPI_OUTPORT) | MOSI_byte) & ~CLK_byte
// set on clk falling edge
#define MOSI_Low() SPI_OUTPORT = ((SPI_OUTPORT) & ~MOSI_byte) & ~CLK_byte // set on clk edge
                                                                          // set on clk falling edge, lower both MOSI and CLK bits
#define MISO_In() ((SPI_INPORT >> MISO_Bit) & 0x01)
#define NOP __asm__ __volatile__("nop\n\t")

//-----------------------------------------------------------------
// class methods
//-----------------------------------------------------------------
lensPort::lensPort()
{
    CLK_byte = (1 << CLK_BIT);
    MOSI_byte = (1 << MOSI_BIT);

    //

    SPI_DIR = 0x00 | CLK_byte | MOSI_byte;
    SPI_OUTPORT = 0xFF;
}

//-----------------------------------------------------------------
errorCodes lensPort::_sendSlowByte(uint8_t val_tmp_MOSI, uint8_t &val_MISO)
{
    /* send bytes with a 6us clock half duration.*/

    // --- variables
    int SPI_outBits[8];
    uint8_t SPI_inBytes[8];

    val_MISO = 0x00;

    //--- timing variables
    int N_nop_lo = 19; // done with a for loop so not 62.5ns/nop
    int N_nop_hi = 16;
    int N_nop_post = 3;

    //

    for (int i = 0; i < 8; i++)
    {
        SPI_outBits[i] = (val_tmp_MOSI & 0x80) >> 7;
        val_tmp_MOSI = val_tmp_MOSI << 1;
    }

    //--- ensure correct o/p state for CLK
    clkHigh(); //clock should be high already
    clkOut();

    //--- begin SPI transmission
    noInterrupts();

    for (int i = 0; i < 8; i++)
    {
        if (SPI_outBits[i] == 1)
            MOSI_High();
        else
            MOSI_Low();

        for (int j = 0; j < N_nop_lo; j++)
            NOP;

        clkHigh();
        SPI_inBytes[i] = SPI_INPORT;

        for (int j = 0; j < N_nop_hi; j++)
            NOP;
    }

    //--- get ready for ack
    clkIn(); //convert to an input (n.b. pulled up so we can do this early)
    for (int j = 0; j < N_nop_post; j++)
        NOP;

    SPI_OUTPORT = (SPI_OUTPORT) | MOSI_byte; // put MOSI high again

    if (errorCodes err = _waitForLens(DT_LENS_TIMEOUT_US))
        return err;

    //--- calc val_MISO
    uint8_t val_tmp_MISO = 0x00;

    for (int i = 0; i < 8; i++)
        val_tmp_MISO = (val_tmp_MISO << 1) | ((SPI_inBytes[i] >> MISO_BIT) & 0x01);
    val_MISO = val_tmp_MISO;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::_sendFastByte(uint8_t val_tmp_MOSI, uint8_t &val_MISO)
{
    /* send bytes with a 0.5us clock half duration.*/

    // --- variables
    int SPI_outBits[8];
    uint8_t SPI_inBytes[8];

    val_MISO = 0x00;

    //--- calculate theByte (do some pre-calculation for speed)
    for (int i = 0; i < 8; i++)
    {
        SPI_outBits[i] = (val_tmp_MOSI & 0x80) >> 7;
        val_tmp_MOSI = val_tmp_MOSI << 1;
    }

    //--- ensure correct o/p state for CLK
    clkHigh(); //clock should be high already
    clkOut();

    //--- begin serial transmission
    noInterrupts();

    for (int i = 0; i < 8; i++)
    {

        //set clock and output bits
        if (SPI_outBits[i] == 1)
            MOSI_High();
        else
            MOSI_Low();

        // set clock half period
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        NOP;

        // now do read portion of bit: this will be retained while next state is calculated
        clkHigh();
        SPI_inBytes[i] = SPI_INPORT; //start writing and shifting the input bits leftwards (MSB first)
    }

    // get ready for ack
    clkIn(); //convert to an input (n.b. pulled up so we can do this early)
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    SPI_OUTPORT = (SPI_OUTPORT) | MOSI_byte; // put MOSI high again

    if (errorCodes err = _waitForLens(DT_LENS_TIMEOUT_US))
        return err;

    //--- calc val_MISO
    uint8_t val_tmp_MISO = 0x00;

    for (int i = 0; i < 8; i++)
        val_tmp_MISO = (val_tmp_MISO << 1) | ((SPI_inBytes[i] >> MISO_BIT) & 0x01);
    val_MISO = val_tmp_MISO;

    return SUCCESS;
}

//-----------------------------------------------------
errorCodes lensPort::_waitForLens(unsigned long timeout_us)
{

    unsigned long T0; // timer value when ACK sequence started
    unsigned long T1; //timer value when clk pulled down by lens
    unsigned long T2; // timer value when clk released by lens
    bool timed_out = false;

    unsigned long n_iter = 0;
    unsigned long N1_iter = 1e2;

    T1 = T0;

    // wait first for clk line to be pulled down
    while ((timed_out == false) && clockIsHigh())
    {
        T1 = micros(); // this just wastes time, interrupts are still off
        if (N1_iter < n_iter)
        {
            interrupts();
            return LENS_PORT_CLK_PULL_DOWN_TIMEOUT;
        }

        n_iter++;
    }

    interrupts();  // now can re-enable the interrupts no risk of missing release as this is the steady state
                   // n.b. now can we also rely on micros for longer events
    T0 = micros(); // n.b. stored in 4us increments, should work correctly now that interrupts are back
    T2 = T0;

    // wait for clk line to be released
    do
    {
        T2 = micros();

        if (timeout_us < (T2 - T0))
            timed_out = true;

    } while (!timed_out && !clockIsHigh());

    if (timed_out == true)
        return LENS_PORT_CLK_RELEASE_TIMEOUT;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::setMsg(uint8_t msg[], unsigned int msgLength_)
{
    msgAvailable = false;
    msgLength = 0;

    if (msgLength > SPI_BUFFER_LENGTH)
        return LENS_PORT_SPI_BUFFER_TOO_SMALL;

    msgLength = msgLength_;

    for (unsigned int p; p < msgLength; p++)
    {
        SPI_MOSI_buffer[p] = msg[p];
        SPI_MISO_buffer[p] = 0x00;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::getMsg(uint8_t msg[], unsigned int &msgLength_)
{

    for (unsigned int p; p < msgLength; p++)
        msg[p] = SPI_MISO_buffer[p];

    msgLength_ = msgLength;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::getMsgLength(unsigned int &msgLength_)
{
    msgLength_ = msgLength;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::getAnswer(uint8_t answer[], unsigned int &answerLength)
{
    if (!msgAvailable)
        return LENS_PORT_NO_ANSWER_AVAILABLE;

    for (unsigned int p; p < msgLength; p++)
        answer[p] = SPI_MISO_buffer[p];

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::sendSlowMsg()
{
    for (unsigned int i = 0; i < msgLength; i++)
    {
        if (int err = _sendSlowByte(SPI_MOSI_buffer[i], SPI_MISO_buffer[i]))
            return err;

        delayMicroseconds(DT_BETWEEN_SLOW_BYTES_US);
    }

    msgAvailable = true;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensPort::sendFastMsg()
{
    for (unsigned int i = 0; i < msgLength; i++)
    {
        if (int err = _sendFastByte(SPI_MOSI_buffer[i], SPI_MISO_buffer[i]))
            return err;

        delayMicroseconds(DT_BETWEEN_FAST_BYTES_US);
    }

    msgAvailable = true;

    return SUCCESS;
}
