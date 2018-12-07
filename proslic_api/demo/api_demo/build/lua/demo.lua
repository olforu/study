--[[-- 
--Example Lua Script 
-- $Id: demo.lua 5719 2016-06-17 22:04:43Z nizajerk $
--
-- Copyright 2012-2016, Silicon Laboratories, Inc.
--
--]] --

-- Print out a simple banner showing what environment we're running... -------
function simple_banner()
  print("ProSLIC API version = " .. PROSLIC_VERSION)
  print("Number of channels = " .. PROSLIC_NUM_CHAN)
end 

-- Print out some registers of interest.. -------------------------------------
function simple_reg_dump(channel)
    print( "Reg read of 0 on channel " .. channel .. " = " .. string.format( '%x', SiVoice_ReadReg(channel,0)))
    print( "Reg read of 3 on channel " .. channel .. " = " .. string.format( '%x', SiVoice_ReadReg(channel,3)))
    print( "Reg read of 12 on channel " .. channel .. " = " .. string.format( '%x', SiVoice_ReadReg(channel,12)))
    print( "Reg read of 14 on channel " .. channel .. " = " .. string.format( '%x', SiVoice_ReadReg(channel,14)))
    print( "RAM read of 1538 on channel " .. channel .. " = " .. string.format( '%x', ProSLIC_ReadRAM(channel,1538)))
end

-- Ring a channel with preset 1 for 12 seconds. -------------------------------
function ring_for12sec(channel)
  print( "Ringing phone on channel " .. channel )
  ProSLIC_RingSetup(channel,1)
  ProSLIC_RingStart(channel)
  si_sleep(12)
  ProSLIC_RingStop(channel)
  print( "Ring stop on channel " .. channel )
end

-- Play a tone for N seconds --------------------------------------------------
function playTone(channel, tone, time)
  print( "About to play tone " .. tone .. " on channel " .. channel )
  --Set the line feed state to forward active (state = 1)
  ProSLIC_SetLinefeedStatus(channel,1)
  ProSLIC_ToneGenSetup(channel, tone) 
  ProSLIC_ToneGenStart(channel, true) -- ASSUME: timers need to be enabled...
  si_sleep(time)
  ProSLIC_ToneGenStop(channel)
end

------------------------- main() ----------------------------------------------
simple_banner()

for i = 0, PROSLIC_NUM_CHAN-1 do
  simple_reg_dump(i)
end

-- Uncomment out the lines to ring the port 
-- ring_for12sec(0)

playTone(0, 2, 10)

-- Set the Line feed state to open (0)
for i = 0, PROSLIC_NUM_CHAN-1 do
  print( "Placing channel " .. i .. " to open")
  ProSLIC_SetLinefeedStatus(i,0)
end

