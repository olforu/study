--[[-- 
--Example Lua Script 
-- $Id: wrv.lua 5719 2016-06-17 22:04:43Z nizajerk $
--
-- Copyright 2016, Silicon Laboratories, Inc.
--
--]] --

-- Simple write/verify function...
function wrv(channel, register)
  for i = 0, 255 do

    SiVoice_WriteReg(channel, register, i)
    new_value = SiVoice_ReadReg(channel, register)

    if( new_value ~= i) then
      print("Failed to write to channel: " .. channel .. " value of " .. i)
      return -1
    end 
  end

  return 0
end

------------------------- main() ----------------------------------------------
for channel = 0, PROSLIC_NUM_CHAN-1 do
  if (wrv(channel, 12) == 0) then
    print("Channel " .. channel .. " Passed")
  end
end

