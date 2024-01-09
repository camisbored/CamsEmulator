# CamsEmulator

<img width="632" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/69677fe4-a5a2-4aed-9b5b-ab9185a72d5f">


This is a emulator for a hypothetical computer architecture, implemented using C and the Windows API. This is in early phases of development. For CE assembly format instructions, look in the instruction-set.txt file. For opcodes for these instructions, you can look in the assembler source code. This is currently a 8 bit architecture, but next phases of this would involve switching to 16 bit.

Currently, this:
1. Loads in a ROM (.ce file) from disk.
2. Can run ROM in two ways: Stepping through and Running through.
3. Can display register values via debug feature at any time.
4. Contains input and output functions, reading keystrokes and drawing pixels to screen.
5. Allows simple audio beeps.
6. Has an assembler to convert CE format .asm files to .ce binaries that can be run.
7. Has a simple compile that will convert a custom language (.cg) files to .asm. See source roms for basic examples.

To do- 
1. Switch to using 16 bit addressing. I started this as 8 bits for simplicities sake, but leaving it as 8 bits has severe implications on memory space and being usable (256 bytes of memory, stack space, code size). Uping this to 16 bit will allow 65k of addressable space in these sections.
2. Restructure and organize c code. Get rid of warnings, move around functions, etc.
4. Enhance compiler.
5. Create games, maybe pong, snake, etc.

To get started, I included some different roms and the source for them in both the src/roms and bin/roms directories. These have examples of drawing art to screen, I/O, audio output, rng.

<img width="220" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/51a11a66-9094-4585-98bd-b1f0d5c6a138">

<img width="217" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/5e8f6992-cc73-4e80-8373-6f3de5c74be2">

<img width="220" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/f9e1a577-a0c9-4a3a-aa25-6b42a2813d20">

<img width="206" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/6e51cc36-cb80-4337-9988-41404ac32599">





