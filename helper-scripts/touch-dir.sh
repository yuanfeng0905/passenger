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
	sleep 1800
	if test -e "$dir"; then
		find "$dir" | xargs touch
	else
		break
	fi
done
