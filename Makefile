ARDUINO_DIR  = ../arduino-1.6.13
USER_LIB_PATH = ../ardulibs
ARDUINO_LIBS = OneWire-2.3.2 Arduino-Temperature-Control-Library-3.7.6
BOARD_TAG    = nano
BOARD_SUB    = atmega328
MONITOR_PORT = /dev/ttyUSB1
include ../Arduino-Makefile-1.5.1/Arduino.mk
