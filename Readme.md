# MagSplit
> A wired split keyboard with ATmega32u4 promicro microcontrollers, oled displays, and magnetic connectors for module attachment

## Overview
MagSplit is an extension of my Beniaris46 keyboard featuring an extra column for each hand, much like a Corne board, and magnetic connectors for attaching "modules".  The has an ortholinear columns staggered layout. Modules connect to the main keyboard via the magnetic connector on the side of the left split of the keyboard.  The magnetic connector handles ESD for the exposed pins as well as load switching for delivering power to the modules when snapped into place.  The modules themselves communicate over UART to the main half of the keyboard, notifying it of a successful connection. Modules still retain their functionality from usb and can be used independently.

## WODULE POWER WARNING
THERE IS NO BACK FEED PROTECTION FOR THE 5V RAIL.  You can plug in the usb and connect the module with the magnetic connector.  In an effort to keep complexity low and cost down, I did not include this.  The simple rule is, "One or the other", when it comes to the usb and magnetic connector.

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