# Door-Opener
This Readme is written in May 2024.
## Story behind this project
NTURacing have a garage and workshop where we hold meetings and manufacture stuff. The garage has a regular sized door and a big rolling door where big cargo and the car can go in and out. Because the switch of the garage door is inside the garage itself, the only canaical way to enter the garage when everything is locked is through the regular sized door.

In the prehistorical times(probably before 2021), a pioneer made a remote garage door opener attached on the rolling door electrically that can let people open the rolling door with an IR remote and some password. After that, people without keys can still enter the garage with the remote gate opener.

Unfortunately, in summer 2023, the OG door opener module made out of an Aruduino UNO broke for some reason, and comes the dark ages when people can only enter the garage the canonical way. What's worse, the sacred texts that is the source code of the OG garage door opener was lost in time, so there was no way anybody could restore the OG garage door opener back to its original glory.

This project then serves as the successor of our beloved Arduino door lock. It consists of a custom PCB, an STM32, and some firmware that runs on the STM32.
## How to build this project
This project is built with STM32CubeMX and GNU make on a Linux environment. The project structure should be identical to all other STM32Cube projects out there, but the underlying tools used are GNU make.
The tools required for this project:
1. GNU Make
2. arm-none-eabi-gcc
3. STM32CubeMX
4. stlink-utility, or anything else that can flash a .bin or .elf file onto your stm32.

To build and flash this project:
1. Use STM32CubeMX to open the ioc file in the root of the project
2. Press Generate Code to generate the necessary HAL drivers and library for this project
3. fire up your terminal
4. run `make` at the root directory.
5. if you have stlink-utility command line tool installed on your machine, you can simply plug your STM32 board on your PC and run `make install` to flash the program onto it.
6. If you don't have stlink-utility, you can use any other tools such as STM32CubeProgrammer to flash the elf onto the board. the elf and bin is supposed to be the openator.elf and openator.bin in the build/ directory.

Note: in step 4, 2 variables need to be set to build the complete functionality of the gate opener. They are:
* USE_BUZZ: in testing, the relay for the door and the buzzer was not enabled since they require an external 9V power source. Build this code with `USE_BUZZ=1` to enable the buzzer and leave this variable undefined to disable the buzzer.
* GATE_PASSWORD: set the password for the door with this variable. For example, `GATE_PASSWORD=114514`. The password need to be a 6 digit code made up of numbers and these characters: "udlro*". The special code coorispond to direction buttons, the OK button and the * button. If no password is specified, the default is 123456.
If you screwed up a build, run `make clean` to remove the previously built binary.
Example command: `make -j16 USE_BUZZ=1 GATE_PASSWORD=037343`
