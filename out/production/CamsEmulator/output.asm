mov r1, 115
str r1, 0
mov r1, 115
str r1, 1
mov r1, 0
str r1, 2
ldr r1, 1
push r1
pop r2
ldr r1, 0
mov r3, 255
mov r4, 0
mov r5, 0
setPixelRedraw
start:
getch
push r5
pop r1
str r1, 2
ldr r1, 2
push r1
mov r1, 87
cmpStackTop r1
jne label1
je label0
label0:
ldr r1, 1
push r1
mov r1, 1
subStackTop r1
pop r1
str r1, 1
jmp label2done
label1:
mov r1, 2
str r1, 3
jmp label2done
label2done:
ldr r1, 2
push r1
mov r1, 65
cmpStackTop r1
je label3
label3:
ldr r1, 0
push r1
mov r1, 1
subStackTop r1
pop r1
str r1, 0
jmp label4done
label4done:
ldr r1, 2
push r1
mov r1, 83
cmpStackTop r1
jne label6
je label5
label5:
ldr r1, 1
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 1
jmp label7done
label6:
mov r1, 2
str r1, 3
jmp label7done
label7done:
ldr r1, 2
push r1
mov r1, 68
cmpStackTop r1
jne label9
je label8
label8:
ldr r1, 0
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 0
jmp label10done
label9:
mov r1, 2
str r1, 3
jmp label10done
label10done:
ldr r1, 1
push r1
pop r2
ldr r1, 0
mov r3, 255
mov r4, 0
mov r5, 0
setPixelRedraw
jmp start
