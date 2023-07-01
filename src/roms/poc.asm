;w = 0x57
;a = 0x41
;s = 0x53
;d = 0x44

;up = 0x26
;left = 0x25
;down = 0x28
;right = 0x27

main:
	getch
	cmp r5, 87 
	je wPressed
	cmp r5, 65
	je aPressed
	cmp r5, 83 
	je sPressed
	cmp r5, 68
	je dPressed
	cmp r5, 38
	je upPressed
	cmp r5, 37
	je leftPressed
	cmp r5, 40
	je downPressed
	cmp r5, 39
	je rightPressed
	jmp elsePressed
	hlt

;fillScreen- will color screen with whatever RGB values are in r3,r4,r5
fillScreen:
	push r1
	push r2
	mov r1, 0
	mov r2, 0
drawLoop:
	setPixel
	cmp r1, 255
	inc r1
	jne drawLoop
	cmp r2, 255
	inc r2
	mov r1, 0
	jne drawLoop
	redraw
	pop r2
	pop r1
	ret

clearBox:
	call clear3to5
	setPixel
	dec r2
	setPixel
	dec r1
	setPixel
	inc r2
	setPixel
	inc r2
	setPixel
	inc r1
	setPixel
	inc r1
	setPixel
	dec r2
	setPixel
	dec r2
	setPixel
	dec r1
	inc r2
	ret

drawBox:
	mov r3, 255
	setPixel
	dec r2
	setPixel
	dec r1
	setPixel
	inc r2
	setPixel
	inc r2
	setPixel
	inc r1
	setPixel
	inc r1
	setPixel
	dec r2
	setPixel
	dec r2
	setPixel
	dec r1
	inc r2
	redraw
	ret

wPressed:
	beep,10,10
	call clear3to5
	mov r3, 255
	mov r4, 255
	mov r5, 255
	call fillScreen
	jmp main
aPressed:
	beep,10,10
	call clear3to5
	mov r3, 255
	call fillScreen
	jmp main
sPressed:
	beep,10,10
	call clear3to5
	mov r4, 255
	call fillScreen
	jmp main

dPressed:
	beep,10,10
	call clear3to5
	mov r5, 255
	call fillScreen
	jmp main

upPressed:
	call clearBox
	dec r2
	call drawBox
	jmp main

leftPressed:
	call clearBox
	dec r1
	call drawBox
	jmp main

downPressed:
	call clearBox
	inc r2
	call drawBox
	jmp main

rightPressed:
	call clearBox
	inc r1
	call drawBox
	jmp main

elsePressed:
	call clear3to5
	call fillScreen
	jmp main

clear3to5:
	mov r3, 0
	mov r4, 0
	mov r5, 0
	ret
