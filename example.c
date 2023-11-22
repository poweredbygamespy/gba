#include <stdint.h>

uint16_t *fb = (void*)0x6000000;
const int xsz = 240;
const int ysz = 160;

#define FRAME_SEL_BIT		0x10
#define BG2_ENABLE			0x400

int AgbMain(void) {
	*(unsigned int*)0x04000000 = 0x0403;

	((unsigned short*)0x06000000)[120+80*240] = 0x001F;
	((unsigned short*)0x06000000)[136+80*240] = 0x03E0;
	((unsigned short*)0x06000000)[120+96*240] = 0x7C00;

	while(1);

	return 0;

	int i;
	static volatile uint16_t * const reg_disp_ctl = (void*)0x4000000;
	
	*reg_disp_ctl = 3 | BG2_ENABLE;
	*reg_disp_ctl &= ~FRAME_SEL_BIT;

	for(i=0; i<xsz * ysz; i++) {
		fb[i] = 31;
	}

	for(;;);
	return 0;
}
