
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

__text_start = 0x8000000;
__ewram_start     = 0x2000000;
__eheap_end       = 0x2040000;
__iwram_start     = 0x3000000;
__iheap_end       = 0x3008000 - 0x400;
__sp_usr          = 0x3008000 - 0x100;
__sp_irq          = 0x3008000 - 0x60;
__intr_vector_buf = 0x3008000 - 0x4;
__sp_usr_offset   = __sp_usr - __iwram_start;


SECTIONS
{
	.text __text_start :
	{
		*(EXCLUDE_FILE (*iwram.*) .text)
		*(.text.*)
		*(.stub)
		/* .gnu.warning sections are handled specially by elf32.em. */
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		*(.glue_7)
		*(.glue_7t)
		. = ALIGN(4);
	} = 0xff
	__text_end = .;

	.rodata :
	{
		*(.rodata*)
		*(.gnu.linkonce.r*)
		. = ALIGN(4);
	} = 0xff

	.gcc_except_table :
	{
		*(.gcc_except_table)
		. = ALIGN(4);
	} = 0x00

	__iwram_lma = (ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.gcc_except_table) + 3) & ~ 3;
	.iwram __iwram_start : AT(__iwram_lma)
	{
		*(.iwram)
		*iwram.*(.text)
		. = ALIGN(4);
	} = 0xff
	__iwram_end = .;

	__bss_start = .;
	.bss ALIGN(4) :
	{
		*(.dynbss)
		*(.gnu.linkonce.b*)
		*(COMMON)
		. = ALIGN(4);
	} = 0x00
	__bss_end = .;

	__data_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram);
	__data_start = .;
	.data ALIGN(4) : AT(__data_lma)
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		. = ALIGN(4);
	} = 0xff
	__data_end = .;

	__iwram_overlay_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data);
	__iwram_overlay_start = .;
	OVERLAY : NOCROSSREFS AT(__iwram_overlay_lma)
	{
		.iwram0 { *(.iwram0) . = ALIGN(4);}
		.iwram1 { *(.iwram1) . = ALIGN(4);}
		.iwram2 { *(.iwram2) . = ALIGN(4);}
		.iwram3 { *(.iwram3) . = ALIGN(4);}
		.iwram4 { *(.iwram4) . = ALIGN(4);}
		.iwram5 { *(.iwram5) . = ALIGN(4);}
		.iwram6 { *(.iwram6) . = ALIGN(4);}
		.iwram7 { *(.iwram7) . = ALIGN(4);}
		.iwram8 { *(.iwram8) . = ALIGN(4);}
		.iwram9 { *(.iwram9) . = ALIGN(4);}
	} = 0xff
	__iwram_overlay_end = .;
	__iheap_start = .;
	__iheap_size = __iheap_end - __iheap_start;

	__ewram_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data) + SIZEOF(.iwram0) + SIZEOF(.iwram1) + SIZEOF(.iwram2) + SIZEOF(.iwram3) + SIZEOF(.iwram4) + SIZEOF(.iwram5) + SIZEOF(.iwram6) + SIZEOF(.iwram7) + SIZEOF(.iwram8) + SIZEOF(.iwram9);
	.ewram __ewram_start : AT(__ewram_lma)
	{
		*(.ewram)
		. = ALIGN(4);
	} = 0xff
	__ewram_end = .;

	__ewram_overlay_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data) + SIZEOF(.iwram0) + SIZEOF(.iwram1) + SIZEOF(.iwram2) + SIZEOF(.iwram3) + SIZEOF(.iwram4) + SIZEOF(.iwram5) + SIZEOF(.iwram6) + SIZEOF(.iwram7) + SIZEOF(.iwram8) + SIZEOF(.iwram9) + SIZEOF(.ewram);
	__ewram_overlay_start = .;
	OVERLAY : NOCROSSREFS AT(__ewram_overlay_lma)
	{
		.ewram0 { *(.ewram0) . = ALIGN(4);}
		.ewram1 { *(.ewram1) . = ALIGN(4);}
		.ewram2 { *(.ewram2) . = ALIGN(4);}
		.ewram3 { *(.ewram3) . = ALIGN(4);}
		.ewram4 { *(.ewram4) . = ALIGN(4);}
		.ewram5 { *(.ewram5) . = ALIGN(4);}
		.ewram6 { *(.ewram6) . = ALIGN(4);}
		.ewram7 { *(.ewram7) . = ALIGN(4);}
		.ewram8 { *(.ewram8) . = ALIGN(4);}
		.ewram9 { *(.ewram9) . = ALIGN(4);}
	} = 0xff
	__ewram_overlay_end = .;
	__eheap_start = .;
	__eheap_size = __eheap_end - __eheap_start;

	_end = DEFINED (__gba_iwram_heap) ? __iheap_start : __eheap_start;
	__end__ = _end;
	PROVIDE (end = _end);

	/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }

	/* DWARF debug sections.
	 * Symbols in the DWARF debugging sections are relative to the beginning
	 * of the section so we begin them at 0.
	 */
	/* DWARF 1 */
	.debug 0 : { *(.debug) }
	.line 0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo 0 : { *(.debug_srcinfo) }
	.debug_sfnames 0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info 0 : { *(.debug_info) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line) }
	.debug_frame 0 : { *(.debug_frame) }
	.debug_str 0 : { *(.debug_str) }
	.debug_loc 0 : { *(.debug_loc) }
	.debug_macinfo 0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames	0 : { *(.debug_varnames) }
	.stack 0x80000 : { _stack = .; *(.stack) }
	/* These must appear regardless of . */
}
