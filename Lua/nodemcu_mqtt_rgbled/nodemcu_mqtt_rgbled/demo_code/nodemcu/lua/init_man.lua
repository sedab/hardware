--[[
init_man.lua
by Patrick Lloyd

Actual init file, but named something other than init.lua in order to 
manually test and debug initialization code.
--]]

-- Load all the global user-defined variables
dofile("config.lua")


-- Initialization routine for RGB LED pins
function rgb_init(freq, duty)
	-- Configure PWM (freq, 50% duty cycle[512/1203])
	pwm.setup(PIN_RED, freq, duty) -- Red
	pwm.setup(PIN_GRN, freq, duty) -- Green
	pwm.setup(PIN_BLU, freq, duty) -- Blue

	-- Start the PWM on those pins (Defaults to white)
	pwm.start(PIN_RED)
	pwm.start(PIN_GRN)
	pwm.start(PIN_BLU)
end 


-- Basic color function used for boot animations.
-- Set LED red, green, blue or white
function rgb_solid(mode)
	if mode == 0 then
		-- RED MODE
		pwm.setduty(PIN_RED, 512)
		pwm.setduty(PIN_GRN, 0)
		pwm.setduty(PIN_BLU, 0)
	elseif mode == 1 then
		-- GREEN MODE
		pwm.setduty(PIN_RED, 0)
		pwm.setduty(PIN_GRN, 512)
		pwm.setduty(PIN_BLU, 0)
	elseif mode == 2 then
		-- BLUE MODE
		pwm.setduty(PIN_RED, 0)
		pwm.setduty(PIN_GRN, 0)
		pwm.setduty(PIN_BLU, 512)
	else
		-- WHITE MODE
		pwm.setduty(PIN_RED, 512)
		pwm.setduty(PIN_GRN, 512)
		pwm.setduty(PIN_BLU, 512)
	end
end


-- LED Initialization
rgb_init(500, 512)


-- Put radio into station mode to connect to network
wifi.setmode(wifi.STATION)


-- Debug info
print('\n\nSTATION Mode:',	'mode='..wifi.getmode())
print('MAC Address: ',		wifi.sta.getmac())
print('Chip ID: ',			node.chipid())
print('Heap Size: ',		node.heap(),'\n')


-- Start the connection attempt
wifi.sta.config(WIFI_SSID, WIFI_PASS)


-- Count how many times you tried to connect to the network
local wifi_counter = 0


-- Create an alarm to poll the wifi.sta.getip() function once a second
-- If the device hasn't connected yet, blink through the LED colors. If it 
-- has, turn the LED white
tmr.alarm(0, 1000, 1, function()
	if wifi.sta.getip() == nil then
		print("Connecting to AP...\n")
		
		-- Rotate through RGB colors while waiting
		wifi_counter = wifi_counter + 1;
		rgb_solid(wifi_counter % 3)
   	else
    	ip, nm, gw = wifi.sta.getip()
      	
    	-- Debug info
      	print("\n\nIP Info: \nIP Address: ",ip)
      	print("Netmask: ",nm)
      	print("Gateway Addr: ",gw,'\n')
      	
      	tmr.stop(0)		-- Stop the polling loop
      	rgb_solid(4)	-- Turn LED white to indicate success
      	
      	-- Continue to main function after network connection
      	dofile("main.lua")
   	end
end)