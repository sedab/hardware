#!/bin/bash

echo "*************************************************************************************************"
echo "* 0. Generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin for 512KB Flash, no need boot"
echo "* 1. Generate user1 bin for  512KB Flash, support boot1.1 and boot1.2"
echo "* 2. Generate user2 bin for  512KB Flash, support boot1.1 and boot1.2"
echo "* 3. Generate user1 bin for 1024KB Flash, support boot1.1 and boot1.2"
echo "* 4. Generate user2 bin for 1024KB Flash, support boot1.1 and boot1.2"
echo "* 5. Generate user1 bin for  512KB Flash, support only boot1.2 (smaller user bin)"
echo "* 6. Generate user2 bin for  512KB Flash, support only boot1.2 (smaller user bin)"
echo "* 7. Generate user1 bin for 1024KB Flash, support only boot1.2 (smaller user bin)"
echo "* 8. Generate user2 bin for 1024KB Flash, support only boot1.2 (smaller user bin)"
echo "*************************************************************************************************"
echo "Please enter index(0-8):"
read index

touch user/user_main.c

if [ -z "$index" ]; then
	make COMPILE=gcc
	if [ $? == 0 ];then
	    pass=1
	fi
elif [ $index == 0 ]; then
	make COMPILE=gcc
	if [ $? == 0 ];then
	    pass=1
	fi
elif [ $index == 1 ]; then
	app=1
	flash=512
	boot=old
	addr=0x1000
elif [ $index == 2 ]; then
	app=2
	flash=512
	boot=old
	addr=0x41000
elif [ $index == 3 ]; then
	app=1
	flash=1024
	boot=old
	addr=0x1000
elif [ $index == 4 ]; then
	app=2
	flash=1024
	boot=old
	addr=0x81000
elif [ $index == 5 ]; then
	app=1
	flash=512
	boot=new
	addr=0x1000
elif [ $index == 6 ]; then
	app=2
	flash=512
	boot=new
	addr=0x41000
elif [ $index == 7 ]; then
	app=1
	flash=1024
	boot=new
	addr=0x1000
elif [ $index == 8 ]; then
	app=2
	flash=1024
	boot=new
	addr=0x81000
else
	make COMPILE=gcc
	if [ $? == 0 ];then
	    pass=1
	fi
fi

if [ "$app" ]; then
	make COMPILE=gcc BOOT=$boot APP=$app FLASH=$flash
	if [ $? == 0 ];then
	    pass=1
	fi
fi

if [ "$pass" ];then
    echo ""
    echo "!!!"
    if [ "$app" ];then
    	echo "Generate user$app.$flash.$boot.bin successully in folder bin/upgrade."
    	if [ $boot == new ];then
    		echo "Support boot_v1.2 and +"
    	elif [ $boot == old ];then
    		echo "Support boot_v1.1 and +"
    	fi
    	echo "user$app.$flash.$boot.bin------>addr:$addr"
    else
    	echo "Generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin successully in folder bin."
    	echo "eagle.app.v6.flash.bin---------->addr:0x00000"
    	echo "eagle.app.v6.irom0text.bin------>addr:0x40000"
    fi
    echo "!!!"
fi
