#include <graphics/tiledbg.h>

#define MEM_IO (0x04000000)
#define MEM_VRAM (0x06000000)

#define tileset_memory ((tileset_t*)MEM_VRAM)
#define dtileset_memory ((dtileset_t*)MEM_VRAM)
#define tilemap_memory ((tilemap_t*)MEM_VRAM)
#define affine_tilemap_memory ((affine_tilemap_t*)MEM_VRAM)

#define REG_DISPCNT (*(volatile hword*)MEM_IO)
#define DCNT_MODE0 (0)
#define DCNT_MODE1 (1)
#define DCNT_MODE2 (2)
#define DCNT_MODE3 (3)
#define DCNT_MODE4 (4)
#define DCNT_MODE5 (5)
#define DCNT_GB (1<<3)
#define DCNT_PAGE (1<<4)
#define DCNT_OAM_HBL (1<<5)
#define DCNT_OBJ_2D (0<<6)
#define DCNT_OBJ_1D (1<<6)
#define DCNT_BLANK (1<<7)
#define DCNT_BG0 (1<<8)
#define DCNT_BG1 (1<<9)
#define DCNT_BG2 (1<<10)
#define DCNT_BG3 (1<<11)
#define DCNT_OBJ (1<<12)
#define DCNT_WIN0 (1<<13)
#define DCNT_WIN1 (1<<14)
#define DCNT_WINOBJ (1<<15)

#define REG_DISPSTAT (*(volatile hword*)(MEM_IO + 0x4))
#define DSTAT_IN_VBL (1<<0)
#define DSTAT_IN_HBL (1<<1)
#define DSTAT_IN_VCT (1<<2)
#define DSTAT_VBL_IRQ (1<<3)
#define DSTAT_HBL_IRQ (1<<4)
#define DSTAT_VCT_IRQ (1<<5)
#define DSTAT_VCT_TRG_VAL (0xff<<8)

#define REG_VCOUNT (*(volatile hword*)(MEM_IO + 0x6))

#define REG_BG0CNT (*(volatile hword*)(MEM_IO + 0x8))
#define REG_BG1CNT (*(volatile hword*)(MEM_IO + 0xa))
#define REG_BG2CNT (*(volatile hword*)(MEM_IO + 0xc))
#define REG_BG3CNT (*(volatile hword*)(MEM_IO + 0xe))
#define BG_PRIO0 (0)
#define BG_PRIO1 (1)
#define BG_PRIO2 (2)
#define BG_PRIO3 (3)
#define BG_CBB0 (0<<2)
#define BG_CBB1 (1<<2)
#define BG_CBB2 (2<<2)
#define BG_CBB3 (3<<2)
#define BG_MOSAIC (1<<6)
#define BG_CM_4BPP (0<<7)
#define BG_CM_8BPP (1<<7)
#define BG_SBB0 (0<<8)
#define BG_SBB1 (1<<8)
#define BG_SBB2 (2<<8)
#define BG_SBB3 (3<<8)
#define BG_SBB4 (4<<8)
#define BG_SBB5 (5<<8)
#define BG_SBB6 (6<<8)
#define BG_SBB7 (7<<8)
#define BG_SBB8 (8<<8)
#define BG_SBB9 (9<<8)
#define BG_SBB10 (10<<8)
#define BG_SBB11 (11<<8)
#define BG_SBB12 (12<<8)
#define BG_SBB13 (13<<8)
#define BG_SBB14 (14<<8)
#define BG_SBB15 (15<<8)
#define BG_SBB16 (16<<8)
#define BG_SBB17 (17<<8)
#define BG_SBB18 (18<<8)
#define BG_SBB19 (19<<8)
#define BG_SBB20 (20<<8)
#define BG_SBB21 (21<<8)
#define BG_SBB22 (22<<8)
#define BG_SBB23 (23<<8)
#define BG_SBB24 (24<<8)
#define BG_SBB25 (25<<8)
#define BG_SBB26 (26<<8)
#define BG_SBB27 (27<<8)
#define BG_SBB28 (28<<8)
#define BG_SBB29 (29<<8)
#define BG_SBB30 (30<<8)
#define BG_SBB31 (31<<8)
#define BG_REG_32x32 (0<<14)
#define BG_REG_64x32 (1<<14)
#define BG_REG_32x64 (2<<14)
#define BG_REG_64x64 (3<<14)
#define BG_AFF_16x16 (0<<14)
#define BG_AFF_32x32 (1<<14)
#define BG_AFF_64x64 (2<<14)
#define BG_AFF_128x128 (3<<14)

void setup(void) {
	REG_DISPCNT = DCNT_BG0 | DCNT_BG2 | DCNT_MODE1;
	REG_BG0CNT = BG_REG_32x32 | BG_SBB31 | BG_CBB0 | BG_PRIO1;
	REG_BG2CNT = BG_AFF_16x16 | BG_CM_4BPP | BG_SBB30 | BG_CBB1;
}

void load_tilemap(const tilemap_t tilemap, int sbb) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (&tilemap), "r" (&tilemap_memory[sbb]), "r" (sizeof(tilemap_t) / 4)
		: "r0", "r1", "r2");
}

void update_tile(int sbb, int offset, tilemap_entry_t te) {
	tilemap_memory[sbb][offset] = te;
}

void update_tile_affine(int sbb, int offset, affine_tilemap_entry_t te) {
	byte *address = &affine_tilemap_memory[sbb][offset];
	hword *align = (hword*)((unsigned int)address & ~1u);
	hword mask = (hword)(0xff<<(((unsigned int)address & 1u)) * 8);
	*align = (*align & ~mask) | ((te<<8 | te) & mask);
}

tilemap_entry_t flip_tilemap_entry_h(tilemap_entry_t te) {
	return te ^ (1<<10);
}

tilemap_entry_t flip_tilemap_entry_v(tilemap_entry_t te) {
	return te ^ (1<<11);
}

tilemap_entry_t set_tilemap_entry_palette(tilemap_entry_t te, byte pal) {
	return (hword)((te & 0xfff) | (pal<<12));
}

void shift_up(int sbb, unsigned int lines) {
	memcpy(&tilemap_memory[sbb], &tilemap_memory[sbb][32 * lines],
			(32 - lines) * 32 * sizeof(tilemap_entry_t));
	memclr(&tilemap_memory[sbb][(32 - lines) * 32],
			lines * 32 * (int)sizeof(tilemap_entry_t));
}

