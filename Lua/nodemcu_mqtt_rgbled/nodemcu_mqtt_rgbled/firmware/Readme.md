# NodeMCU Firmware

These two firmware files are pre-compiled NodeMCU binaries for use on the
ESP8266 wireless SoC. Use esptool to upload one or the other. Either should
work.

python <path/to/esptool/>esptool.py write_flash 0x00000 <firmware_name>.bin
