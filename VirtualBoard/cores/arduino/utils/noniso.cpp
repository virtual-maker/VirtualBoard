/*
 noniso.cpp - replacements for non-ISO functions used by Arduino core
 Copyright © 2016 Ivan Grokhotkov

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 Modified August 2016 by Marcelo Aquino <marceloaqno@gmail.org> for MySensors use
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "stdlib_noniso.h"

char* utoa(unsigned value, char* result, int base)
{
	if(base < 2 || base > 16) {
		*result = 0;
		return result;
	}

	char* out = result;
	unsigned quotient = value;

	do {
		const unsigned tmp = quotient / base;
		*out = "0123456789abcdef"[quotient - (tmp * base)];
		++out;
		quotient = tmp;
	} while(quotient);

	reverse(result, out);
	*out = 0;
	return result;
}

//char* itoa(int value, char* result, int base)
//{
//	if(base < 2 || base > 16) {
//		*result = 0;
//		return result;
//	}
//
//	char* out = result;
//	int quotient = abs(value);
//
//	do {
//		const int tmp = quotient / base;
//		*out = "0123456789abcdef"[quotient - (tmp * base)];
//		++out;
//		quotient = tmp;
//	} while(quotient);
//
//	// Apply negative sign
//	if(value < 0) {
//		*out++ = '-';
//	}
//
//	reverse(result, out);
//	*out = 0;
//	return result;
//}

//int atoi(const char* s)
//{
//	return (int) atol(s);
//}

//long atol(const char* s)
//{
//	char * tmp;
//	return strtol(s, &tmp, 10);
//}

//double atof(const char* s)
//{
//	char * tmp;
//	return strtod(s, &tmp);
//}

void reverse(char* begin, char* end)
{
	char *is = begin;
	char *ie = end - 1;
	while(is < ie) {
		char tmp = *ie;
		*ie = *is;
		*is = tmp;
		++is;
		--ie;
	}
}

//char* ltoa(long value, char* result, int base)
//{
//	if(base < 2 || base > 16) {
//		*result = 0;
//		return result;
//	}
//
//	char* out = result;
//	long quotient = abs(value);
//
//	do {
//		const long tmp = quotient / base;
//		*out = "0123456789abcdef"[quotient - (tmp * base)];
//		++out;
//		quotient = tmp;
//	} while(quotient);
//
//	// Apply negative sign
//	if(value < 0) {
//		*out++ = '-';
//	}
//
//	reverse(result, out);
//	*out = 0;
//	return result;
//}

//char* ultoa(unsigned long value, char* result, int base)
//{
//	if(base < 2 || base > 16) {
//		*result = 0;
//		return result;
//	}
//
//	char* out = result;
//	unsigned long quotient = value;
//
//	do {
//		const unsigned long tmp = quotient / base;
//		*out = "0123456789abcdef"[quotient - (tmp * base)];
//		++out;
//		quotient = tmp;
//	} while(quotient);
//
//	reverse(result, out);
//	*out = 0;
//	return result;
//}

char* dtostrf (double val, signed char width, unsigned char prec, char *s)
{
	sprintf(s, "%*.*f", width, prec, val);
	return s;
}
