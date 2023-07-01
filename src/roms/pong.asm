;mem address 0: current color
;mem address 1: opp x position
;mem address 2: opp y position
;mem address 3: lower bounds of opp positioning
;mem address 4: upper bounds of opp positioning
;mem address 5: ball x position
;mem address 6: ball y position
;mem address 7: ball direction (0 left, 1 right)
;mem address 8: player x position
;mem address 9: player y position

main:
	; set variables and move to mem addresses stated above
	mov r1, 240
	mov r2, 50
	str r1, 1
	str r2, 2
	mov r1, 127
	str r1, 5
	str r1, 6
	mov r1, 0
	str r1, 7
	mov r1, 5
	mov r2, 10
	call drawLine

	;main logic/main loop
gameLoop:
	sleep,1
	; if up or down pressed, update and draw accordingly
	checkKeyPress
	cmp r5, 38
	je upPressed
retFromUpPressed:
	cmp r5, 40
	je downPressed
retFromDownPressed:
	; default to be drawn each cycle (opponent, ball)
	str r1, 8
	str r2, 9
	push r1
	push r2
	ldr r1, 1
	ldr r2, 2
	call setWhite
	call drawLine
	ldr r3, 3
	call checkDirection
	call setBlack
	cmp r2, 10
	str r2, 2
	call drawLine
	call drawBall
	pop r2
	pop r1
	redraw
	jmp gameLoop
hlt

drawBall:
	ldr r1, 5
	ldr r2, 6
	call setWhite
	call ballPixels
	call setBlack
	call collisionCheck
	str r1, 5
	str r2, 6
	call ballPixels
	ret

	; if ball hits paddle, flip direction
collisionCheck:
	push r3
	push r4
	push r5
	ldr r4, 9
	ldr r5, 7
	call checkBallDir
	cmp r5, 0
	jne incInstead
	dec r1
	inc r2
retFromIncInstead:
	pop r5
	pop r4
	pop r3
	ret

checkBallDir:
	cmp r4, 216
	je flipDirection
retFromFlipDirection:
	ret

flipDirection: 
	cmp r1, 5
	je flipIt
retFromFlipIt:
jmp retFromFlipDirection

flipIt:
	mov r5, 1
	str r5, 7
	jmp retFromFlipIt

incInstead:
	inc r1
	dec r2
	jmp retFromIncInstead

ballPixels:
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
	setPixel
	ret

drawLine:	
	str r3,0
	mov r3,0
drawLoop1:
	push r3
	ldr r3,0
	setPixel
	pop r3
	inc r2
	inc r3
	cmp r3, 40
	jne drawLoop1
	inc r1
drawLoop2:
	push r3
	ldr r3,0
	setPixel
	pop r3
	dec r3
	dec r2
	cmp r3, 0
	jne drawLoop2
	dec r1
ret

upPressed:
	beep,25,25
	call setWhite
	call drawLine
	call setBlack
	dec r2
	call drawLine
	jmp retFromUpPressed

downPressed:
	beep,25,25
	call setWhite
	call drawLine
	call setBlack
	inc r2
	call drawLine
	jmp retFromDownPressed

setBlack:
	mov r3, 0
	mov r4, 0
	mov r5, 0
	ret

setWhite:
	mov r3, 255
	mov r4, 255
	mov r5, 255
	ret

checkDirection:
	cmp r2, 10
	je setDownDir
retFromSetDownDir:
	cmp r2, 210
	je setUpDir
retFromSetUpDir:
	cmp r3, 0
	je inc2game
retFromInc2game:
	cmp r3, 1
	je dec2game
retFromDec2game:
	ret

setDownDir:
	mov r3,0
	str r3,3
	jmp retFromSetDownDir
setUpDir:
	mov r3,1
	str r3,3
	jmp retFromSetUpDir
inc2game:
	inc r2
	jmp retFromInc2game
dec2game:
	dec r2
	jmp retFromDec2game
