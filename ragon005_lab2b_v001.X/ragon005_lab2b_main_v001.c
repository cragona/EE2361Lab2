#include "xc.h"
#include <p24Fxxxx.h>
#include "asm_ragon005_lab2b_mainv001.h"
#include <stdint.h>


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1 // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
 

// Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define frame_per 60//Period (1/FrameRate) in milliseconds


void setup(void)
{
    CLKDIVbits.RCDIV = 0; //Set RCDIV=1:1 (default 2:1) 32mhz or fcy/2=16mhz)
    AD1PCFG = 0x9fff;
    TRISA = 0xFFFE;
    TRISB = 0x0011;
   
}

//bit shifts right 1 bit
void writeColor(int r, int g, int b)
{
    if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
    {
        int shift = 7;
        int count = 8;
        while(count > 0)
        {
            if( (r >> shift) % 2 == 1)
                write_1a();
            else
                write_0a();
            count--;
            shift--;
        }

        shift = 7;
        count = 8;
        while(count > 0)
        {
            if( (g >> shift) % 2 == 1)
                write_1a();
            else
                write_0a();
            count--;
            shift--;
        }

        shift = 7;
        count = 8;
        while(count > 0)
        {
            if( (b >> shift) % 2 == 1)
                write_1a();
            else
                write_0a();
            count--;
            shift--;
        }

        oneMilliSec();
    }
}

void drawFrame(unsigned char frame)
{
    int byteRed = frame;
    int byteBlue = 255 - frame;
    writeColor(byteRed, 0, byteBlue);
}

unsigned long int packColor(unsigned char r, unsigned char g, unsigned char b)
{
    return (((long)r << 16) | ((long)g << 8) | ((long)b));
}

unsigned char getR(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 16));
}

unsigned char getG(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 8));
}

unsigned char getB(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 0));
}

void writePacked(unsigned long int packed)
{
    writeColor(getR(packed), getG(packed), getB(packed));
}

unsigned long int wheel(unsigned char pos)
{
    pos = 255 - pos;
    if(pos < 85)
    {
        return (packColor(255 - pos * 3, 0, pos * 3));
    }
    if(pos < 170)
    {
        pos -=85;
        return (packColor(0, pos * 3, 255 - pos * 3));
    }
    pos -= 170;
    return (packColor(pos * 3, 255 - pos * 3, 0));
}

void delay(int delay)
{
    while(delay > 0)
    {
        oneMilliSec();
        delay--;
    }
}

int main(void) {
    setup();
    while(1)
    {
        int count = 0;
        
        while(count < 255)
        {
            writePacked(wheel(count));
            delay(frame_per);
            count++;
        }

    }
    return 0;
}
