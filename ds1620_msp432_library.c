#include <ds1620_msp432_library.h>
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

volatile uint32_t i;
unsigned char temp;

void blink () {

    P2-> DIR |= BIT2; //set as OUT

    P2-> OUT ^= BIT2;
}

//*******************************************************************************************************************************************
extern void start () {
    writeSetupCommand ();

    readDateFromSensor ();
}
//*******************************************************************************************************************************************
extern void write_command (unsigned short command) {

    unsigned short n, bit;

    for (n = 0; n < 8; n++) {
        bit = ((command >> n) & (0x01));
        out_bit (bit);
    }

}

extern void out_bit (unsigned short bit) {
    volatile uint32_t i;
//    GPIO_write(Board_GPIO_DQ, bit);
    if (bit == 1) {
        P2->OUT |= BIT6;
    } else {
        P2->OUT &= ~BIT6;
    }

//    sendMessage(bit + '0');
    P2->OUT &= ~BIT7;                                                               //CLK -> 0
//    for (i = 150; i > 0; i--);
    P2->OUT |= BIT7;                                                                //CLK -> 1

}
//*******************************************************************************************************************************************
extern void writeSetupCommand () {
    volatile uint32_t i;
    P2->OUT &= ~BIT4;                                                               //RST -> 0
    P2->OUT |= BIT7;                                                                //CLK -> 1
    P2->OUT |= BIT4;                                                                //RST -> 1

    write_command (0x0C);                                                           // write config command
    write_command (0x02);                                                           // cpu mode

    P2->OUT &= ~BIT4;                                                               //RST -> 0

    P2->OUT |= BIT7;                                                                //CLK -> 1
    P2->OUT |= BIT4;                                                                //RST -> 1

    write_command (0xEE);                                                           //start conversion

    P2->OUT &= ~BIT4;                                                               //RST -> 0

    P2->OUT |= BIT7;                                                                //CLK -> 1
    P2->OUT |= BIT4;                                                                //RST -> 1

    write_command (0xAA);                                                           //read tempe rature
}
//*******************************************************************************************************************************************
extern void readDateFromSensor () {
    volatile uint32_t i;

    uint8_t n,bit;
    uint8_t raw_data = 0;

    for (n = 0; n < 8; n++) {
        P2->OUT &= ~BIT7;                                                           //CLK -> 0

        P2->DIR &= ~BIT6;                                                           //set to input
        if (P2->IN & BIT6) {
            bit |= BIT0;
        } else {
            bit &= ~BIT0;
        }

        P2->OUT |= BIT7;                                                            //CLK -> 1;

        raw_data = raw_data | (bit << n);
    }

    P2->DIR |= BIT6;                                                                //set to output

    P2->OUT &= ~BIT4;                                                               //RST -> 0

    temp = (raw_data/2);

}
//*******************************************************************************************************************************************
extern unsigned char getData () {
    return temp;
}
