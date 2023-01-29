mov r3, 255
loop:
inc r1
inc r2
setpixel
cmp r1, 255
jne loop
redraw

alter:
inc r1
beep,25,25
sleep,25
cmp r1, 10
jne alter

hlt