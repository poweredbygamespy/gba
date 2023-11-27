#include <memory.h>

void memclr(void *dst, unsigned int len) {
	memset(dst, 0, len);
}

void memset(void *dst, const int byte, unsigned int len) {
	if (dst == 0 || len == 0)
		return;

	const char byte8 = (const char)byte;
	const unsigned int byte32 =
		(unsigned int)(byte8 | byte8<<8 | byte8<<16 | byte8<<24);
	unsigned int *dst32 = (unsigned int*)((unsigned int)dst & ~3u);

	unsigned int offset = (unsigned int)dst & 3u;
	if (offset) {
		unsigned int mask = 0;

		offset = 4 - offset;
		while (offset-- && len--) {
			mask = mask>>8 | 0xff000000;
		}
		*dst32 = (*dst32 & ~mask) | (byte32 & mask);
		dst32++;

		if (len == 0)
			return;
	}

	if (len >= 4) {
		unsigned int count = len / 4;
		count /= 8;
		switch ((len / 4) & 7u) {
			do {
						*dst32++ = byte32;
				//fallthrough
				case 7: *dst32++ = byte32;
				//fallthrough
				case 6: *dst32++ = byte32;
				//fallthrough
				case 5: *dst32++ = byte32;
				//fallthrough
				case 4: *dst32++ = byte32;
				//fallthrough
				case 3: *dst32++ = byte32;
				//fallthrough
				case 2: *dst32++ = byte32;
				//fallthrough
				case 1: *dst32++ = byte32;
				//fallthrough
				case 0: ;
			} while (count--);
		}

		len &= 3u;
		if (len == 0)
			return;
	}

	unsigned int mask = 0xffffffff>>((4 - len) * 8);
	*dst32 = (*dst32 & ~mask) | (byte32 & mask);
}

void memcpy(void *dst, const void *src, unsigned int len) {
	if (dst == 0 || src == 0 || len == 0)
		return;

	unsigned short* dst16;
	const unsigned char* src8;

	if ((unsigned int)dst % 4 == 0 && (unsigned int)src % 4 == 0 && len >= 4) {
		unsigned int *dst32 = (unsigned int*)dst;
		const unsigned int *src32 = (const unsigned int*)src;

		unsigned int count = len / 4;
		count /= 8;
		switch ((len / 4) & 7u) {
			do {
						*dst32++ = *src32++;
				//fallthrough
				case 7: *dst32++ = *src32++;
				// fallthrough
				case 6: *dst32++ = *src32++;
				// fallthrough
				case 5: *dst32++ = *src32++;
				// fallthrough
				case 4: *dst32++ = *src32++;
				// fallthrough
				case 3: *dst32++ = *src32++;
				// fallthrough
				case 2: *dst32++ = *src32++;
				// fallthrough
				case 1: *dst32++ = *src32++;
				// fallthrough
				case 0: ;
			} while (count--);
		}

		len &= 3u;
		if (len == 0)
			return;
		dst16 = (unsigned short*)dst32;
		src8 = (const unsigned char*)src32;
	}
	else {
		dst16 = (unsigned short*)dst;
		src8 = (const unsigned char*)src;
		if ((unsigned int)dst16 & 1u) {
			*dst16 = (unsigned short)((*dst16 & 0x00ff) | *src8++<<8);
			dst16 = (unsigned short*)((unsigned int)dst16 + 1);
			if (--len == 0)
				return;
		}
	}

	int count = (int)(len / 2);
	while (count--) {
		*dst16++ = (unsigned short)((src8[1]<<8) | src8[0]);
		src8 += 2;
	}

	if (len & 1u)
		*dst16 = (unsigned short)((*dst16 & 0xff00) | *src8);
}
