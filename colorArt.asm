;fill fill screen with random colors in an incremental fashion
;meaning at the start of each loop a random color will dsiplay
;then we will increment based on that color

fillScreen:
	mov r1, 0
	mov r2, 0
drawLoop:
	setPixel
	cmp r1, 255
	inc r1
	inc r3
	inc r4
	inc r5
	jne drawLoop
	cmp r2, 255
	inc r2
	mov r1, 0
	jne drawLoop
	redraw
	sleep,10
rand
push r5
pop r3
rand
push r5
pop r4
rand
jmp fillScreen

