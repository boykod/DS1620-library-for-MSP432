#include <_ti_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DS1620_MSP432_LIBRARY_H_
#define DS1620_MSP432_LIBRARY_H_

extern void blink (void);

extern void write_command (unsigned short command);
extern void out_bit (unsigned short bit);
extern void start ();
extern void writeSetupCommand ();
extern void readDateFromSensor ();
extern unsigned char getData ();

#endif /* LIB_H_ */
