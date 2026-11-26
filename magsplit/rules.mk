# Split keyboard settings
SPLIT_KEYBOARD = yes        # Enable split keyboard support

# Controller and bootloader
MCU = atmega32u4
BOOTLOADER = caterina

#Console Enable
CONSOLE_ENABLE = yes

#Tap Dancing
TAP_DANCE_ENABLE = yes

# Module link and OLED
UART_DRIVER_REQUIRED = yes
OLED_ENABLE = yes
SRC += module_link.c module_oled.c