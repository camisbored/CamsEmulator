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

To do- 
1. Switch to using 16 bit addressing. I started this as 8 bits for simplicities sake, but leaving it as 8 bits has severe implications on memory space and being usable (256 bytes of memory, stack space, code size). Uping this to 16 bit will allow 65k of addressable space in these sections.
2. Restructure and organize c code. Get rid of warnings, move around functions, etc.
4. Create compiler.
5. Create games, maybe pong, snake, etc.

To get started, I included some different roms and the source for them in both the src/roms and bin/roms directories. These have examples of drawing art to screen, I/O, audio output, rng.

<img width="211" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/b364409c-9286-432d-8ef8-796c5f8cf99e">



<img width="553" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/4652c86a-c3ef-4dbd-8024-9f526845aa9e">



<img width="209" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/eb6c467b-d00f-4dc4-b662-2d47eaf7ded4">



<img width="215" alt="image" src="https://github.com/camisbored/CamsEmulator/assets/81730723/d9a6dae6-dca4-4ebd-a377-6f27b319d2d9">



