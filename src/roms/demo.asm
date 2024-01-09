mov r1, 1
push r1
mov r1, 3
addStackTop r1
mov r1, 2
subStackTop r1
mov r1, 2
mulStackTop r1
mov r1, 2
divStackTop r1
mov r1, 1
orStackTop r1
mov r1, 2
andStackTop r1
mov r1, 1
shlStackTop r1
mov r1, 2
shrStackTop r1
mov r1, 3
xorStackTop r1
pop r1
str r1, 0
mov r1, 4
str r1, 1
sleep, 25
beep,25,25
ldr r1, 1
push r1
pop r2
ldr r1, 0
mov r3, 0
mov r4, 0
mov r5, 0
setPixelRedraw
getch
push r5
pop r1
str r1, 2
rand
push r5
pop r1
str r1, 3
ldr r1, 0
push r1
ldr r1, 1
cmpStackTop r1
je label0
je label0
jne label1
label0:
mov r1, 122
str r1, 4
jmp label0done
label1:
mov r1, 432
str r1, 5
label0done:
label2:
ldr r1, 1
push r1
mov r1, 200
cmpStackTop r1
je label3done
ldr r1, 1
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 1
ldr r1, 1
push r1
pop r2
ldr r1, 0
mov r3, 0
mov r4, 0
mov r5, 0
setPixelRedraw
sleep, 25
getch
push r5
pop r1
str r1, 6
jmp label2
label3done:
myFunc:
ldr r1, 0
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 0
ldr r1, 1
push r1
mov r1, 1
addStackTop r1
pop r1
str r1, 1
ret
call myFunc
hlt
