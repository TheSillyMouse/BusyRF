## **How to flash binary file (*.bin) into the chip.**


The command to run the debugger is:  
`/home/george/JLink_Linux_V680_x86_64/JLinkExe -autoconnect 1 -usb -1 -device nrf52832_xxaa -if SWD -speed 4000 -CommanderScript flash_script/CommandFile.jlink`

The `CommandFile.jlink` contains the commands that JLinkExe will execute.
Modify `CommandFile.jlink` if you need to change the name of the binary of its path.