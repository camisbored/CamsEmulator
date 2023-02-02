; will fill screen with random colors for each pixel
; r1 is xPos, r2 is yPos, r3-5 are r,g,b


fillScreen:
	mov r1, 0
	mov r2, 0
drawLoop:
	setPixel
	cmp r1, 255
	inc r1
	; set random colors in registers 3, 4, 5
	rand
	push r5
	pop r3
	rand
	push r5
	pop r4
	rand
	jne drawLoop
	cmp r2, 255
	inc r2
	mov r1, 0
	;go to start of next line
	jne drawLoop
	;pixel mapping complete, draw and restart
	redraw
jmp fillScreen
