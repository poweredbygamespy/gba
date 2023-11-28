#include <memory.h>

void memclr(void *dst, unsigned int len) {
	memset(dst, 0, len);
}

void memset(void *dst, const int fill_byte, unsigned int len) {
	if (dst == 0 || len == 0)
		return;

	const byte byte8 = (const byte)fill_byte;
	const word byte32 = (word)(byte8 | byte8<<8 | byte8<<16 | byte8<<24);
	word *dst32 = (word*)((unsigned int)dst & ~3u);

	word offset = (unsigned int)dst & 3u;
	if (offset) {
		word mask = 0;

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

	word mask = 0xffffffff>>((4 - len) * 8);
	*dst32 = (*dst32 & ~mask) | (byte32 & mask);
}

void memcpy(void *dst, const void *src, unsigned int len) {
	if (dst == 0 || src == 0 || len == 0)
		return;

	hword* dst16;
	const byte* src8;

	if ((unsigned int)dst % 4 == 0 && (unsigned int)src % 4 == 0 && len >= 4) {
		word *dst32 = (word*)dst;
		const word *src32 = (const word*)src;

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
		dst16 = (hword*)dst32;
		src8 = (const byte*)src32;
	}
	else {
		dst16 = (hword*)dst;
		src8 = (const byte*)src;
		if ((unsigned int)dst16 & 1u) {
			*dst16 = (hword)((*dst16 & 0x00ff) | *src8++<<8);
			dst16 = (hword*)((unsigned int)dst16 + 1);
			if (--len == 0)
				return;
		}
	}

	int count = (int)(len / 2);
	while (count--) {
		*dst16++ = (hword)((src8[1]<<8) | src8[0]);
		src8 += 2;
	}

	if (len & 1u)
		*dst16 = (hword)((*dst16 & 0xff00) | *src8);
}
