#!/bin/bash

PLATFORM="${PLATFORM:-mimasv2}"
TARGET="${TARGET:-base}"
CPU="${CPU:-lm32}"
BRANCH="${BRANCH:-master}"

GITHUB_URL=https://github.com/timvideos/HDMI2USB-firmware-prebuilt/

# Automatically figure out the latest revision
if [ "$REV" = "" ]; then
	REV=$(svn ls $GITHUB_URL/trunk/archive/$BRANCH | sort | tail -n1 | sed -e's-/$--')
	echo "Using revision $REV"
fi

# archive/master/v0.0.3-696-g2f815c1/minispartan6/base/lm32
BASE_DIR=archive/$BRANCH/$REV/$PLATFORM/$TARGET/$CPU

# Get the generated header files
svn export --force $GITHUB_URL/trunk/$BASE_DIR/software/include/generated | grep -v "^Exported"

# Get the flash file
#svn export --force $SVN_URL/flash.bin

# Get gateware and bios files (to allow image building)
#svn export --force $SVN_URL/gateware/top.bin gateware.bin
#svn export --force $SVN_URL/software/bios/bios.bin
