mov r1, 115
str r1, 0
mov r1, 115
str r1, 1
mov r1, 0
str r1, 2
start:
getch
push r5
pop r1
str r1, 2
ldr r1, 2
push r1
mov r1, 87
cmpStackTop r1
je label0
je label0
jne label0done
label0:
ldr r1, 1
push r1
mov r1, 1
subStackTop r1
pop r1
str r1, 1
jmp label0done
label0done:
ldr r1, 2
push r1
mov r1, 65
cmpStackTop r1
je label2
je label2
jne label2done
label2:
ldr r1, 0
push r1
mov r1, 1
subStackTop r1
pop r1
str r1, 0
jmp label2done
label2done:
ldr r1, 2
push r1
mov r1, 83
cmpStackTop r1
je label4
je label4
jne label4done
label4:
ldr r1, 1
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 1
jmp label4done
label4done:
ldr r1, 2
push r1
mov r1, 68
cmpStackTop r1
je label6
je label6
jne label6done
label6:
ldr r1, 0
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 0
jmp label6done
label6done:
ldr r1, 1
push r1
pop r2
ldr r1, 0
mov r3, 0
mov r4, 0
mov r5, 0
setPixelRedraw
jmp start
