.section .iwram, "ax"

.section .text

.global _start
.align
.arm
_start:
	@ Start Vector
	b rom_header_end

	@ Nintendo Logo Character Data (156 Bytes) (8000004h)
	.word 0x51aeff24,0x21a29a69,0x0a82843d
	.word 0xad09e484,0x988b2411,0x217f81c0,0x19be52a3
	.word 0x20ce0993,0x4a4a4610,0xec3127f8,0x33e8c758
	.word 0xbfcee382,0x94dff485,0xc1094bce,0xc08a5694
	.word 0xfca77213,0x734d849f,0x619acaa3,0x27a39758
	.word 0x769803fc,0x61c71d23,0x56ae0403,0x008438bf
	.word 0xfd0ea740,0x03fe52ff,0xf130956f,0x85c0fb97
	.word 0x2580d660,0x03be63a9,0xe2384e01,0xff34a2f9
	.word 0x44033ebb,0xcb900078,0x943a1188,0x637cc065
	.word 0xaf3cf087,0x8be425d6,0x72ac0a38,0x07f8d421

	@ Game Title (12 Bytes) (80000A0h)
	.ascii "Playground\0\0"

	@ Game Code (80000ACh)
	.byte 0x00,0x00,0x00,0x00

	@ Maker Code (80000B0h)
	.byte 0x30,0x31	@ 01: Nintendo

	@ Fixed Value (80000B2h)
	.byte 0x96

	@ Main Unit Code (80000B3h)
	.byte 0x00

	@ Device Type (80000B4h)
	.byte 0x00

	@ Unused Data (7 Bytes) (80000B5h)
	.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00

	@ Software Version No (80000BCh)
	.byte 0x00

	@ Complement Check (80000BDh)
	.byte 0xcb

	@ Unused Data (2 Bytes) (80000BEh)
	.byte 0x00,0x00

.align
.arm
rom_header_end:
	b start_vector	@ This branch must be here for proper
					@ positioning of the following header.
					@ DO NOT REMOVE IT.

.global __boot_method, __slave_number
__boot_method:
	.byte 0x00		@ boot method (0=ROM boot, 3=Multiplay boot)
__slave_number:
	.byte 0x00		@ slave # (1=slave#1, 2=slave#2, 3=slave#3)

	.word 0x00000000
	.word 0x00000000
	.word 0x00000000
	.word 0x00000000
	.word 0x00000000
	.word 0x00000000
	.word 0x00000000
	.byte 0x00
	.byte 0x00

.align
.arm
.global start_vector
start_vector:
	mov r0, #0x12			@ Switch to IRQ mode
	msr cpsr, r0
	ldr sp, =__sp_irq		@ Set SP_irq
	mov r0, #0x1f			@ Switch to system mode
	msr cpsr, r0
	ldr sp, =__sp_usr		@ Set SP_usr

	add r0, pc, #1			@ Enter THUMB mode
	bx r0

.align
.thumb
WRAMSetup:
	@ Clear External WRAM to 0x00
	mov r1, #0x40
	lsl r1, #12			@ r1 = 0x40000
	lsl r0, r1, #7		@ r0 = 0x2000000
	lsr r1, #2
	bl memclr

	@ Clear Internal WRAM to 0x00
	mov r0, #3
	lsl r0, #24			@ r0 = 0x3000000
	ldr r1, =__sp_usr_offset - 16
	lsr r1, #2
	bl memclr

	@ Copy initialized data (data section) from LMA to VMA
	ldr r0, =__data_start
	ldr r1, =__data_lma
	ldr r2, =__data_end
	sub r2, r2, r0
	lsr r2, #2
	bl memcpy

	@ Copy internal work ram (iwram section) from LMA to VMA
	ldr r0, =__iwram_start
	ldr r1, =__iwram_lma
	ldr r2, =__iwram_end
	sub r2, r2, r0
	lsr r2, #2
	bl memcpy

	@ Copy internal work ram overlay 0 (iwram0 section) from LMA to VMA
	ldr r0, =__iwram_overlay_start
	ldr r1, =__load_start_iwram0
	ldr r2, =__load_stop_iwram0
	sub r2, r2, r1
	lsr r2, #2
	bl memcpy

	@ Copy external work ram (ewram section) from LMA to VMA
	ldr r0, =__ewram_start
	ldr r1, =__ewram_lma
	ldr r2, =__ewram_end
	sub r2, r2, r0
	lsr r2, #2
	bl memcpy

	@ Copy external work ram overlay 0 (ewram0 section) from LMA to VMA
	ldr r0, =__ewram_overlay_start
	ldr r1, =__load_start_ewram0
	ldr r2, =__load_stop_ewram0
	sub r2, r2, r1
	lsr r2, #2
	bl memcpy

	@ Set up heap in external WRAM
	ldr r0, =start_user	@ Jump to start_user routine when we're done
	mov lr, r0
	ldr r0, =heap_init
	bx r0

.align
.thumb
.global start_user
.thumb_func
start_user:
	@ Jump to user code
	mov r0, #0			@ int argc
	mov r1, #0			@ char *argv[]

	ldr r3, =start_vector
	mov lr, r3			@ Set start_vector as return address

	ldr r3, =start_user	@ Test
	ldr r3, =main
	bx r3
