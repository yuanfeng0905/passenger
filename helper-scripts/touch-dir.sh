#!/bin/sh
#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

# This script touches everything in a directory every 30 minutes to prevent
# /tmp cleaners from removing it.

set -e

dir="$1"
while true; do
	# Sleep for approx 1800 seconds, quit if directory no longer exists.
	i=0
	while test $i -lt 900; do
		sleep 2
		if ! test -e "$dir"; then
			exit
		fi
		i=$((i+1))
	done

	# Touch directory, then try loop again.
	if test -e "$dir"; then
		find "$dir" | xargs touch
	else
		exit
	fi
done
