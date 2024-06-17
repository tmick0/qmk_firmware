CUSTOM_MATRIX = lite
VPATH += drivers/gpio
SRC += pca9555.c matrix.c i2c_master.c
I2C_DRIVER_REQUIRED = yes
CONSOLE_ENABLE = yes