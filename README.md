# GP47
GamePad47 - terminate process using the gamepad

Written for the Emulator Station (but not exclusively) 
for termintaion of external emulators using only the GamePad

The program uses window titles (partial serach) for finding process to kill

config.ini file will be generated in the same directory if it does not exist.

Add new program to the termination list: (in config.ini file)

PROCESS3 = xemu # will search for xemu in all windows titles and terminate it

This project borowed heavily from: https://github.com/Tylemagne/Gopher360
