--[[
config.lua
by Patrick Lloyd

Global variable configuration file for better portability
Change for your particular setup. This assumes default Mosquitto config
--]]

-- Pin Declarations
PIN_RED = 1
PIN_GRN = 2
PIN_BLU = 3
PIN_BTN = 4

-- WiFi
WIFI_SSID = ""
WIFI_PASS = ""

-- MQTT
MQTT_CLIENTID = "esp-blinkenlite"
MQTT_HOST = ""
MQTT_PORT = 1883

-- Confirmation message
print("\nGlobal variables loaded...\n")