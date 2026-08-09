#include <stdarg.h>
#include <stdio.h>

typedef unsigned char _uint8_t;
typedef unsigned int _uint16_t;
typedef unsigned long _uint32_t;
typedef unsigned long long _uint64_t;
typedef char _int8_t;
typedef int _int16_t;
typedef long _int32_t;
typedef long long _int64_t;
typedef float _float16_t;
typedef double _float32_t;
typedef long double _float64_t;

typedef char* strng;
#define bool _Bool
#define true 1
#define false 0
typedef struct numberVariable {
	_uint8_t decimalOffset;
	_uint8_t negative;
	_int64_t integerPart;
	_uint64_t decimalPart;
} nmbr;

#define DECIMAL_PRECISION 15

#define MAX_TEXT_BUFFER_LENGTH 1024

nmbr nmbrAdd(nmbr a, nmbr b);
nmbr nmbrSub(nmbr a, nmbr b);
nmbr nmbrMul(nmbr a, nmbr b);
nmbr nmbrDiv(nmbr a, nmbr b);
nmbr nmbrPow(nmbr a, nmbr b);
bool nmbrEql(nmbr a, nmbr b);
bool nmbrGrt(nmbr a, nmbr b);
bool nmbrLss(nmbr a, nmbr b);
bool nmbrGrtEql(nmbr a, nmbr b);
bool nmbrLssEql(nmbr a, nmbr b);

strng textFormat(strng text, ...) {
	char buf[1024];
	va_list vl;
	va_start(vl, text);
	vsnprintf(buf, sizeof(buf), text, vl);
	va_end(vl);
	strng result = buf;
	return result;
}
_uint64_t pow10u(_uint8_t n) {
	_uint64_t p = 1;
	while (n--) p *= 10;
	return p;
}

strng nmbrToString(nmbr n) {
	strng string = "";
	if (n.negative) string = "-";
	string = textFormat("%s%lld", string, (long long)n.integerPart);
	if (n.decimalOffset > 0) string = textFormat("%s.%0*llu", string, n.decimalOffset, (unsigned long long)n.decimalPart);
	return string;
}

nmbr nmbrVar(_int64_t value, _uint8_t digits) {
	nmbr out;
	if (value < 0) {
		out.negative = 1;
		value = -value;
	} else
		out.negative = 0;
	while (digits > DECIMAL_PRECISION) {
		value /= 10;
		digits--;
	}
	out.decimalOffset = digits;
	out.integerPart = value / pow10u(digits);
	out.decimalPart = value % pow10u(digits);
	return out;
}
_int64_t toScaledInteger(nmbr n, _uint8_t digits) {
	_uint64_t scale = pow10u(digits - n.decimalOffset);
	_int64_t value = n.integerPart * pow10u(digits) + n.decimalPart * scale;
	if (n.negative) value = -value;
	return value;
}
_float64_t nmbrToFloat(nmbr n) {
	_float64_t integer = (_float64_t)n.integerPart;
	_float64_t decimal = n.decimalPart / (_float64_t)pow10u(n.decimalPart);
	return integer + decimal;
}

nmbr nmbrAdd(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	_int64_t va = toScaledInteger(a, digits);
	_int64_t vb = toScaledInteger(b, digits);
	return nmbrVar(va + vb, digits);
}
nmbr nmbrSub(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	_int64_t va = toScaledInteger(a, digits);
	_int64_t vb = toScaledInteger(b, digits);
	return nmbrVar(va - vb, digits);
}
nmbr nmbrMul(nmbr a, nmbr b) {
	_int64_t va = toScaledInteger(a, a.decimalOffset);
	_int64_t vb = toScaledInteger(b, b.decimalOffset);
	_int64_t result = va * vb;
	return nmbrVar(
		result,
		a.decimalOffset + b.decimalOffset);
}
nmbr nmbrDiv(nmbr a, nmbr b) {
	_int64_t va = toScaledInteger(a, a.decimalOffset);
	_int64_t vb = toScaledInteger(b, b.decimalOffset);
	nmbr zero = nmbrVar(0, 0);
	if (vb == 0) return zero;
	_uint64_t numeratorScale = pow10u(DECIMAL_PRECISION + b.decimalOffset);
	_uint64_t denominatorScale = pow10u(a.decimalOffset);
	int64_t result = (va * numeratorScale) / (vb * denominatorScale);
	return nmbrVar(result, DECIMAL_PRECISION);
}
nmbr nmbrPow(nmbr a, nmbr b) {
	nmbr result = a;
	for (nmbr i = nmbrVar(2, 0); nmbrGrtEql(i, b); i = nmbrAdd(i, nmbrVar(1, 0))) nmbrMul(result, a);
	return result;
}

bool nmbrEql(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	return toScaledInteger(a, digits) == toScaledInteger(b, digits);
}
bool nmbrGrt(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	return toScaledInteger(a, digits) > toScaledInteger(b, digits);
}
bool nmbrLss(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	return toScaledInteger(a, digits) < toScaledInteger(b, digits);
}
bool nmbrGrtEql(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	return toScaledInteger(a, digits) >= toScaledInteger(b, digits);
}
bool nmbrLssEql(nmbr a, nmbr b) {
	_uint8_t digits = (a.decimalOffset > b.decimalOffset) ? a.decimalOffset : b.decimalOffset;
	return toScaledInteger(a, digits) <= toScaledInteger(b, digits);
}
