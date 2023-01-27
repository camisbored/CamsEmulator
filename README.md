# CamsEmulator


This is a emulator for a hypothetical computer architecture, implemented using C and the Windows API. This is in early phases of development. 

Currently, this:
1. Loads in a ROM (.ce file) from disk.
2. Can run ROM in two ways: Stepping through and Running through.
3. Can display register values via debug feature at any time.
4. Contains input and output functions, reading keystrokes and drawing pixels to screen.

To do- 
1. Clean up, add, and organize opcodes- they are a mess right now as a result of me validating functionality.
2. Structure source code more efficently, it is one jumbled up file for now.
