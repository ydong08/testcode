#!/bin/sh
flag_file="/tmp/yang_load_flag"

wait_pre_action_done()
{
	fstr=$1
	while :
	do
		if [ ! -e $flag_file ];then
			sleep 5
			continue
		fi

		content=$(cat $flag_file)
		if [ "x$content" = "x$fstr" ];then
			break
		fi
	done
}

wait_pre_action_done "setup"
echo -n "setup done"

wait_pre_action_done "mergehostkey"
echo -n "mergehostkey done"


