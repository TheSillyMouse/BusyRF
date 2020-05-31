#!/bin/bash

# path to JlinkExe
JLINK_PATH="/home/george/JLink_Linux_V680_x86_64"

echo "Flasing binary..."
$JLINK_PATH/JLinkExe -autoconnect 1 -usb -1 -device nrf52832_xxaa -if SWD -speed 4000 -CommanderScript CommandFile.jlink