;;;main - sets colors and pixel locations to random ;;
;;		then calls draw method in loop  			;;
main:
	rand
	push r5
	pop r1
	rand
	push r5
	pop r2
	push r5
	pop r4
	rand
	call drawHello
	sleep,50
	jmp main

hlt

;; drawHello- moves space between each character then draws ;; 
drawHello:
	str r1, 0
	str r2, 1
	call drawH

	call loadAndAdd
	call drawE

	call loadAndAdd
	call drawL

	call loadAndAdd
	call drawL

	call loadAndAdd
	call drawO

	call loadAndAdd
	call draw!

	redraw
	ret

drawH:
	mov r3, 11
hl1:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne hl1

	sub r2, 5
	mov r3, 5
hl2:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne hl2

	sub r2, 6
	mov r3, 11
hl3:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne hl3
	
	ret

drawE:
	mov r3, 10
el1:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne el1

	mov r3, 5
el2:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne el2

	sub r2, 5
	sub r1, 5
	mov r3, 5
el3:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne el3
	sub r2, 5
	sub r1, 5
	mov r3, 5
el4:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne el4
	
	ret

drawL:
	mov r3, 10
ll1:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne ll1

	mov r3, 5
ll2:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne ll2
	
	ret

drawO:
	mov r3, 10
ol1:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne ol1

	mov r3, 5
ol2:
	setPixel
	inc r1
	dec r3
	cmp r3, 0
	jne ol2
	
	mov r3, 10
ol3:
	setPixel
	dec r2
	dec r3
	cmp r3, 0
	jne ol3

	mov r3, 5
ol4:
	setPixel
	dec r1
	dec r3
	cmp r3, 0
	jne ol4
	
	ret

draw!:
	mov r3, 8
!l1:
	setPixel
	inc r2
	dec r3
	cmp r3, 0
	jne !l1

	add r2, 2
	setPixel

	ret

loadAndAdd:
	ldr r1, 0
	ldr r2, 1
	add r1, 10
	str r1, 0
	ret