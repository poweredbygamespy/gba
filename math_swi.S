.section .text

.align

.global div
.thumb_func
div:
	swi 0x6
	bx lr

.global mod
.thumb_func
mod:
	swi 0x6
	mov r0, r1
	bx lr

.global divmod
.thumb_func
divmod:
	push {r2, r3}
	swi 0x6
	pop {r2, r3}
	str r0, [r2]
	str r1, [r3]
	bx lr

.global obj_affine_set
.thumb_func
obj_affine_set:
	swi 0xf
	bx lr

.global bg_affine_set
.thumb_func
bg_affine_set:
	swi 0xe
	bx lr
