#
# Regular cron jobs for the developer-debug-display package
#
0 4	* * *	root	[ -x /usr/bin/developer-debug-display_maintenance ] && /usr/bin/developer-debug-display_maintenance
