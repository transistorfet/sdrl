#
# Name:		Makefile
# Version:	0.1
# Description:	Builder Utilities Makefile
#


update:
	cvs export -D `date --iso-8601=seconds` -d /tmp/builder builder
	mv /tmp/builder/* .
	rmdir /tmp/builder

