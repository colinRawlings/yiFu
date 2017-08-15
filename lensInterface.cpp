#include "lensInterface.h"

//-----------------------------------------------------------------
// constants
//-----------------------------------------------------------------

#define delayBetweenSlowBytes_us 120
#define delayBetweenFastBytes_us 10

#define SPI_OUTPORT PORTB
#define SPI_INPORT PINB
#define SPI_DIR DDRB
#define LPOW_PORT PORTB

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
lensInterface::lensInterface(uint8_t MOSI_bit_, uint8_t MISO_bit_, uint8_t CLK_bit_)
{
    MOSI_bit = MOSI_bit_;
    MISO_bit = MISO_bit_;
    CLK_bit = CLK_bit_;

    //

    CLK_byte = (1 << CLK_bit);
    MOSI_byte = (1 << MOSI_bit);

    //

    SPI_DIR = 0x00 | CLK_byte | MOSI_byte;
    SPI_OUTPORT = 0xFF;
}

//-----------------------------------------------------------------
int lensInterface::_sendSlowByte(uint8_t val_tmp_MOSI, uint8_t *val_MISO)
{
    /* send bytes with a 6us clock half duration.*/

    // --- variables
    int SPI_outBits[8];
    uint8_t SPI_inBytes[8];

    //--- tuning variables
    unsigned long timeout_us = 30000;
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

    if (int err = _waitForLens(timeout_us))
        return err;

    //--- calc val_MISO
    uint8_t val_tmp_MISO = 0x00;

    for (int i = 0; i < 8; i++)
        val_tmp_MISO = (val_tmp_MISO << 1) | ((SPI_inBytes[i] >> MISO_bit) & 0x01);
    val_MISO = &val_tmp_MISO;

    return 0;
}

//-----------------------------------------------------
int lensInterface::_waitForLens(unsigned long timeout_us)
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
            timed_out = true;

        n_iter += 1;
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

    // if (timed_out == true) DEBUG
    //     return 1;

    return 0;
}

//-----------------------------------------------------------------
int lensInterface::setMsg(uint8_t msg[], unsigned int msgLength_)
{
    msgAvailable = false;

    if (msgLength > SPI_BUFFER_LENGTH)
        return 1;

    msgLength = msgLength_;

    for (unsigned int p; p < msgLength; p++)
    {
        SPI_MOSI_buffer[p] = msg[p];
        SPI_MISO_buffer[p] = 0x00;
    }

    return 0;
}

//-----------------------------------------------------------------
int lensInterface::getAnswer(uint8_t answer[], unsigned int &answerLength)
{
    if (!msgAvailable)
        return 1;

    for (unsigned int p; p < msgLength; p++)
        answer[p] = SPI_MISO_buffer[p];

    return 0;
}

//-----------------------------------------------------------------
int lensInterface::sendSlowMsg()
{
    for (unsigned int i = 0; i < msgLength; i++)
    {
        if (int err = _sendSlowByte(SPI_MOSI_buffer[i], &SPI_MISO_buffer[i]))
            return err;

        delayMicroseconds(delayBetweenSlowBytes_us);
    }

    msgAvailable = true;

    return 0;
}

//-----------------------------------------------------------------
// debug methods (mostly move to UI)
//-----------------------------------------------------------------
lensInterface::setSerialPort(Stream *theSerialPort_)
{
    theSerialPort = theSerialPort_;

    //

    theSerialPort->println("lensInterface added the serial port");
}

//-----------------------------------------------------------------
void lensInterface::sayHi()
{
    theSerialPort->println("Hello");

    unsigned int CRlength = 3;
    uint8_t CRsequence[] = {0x00, 0x0A, 0x00};
    uint8_t CRresponse[CRlength];

    //

    if (int err = setMsg(CRsequence, CRlength))
        theSerialPort->println("failed to set msg");

    if (int err = sendSlowMsg())
        theSerialPort->println("failed to send msg");

    if (int err = getAnswer(CRresponse, CRlength))
        theSerialPort->println("failed to get answer");

    //

    for (unsigned int p; p < CRlength; p++)
    {
        Serial.print(CRresponse[p]);
        Serial.print(",");
    }
    Serial.println(".");
}
