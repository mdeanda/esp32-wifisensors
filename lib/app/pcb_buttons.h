#ifndef PCB_BUTTONS_H
#define PCB_BUTTONS_H

/**
 * pin bindings for custom pcb
 */

#define BUTTON_01 27 // ok
#define BUTTON_02 26 // ok
#define BUTTON_03 25 // touch? -- doesn't work
#define BUTTON_04 33 // touch? -- doesn't work
#define BUTTON_05 14 // ok
#define BUTTON_06 12 // ok
#define BUTTON_07 13 // ok

#define DHT_PIN 32
#define PHOTO_PIN 35
#define MOTION_PIN 34 //not tested yet

// relay group furthest from usb
#define RELAY_1_1 23
#define RELAY_1_2 19
#define RELAY_1_3 18
#define RELAY_1_4 5

// relay group nearest from usb
#define RELAY_2_1 17
#define RELAY_2_2 16
#define RELAY_2_3 4
#define RELAY_2_4 15

#define I2C_SCL 22
#define I2C_SDA 21 //mislabeled as "sca"

#endif
