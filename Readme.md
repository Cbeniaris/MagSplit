# MagSplit
A wired split keyboard with ATmega32u4 promicro microcontrollers, oled displays, and magnetic connectors for module attachment

## Overview
MagSplit is an extension of my Beniaris46 keyboard featuring an extra column for each hand, much like a Corne board, and magnetic connectors for attaching "modules".  I made a few macropads that supported qmk and got frustrated when each one needed a new cable on my desk.   I wanted a modular system that allow attaching "modules" with different configuations for different workflows. Modules connect to the main keyboard via the magnetic connector on the side of the left split of the keyboard.  The magnetic connector handles ESD for the exposed pins as well as load switching for power delivery.  The modules themselves communicate over UART to the main half of the keyboard, notifying it of a successful connection, and relaying keypresses through to the host. Since each module is still a qmk board with the same ATmega32u4 micros, they still function as standalone HID boards. They retain their layers, macros, etc. when swapping hosts.  This allows for quickly snapping a new module on without needing to fetch a cable. 

## MODULE POWER WARNING
THERE IS NO BACK FEED PROTECTION FOR THE 5V RAIL.  You can plug in the usb and connect the module with the magnetic connector.  In an effort to keep complexity low and cost down, I did not include this.  The simple rule is: "One or the other". DO NOT attach both the magnetic connector to the main half of the keyboard AND the module to usb-c power at the same time. I had initially planned to use Schottky diodes to prevent this but worried about voltage drop for modules.

## Materials
| Component | Part Number |
| --- | --- |
| Micro | Any ATmega32ur pro micro |
| 3 pin TRS Jack | HC-PJ-320A-3P-D |
| Load Switch | TPS2552DBV (SOT-23-6, not WSON)|
| ESD protection | USBLC6-2SC6 |
| Switches | Any Cherry switches |
| Matrix Diodes | 1N4148W |
| Caps / Resistors | 0805 smd variants |
| Magnetic connectors | 2.54mm pitch from AliExpress|
| Oled Socket | 2.54mm pitch female socket|
| Oled Modules | Hosyond 0.96" I2C Oled Display |