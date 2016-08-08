-- file : config.lua
local module = {}

module.SSID = {}  
module.SSID["Barn"] = "16Chickens"

module.HOST = "72.227.147.224"  
module.PORT = 1883  
module.ID = node.chipid()

module.ENDPOINT = "nodemcu/"  
return module  