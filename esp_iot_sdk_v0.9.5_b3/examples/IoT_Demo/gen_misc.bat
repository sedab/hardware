@echo off

echo 0. Generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin for 512KB Flash, no need boot
echo 1. Generate user1 bin for  512KB Flash, support boot1.1 and boot1.2
echo 2. Generate user2 bin for  512KB Flash, support boot1.1 and boot1.2
echo 3. Generate user1 bin for 1024KB Flash, support boot1.1 and boot1.2
echo 4. Generate user2 bin for 1024KB Flash, support boot1.1 and boot1.2
echo 5. Generate user1 bin for  512KB Flash, support only boot1.2 (smaller user bin)
echo 6. Generate user2 bin for  512KB Flash, support only boot1.2 (smaller user bin)
echo 7. Generate user1 bin for 1024KB Flash, support only boot1.2 (smaller user bin)
echo 8. Generate user2 bin for 1024KB Flash, support only boot1.2 (smaller user bin)

echo Please enter index(0-8):
set /p index=

touch user/user_main.c

set app=0

IF %index% EQU 0 (
	make
) ELSE (
IF %index% EQU 1 (
	set app=1
	set flash=512
	set boot=old
	set addr=0x1000
)  ELSE (
IF %index% EQU 2 (
	set app=2
	set flash=512
	set boot=old
	set addr=0x41000
)  ELSE (
IF %index% EQU 3 (
	set app=1
	set flash=1024
	set boot=old
	set addr=0x1000
)  ELSE (
IF %index% EQU 4 (
	set app=2
	set flash=1024
	set boot=old
	set addr=0x81000
)  ELSE (
IF %index% EQU 5 (
	set app=1
	set flash=512
	set boot=new
	set addr=0x1000
)  ELSE (
IF %index% EQU 6 (
	set app=2
	set flash=512
	set boot=new
	set addr=0x41000
)  ELSE (
IF %index% EQU 7 (
	set app=1
	set flash=1024
	set boot=new
	set addr=0x1000
)  ELSE (
IF %index% EQU 8 (
	set app=2
	set flash=1024
	set boot=new
	set addr=0x81000
) ELSE (
	make
)))))))))

IF %index% NEQ 0 (
make BOOT=%boot% APP=%app% FLASH=%flash%
)

echo !!!
IF %index% NEQ 0 (
	echo "Generate user%app%.%flash%.%boot%.bin successully in folder bin/upgrade."
	IF %boot% == new (
		echo "Support boot_v1.2 and +"
	) ELSE (
		IF %boot% == old (
			echo "Support boot_v1.1 and +"
		)
	)
	echo "user%app%.%flash%.%boot%.bin------>addr:%addr%"
) ELSE (
	echo "Generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin successully in folder bin."
	echo "eagle.app.v6.flash.bin---------->addr:0x00000"
	echo "eagle.app.v6.irom0text.bin------>addr:0x40000"
)

echo !!!