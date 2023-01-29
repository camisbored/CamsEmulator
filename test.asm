mov r1, 5;comment here
mov r2, 3  ; comment here
push r1
addStacktop r2
pop r1
mov r3, 255
setPixelRedraw

;this whole line should be gone

beep,25,25
sleep,25
beep,25,25
sleep,25
beep,25,25
sleep,25

hlt