unsigned int a = 0x0403;

//extern void memcpy(const unsigned int *src, const unsigned int* dst, const unsigned int len);

int main(void) {
	//memcpy(&a, (unsigned int*)0x04000000, sizeof(a));
	*(unsigned int*)0x04000000 = 0x0403;

	((unsigned short*)0x06000000)[120+80*240] = 0x001F;
	((unsigned short*)0x06000000)[136+80*240] = 0x03E0;
	((unsigned short*)0x06000000)[120+96*240] = 0x7C00;

	for(;;);
	return 0;
}
