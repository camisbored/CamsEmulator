label0:
mov r1, 1
push r1
mov r1, 1
cmpStackTop r1
jne label1done
rand
push r5
pop r1
str r1, 0
rand
push r5
pop r1
str r1, 1
rand
push r5
pop r1
str r1, 2
rand
push r5
pop r1
str r1, 3
rand
push r5
pop r1
str r1, 4
ldr r1, 4
push r1
pop r5
ldr r1, 3
push r1
pop r4
ldr r1, 2
push r1
pop r3
ldr r1, 1
push r1
pop r2
ldr r1, 0
setPixelRedraw
sleep, 1
jmp label0
label1done:
