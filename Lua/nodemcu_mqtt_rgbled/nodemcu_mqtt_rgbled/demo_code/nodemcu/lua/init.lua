--[[
init.lua
by Patrick Lloyd

NodeMCU automatically runs init.lua on bootup so this acts as a launcher file
meant to load the actual init file only when everything has been tested and
debugged. 
--]]
dofile("init_man.lua")