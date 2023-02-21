#!/bin/sh

export HOME=/root

/usr/bin/tput civis

/usr/bin/emulationstation.bin
RET=$?

case "$RET" in
  0)
	DEPTH2=`/usr/sbin/fbset | head -3 | tail -1 | cut -d " " -f 10`
	if [ "$DEPTH2" == "8" ]; then
		DEPTH1=16
	elif [ "$DEPTH2" == "16" ] || [ "$DEPTH2" == "32" ]; then
		DEPTH1=8
	else
		DEPTH1=8
		DEPTH2=16
	fi
	/usr/sbin/fbset -depth $DEPTH1 > /dev/null 2>&1
	/usr/sbin/fbset -depth $DEPTH2 > /dev/null 2>&1
	/usr/bin/sleep 2
	;;
  1)
	/usr/sbin/poweroff
	;;
  2)
	/usr/sbin/reboot
	;;
  3)
	/usr/bin/tput cnorm
	export PS1='\u@\h:\w# '
	cd $HOME
	echo
	exec /bin/sh --login
	;;
esac

exit 0
