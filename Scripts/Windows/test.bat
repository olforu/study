:: @echo on

:://///////////////////////// 
:: main entrance 
::goto OPEN_TELNET
::goto TEST_CHOICE_CMD
::goto OPEN_GRASSSOFT
::goto REBOOT_TEST
goto shutdown_pc
:://///////////////////////// 


:: ѡ���������
:TEST_CHOICE_CMD
choice /c YN  /M yes,no
choice /c:dme defrag,mem,EDN
@goto EDN

::��������
:TEST_COPY_CMD
@Rem copy c:\devlist.txt d:\
@goto EDN

::IF����ѡ�������÷�
::IF [NOT] ERRORLEVEL number command
::IF [NOT] string1==string2 command
::IF [NOT] EXIST filename command
:TEST_IF_CMD
@set /p var=���������ֵ��
@echo var=%var%
@IF "%var%"=="a" (
	@echo "var=%var%"
	@echo here) ELSE (
	@echo else)
@goto EDN


:OPEN_GRASSSOFT
start /D "C:\Program Files (x86)\˿é�����\�Զ�����ħ��ʦ\" MacroPlayer.exe "C:\ProgramData\Grasssoft\Macro Expert\Data\ʾ��\test1.msd"
@goto EDN
:: ����һ�������������  /D �����·������������C������ĸ�ͨPDC����
:: start /D "C:\Program Files (x86)\Qualcomm\QPST\bin\" PDC.exe


::-----------ʾ�����Զ���telent
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
