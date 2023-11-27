#include <printf.h>
#include <virtterm.h>
#include <math.h>

//TODO move these two functions somewhere else
/*
static unsigned int pow(unsigned int b, int p) { //basis, potenz
	unsigned int res = 1;
	while (p--) {
		res = res * b;
	}

	return res;
}
*/

static int num_digits(unsigned int num, unsigned int base) {
	int digits = 1;
	unsigned int power = base;
	unsigned int max = udiv(UINT32_MAX, base) + 1;
	while (power < num) {
		if (power > max)
			return ++digits;
		digits++;
		power *= base;
	}

	return digits;
}

static void printf_raw(const char *s) {
	while (*s != '\0') {
		putchar(*s);
		s++;
	}
}

static void printf_s_len(const char *s, int len,
		int justify, char fill, int width) {
	if (justify) {
		printf_raw(s);
		width -= len;
		while (width-- > 0) {
			putchar(fill);
		}
	}
	else {
		while (width-- > len) {
			putchar(fill);
		}
		printf_raw(s);
	}
}

static int strlen(const char *s) {
	int c = 0;
	while (*s++ != '\0') {
		c++;
	}
	return c;
}

static void printf_s(const char *s, int justify, char fill, int width) {
	printf_s_len(s, strlen(s), justify, fill, width);
}

static void num_to_string(unsigned int num, unsigned int base, char *digits) {
	int count = num_digits(num, base);
	digits[count] = '\0';

	while (count--) {
		unsigned int div;
		unsigned int digit;
		udivmod(num, base, &div, &digit);
		num -= digit;
		num = udiv(num, base);
		digits[count] = (char)(digit < 10 ? '0' + digit : 'A' + digit - 10);
	}
}

static void printf_u(unsigned int num, unsigned int base,
		int justify, int zero, int width) {
	int count = num_digits(num, base);
	char digits[count + 1];
	num_to_string(num, base, digits);

	char fill = zero ? justify ? ' ' : '0' : ' ';
	printf_s_len(digits, count, justify, fill, width);
}

static void printf_i(int num, int justify, int zero, int width) {
	if (num > 0) {
		printf_u((unsigned int)num, 10, justify, zero, width);
		return;
	}

	unsigned int unum = (unsigned int)-num;

	int count = num_digits(unum, 10);
	char digits[count + 1];
	num_to_string(unum, 10, digits);

	if (!(zero | justify)) {
		printf_s_len("-", 1, 0, ' ', width - count);
		printf_s_len(digits, count, 0, ' ', 0);
		return;
	}

	putchar('-');
	char fill = zero ? justify ? ' ' : '0' : ' ';
	printf_s_len(digits, count, justify, fill, width - 1);
}

static void printf_p(void *p, int justify, int zero, int width) {
	int count = num_digits((unsigned int)p, 16);
	char digits[count + 1];
	num_to_string((unsigned int)p, 16, digits);

	if (!(zero | justify)) {
		printf_s_len("0x", 1, 0, ' ', width - count - 1);
		printf_s_len(digits, count, 0, ' ', 0);
		return;
	}

	printf_raw("0x");
	char fill = zero ? justify ? ' ' : '0' : ' ';
	printf_s_len(digits, count, justify, fill, width - 2);
}

static int printf_escape_sequence(const char **s, va_list *params) {
	// *s ist '%', inspiziere naechsten char
	(*s)++;

	// %(-?)(0?)(\d*)[scxiup%]       #1: justify #2: zero, #3: width
	int zero = 0, justify = 0, width = 0;

	if (**s == '-') {
		justify = 1;
		(*s)++;
	}
	if (**s == '0') {
		zero = 1;
		(*s)++;
	}
	while (**s <= '9' && **s >= '0') {
		width *= 10;
		width += **s - '0';
		(*s)++;
	}

	switch (**s) {
		case 's':
			if (zero) {
				return 1;
			}
			printf_s(va_arg(*params, const char*), justify, ' ', width);
			break;
		case 'c':
			if (zero || width || justify) {
				return 1;
			}
			putchar(va_arg(*params, int));
			break;
		case 'x':
			printf_u(va_arg(*params, unsigned int), 16, justify, zero, width);
			break;
		case 'i':
			printf_i(va_arg(*params, int), justify, zero, width);
			break;
		case 'u':
			printf_u(va_arg(*params, unsigned int), 10, justify, zero, width);
			break;
		case 'p':
			printf_p(va_arg(*params, void*), justify, zero, width);
			break;
		case '%':
			if (zero || width || justify) {
				return 1;
			}
			putchar('%');
			break;
		default:
			return 1;
	}

	return 0;
}

int printf(const char *s, ...) {
	if (!s) {
		printf_raw("Invalid format string\r\n");
		return 1;
	}

	va_list params;
	va_start(params, s);

	while (*s != '\0') {
		if (*s != '%') {
			putchar(*s);
			s++;
		}
		else {
			if (printf_escape_sequence(&s, &params) == 1) {
				printf_raw("Invalid escape sequence\r\n");
				va_end(params);
				return 1;
			}
			s++;
		}
	}

	va_end(params);
	return 0;
}
