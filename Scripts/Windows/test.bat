:: @echo on

:://///////////////////////// 
:: main entrance 
::goto OPEN_TELNET
::goto TEST_CHOICE_CMD
::goto OPEN_GRASSSOFT
::goto REBOOT_TEST
goto shutdown_pc
:://///////////////////////// 


:: 选择命令测试
:TEST_CHOICE_CMD
choice /c YN  /M yes,no
choice /c:dme defrag,mem,EDN
@goto EDN

::复制命令
:TEST_COPY_CMD
@Rem copy c:\devlist.txt d:\
@goto EDN

::IF条件选择，命令用法
::IF [NOT] ERRORLEVEL number command
::IF [NOT] string1==string2 command
::IF [NOT] EXIST filename command
:TEST_IF_CMD
@set /p var=请输入变量值：
@echo var=%var%
@IF "%var%"=="a" (
	@echo "var=%var%"
	@echo here) ELSE (
	@echo else)
@goto EDN


:OPEN_GRASSSOFT
start /D "C:\Program Files (x86)\丝茅草软件\自动操作魔法师\" MacroPlayer.exe "C:\ProgramData\Grasssoft\Macro Expert\Data\示例\test1.msd"
@goto EDN
:: 运行一个程序或者命令  /D 后面跟路径，如下运行C盘下面的高通PDC程序
:: start /D "C:\Program Files (x86)\Qualcomm\QPST\bin\" PDC.exe


::-----------示例，自动打开telent
:OPEN_TELNET
IF exist test.vbs del test.vbs
@echo dim WshShell>>test.vbs
@echo Set WshShell=WScript.CreateObject("WScript.Shell")>>test.vbs
@echo WshShell.Run "cmd">>test.vbs
@echo WshShell.AppActivate "test" >>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys "telnet 192.168.1.1">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"root">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"3F9U8D2B3L">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"cd /">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"ls">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"exit">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

@echo WshShell.SendKeys"exit">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 400>>test.vbs

call test.vbs
goto EDN

:REBOOT_TEST
IF exist test.vbs del test.vbs
@echo dim WshShell>>test.vbs
@echo Set WshShell=WScript.CreateObject("WScript.Shell")>>test.vbs
@echo WshShell.Run "cmd">>test.vbs
@echo WshShell.AppActivate "test" >>test.vbs
@echo WScript.Sleep 1000>>test.vbs

@echo WshShell.SendKeys "adb shell reboot">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs
@echo WScript.Sleep 120000>>test.vbs

@echo WshShell.SendKeys "exit">>test.vbs
@echo WScript.Sleep 1000>>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs

call test.vbs
goto REBOOT_TEST


:shutdown_pc
IF exist test.vbs del test.vbs
@echo dim WshShell>>test.vbs
@echo Set WshShell=WScript.CreateObject("WScript.Shell")>>test.vbs
@echo WshShell.Run "cmd">>test.vbs
@echo WshShell.AppActivate "test" >>test.vbs
@echo WScript.Sleep 3600000>>test.vbs

@echo WshShell.SendKeys "shutdown /h">>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs

@echo WshShell.SendKeys "exit">>test.vbs
@echo WScript.Sleep 1000>>test.vbs
@echo WshShell.SendKeys"{ENTER}">>test.vbs

call test.vbs



:EDN
pause
