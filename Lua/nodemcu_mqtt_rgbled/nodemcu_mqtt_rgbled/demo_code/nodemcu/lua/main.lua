--[[
main.lua
by Patrick Lloyd

This contains all the bits and pieces to connect to the broker, subscribe and
and publish to topics, run various functions that interact with GPIO, and so
on.
--]]

-- Holds dispatching keys to different topics. Serves as a makeshift callback
-- function dispatcher based on topic and message content
m_dis = {}


-- Standard counter variable. Used for modulo aritmatic
local count = 0

function animate(m, pl)
	-- Confirm that an animation message was recieved on the /mcu/cmd topic
	m:publish("/mcu/rgbled_status/", "--> ANIMATE COMMAND", 0, 0,
			function(m) print("ANIMATE COMMAND") end)
	
	-- Main option control structure. Pretty gross-looking but it works
	-- Option 0 turns everything off
	if pl == "0" then
		-- Confirm LED being turned off to serial terminal and MQTT broker
		m:publish("/mcu/rgbled_status/", "--> LED OFF", 0, 0,
			function(m) print("LED OFF") end)
		
		-- Reset the counter and stop the timer from another function
		count = 0
		tmr.stop(1)
		
		-- PWM process is still running but duty cycle is just set to zero
		pwm.setduty(PIN_RED, 0)
		pwm.setduty(PIN_GRN, 0)
		pwm.setduty(PIN_BLU, 0)
	
	-- RBG Mode
	elseif pl == "1" then
		-- Confirm LED in RGB mode to serial terminal and MQTT broker
		m:publish("/mcu/rgbled_status/", "--> R-G-B Mode", 0, 0,
			function(m) print("RGB Mode") end)
		
		-- Just stop the timer from another loop
		tmr.stop(1)
		
		-- Use function declared in init_man.lua to blink through red, green,
		-- and blue
		tmr.alarm(1,500,1,function()
			count = count + 1
			rgb_solid(count % 3)
		end)
	
	-- Pick a "random" color and make it breathe mode
	elseif pl == "2" then
		-- Confirm LED in random breathe mode to serial terminal and MQTT broker
		m:publish("/mcu/rgbled_status/", "--> Random-Breathe Mode", 0, 0,
			function(m) print("Random-Breathe Mode") end)
		
		-- Reset the counter and stop the timer from another function
		tmr.stop(1)
		count = 0
		
		-- Create variables run the breather alarm. Start with random color at
		-- full brightness (percent = 100)
		local percent = 100
		local count_up  = false
		local red = (tmr.now()*tmr.now()) % 512
		tmr.delay(red)
		local grn = (tmr.now()*tmr.now()) % 512
		tmr.delay(grn)
		local blu = (tmr.now()*tmr.now()) % 512

		-- Breather alarm function run every 20 ms
		tmr.alarm(1,20,1,function()
			-- Set the LED brightness
			pwm.setduty(PIN_RED, red * percent / 100)
			pwm.setduty(PIN_BLU, blu * percent / 100)
			pwm.setduty(PIN_GRN, grn * percent / 100)
			
			-- Logic to either dim or brighten
			if count_up == false then
				percent = percent - 1
				if percent < 0 then
					percent = 0
					count_up = true
				end
			else
				percent = percent + 1
				if percent > 100 then
					percent = 100
					count_up = false
				end
			end
		end)
	
	-- Lots of random blinking craziness
	elseif pl == "3" then
		-- Confirm LED in disco mode to serial terminal and MQTT broker
		m:publish("/mcu/rgbled_status/", "--> Disco Mode", 0, 0,
			function(m) print("Disco Mode") end)
		
		-- Reset the counter and stop the timer from another function
		tmr.stop(1)
		count = 0
		
		-- Crazy disco alarm every 20 ms
		tmr.alarm(1,20,1,function()
			pwm.setduty(PIN_GRN, (tmr.now()*tmr.now())%512)
			pwm.setduty(PIN_RED, (tmr.now()*tmr.now())%512)
			pwm.setduty(PIN_BLU, (tmr.now()*tmr.now())%512)
		end)
	
	-- Something went wrong somehow
	else
		-- Print and publish to serial terminal and MQTT broker respectively
		-- that something went wrong
		m:publish("/mcu/rgbled_status/", "--> Error: Unknown Command", 0, 0,
			function(m) print("ERROR: UNKNOWN COMMAND") end)
	end
end


-- As part of the dispatcher algorithm, this assigns a topic name as a key or
-- index to a particular function name
m_dis["/mcu/cmd/animate"] = animate

-- initialize mqtt client with keepalive timer of 60sec
m = mqtt.Client(MQTT_CLIENTID, 60, "", "") -- Living dangerously. No password!


-- Set up Last Will and Testament (optional)
-- Broker will publish a message with qos = 0, retain = 0, data = "offline"
-- to topic "/lwt" if client don't send keepalive packet
m:lwt("/lwt", "Oh noes! Plz! I don't wanna die!", 0, 0)


-- When client connects, print status message and subscribe to cmd topic
m:on("connect", function(m) 
	-- Serial status message
	print ("\n\n", MQTT_CLIENTID, " connected to MQTT host ", MQTT_HOST,
		" on port ", MQTT_PORT, "\n\n")

	-- Subscribe to the topic where the ESP8266 will get commands from
	m:subscribe("/mcu/cmd/#", 0,
		function(m) print("Subscribed to CMD Topic") end)
end)


-- When client disconnects, print a message and list space left on stack
m:on("offline", function(m)
	print ("\n\nDisconnected from broker")
	print("Heap: ", node.heap())
end)


-- On a publish message receive event, run the message dispatcher and
-- interpret the command
m:on("message", function(m,t,pl)
	print("PAYLOAD: ", pl)
	print("TOPIC: ", t)
	
	-- This is like client.message_callback_add() in the Paho python client.
	-- It allows different functions to be run based on the message topic
	if pl~=nil and m_dis[t] then
		m_dis[t](m,pl)
	end
end)


-- Connect to the broker
m:connect(MQTT_HOST, MQTT_PORT, 0, 1)