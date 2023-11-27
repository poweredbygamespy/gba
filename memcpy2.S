
.align
.thumb
@ Clear memory to 0x00 if length != 0
@ r0 = start address
@ r1 = length in bytes
.global memclrAligned
.thumb_func
memclrAligned:
	mov r2, r1
	mov r1, #0
	b memsetAligned
	bx lr
.align
.thumb

@ Clear memory to char if length != 0
@ r0 = start address
@ r1 = char
@ r2 = length in bytes
.global memsetAligned
.thumb_func
memsetAligned:
	cmp r2, #0
	beq memsetExit
memsetLoop:
	stmia r0!, {r1}
	sub r2, #4
	bne memsetLoop
memsetExit:
	bx lr

.align
.thumb
@ Copy memory if length != 0
@ r0 = Dest Address
@ r1 = Source Address
@ r2 = Length in Bytes
.global memcpyAligned
.thumb_func
memcpyAligned:
	cmp r2, #0
	beq memcpyExit
memcpyLoop:
	ldmia r1!, {r3}
	stmia r0!, {r3}
	sub r2, #4
	bne memcpyLoop
memcpyExit:
	bx lr

.align
.pool
