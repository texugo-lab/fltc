#ifndef ESSENTIALS_H_
#define ESSENTIALS_H_

#pragma once

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ "r"
#define WRITE "w"

// Booleans //

#define bool _Bool
#define true 1
#define false 0

// Strings //

typedef char* strng;

// math //

typedef struct mt_s {
	double pi;
	double tau;
	double e;
	double inf;
} mt_s;

mt_s math = {
	.pi = 3.14159265358979323846,
	.tau = 6.28318530717958647692,
	.e = 2.71828182845904523536,
	.inf = INT_MAX};

typedef struct fraction {
	int numerator;
	int denominator;
} fraction;

int gcd(int number1, int number2) {
	number1 = (number1 > 0) ? number1 : -number1;
	number2 = (number2 > 0) ? number2 : -number2;

	while (number2 != 0) {
		int temp = number2;
		number2 = number1 % number2;
		number1 = temp;
	}

	return number1;
}

int lcm(int number1, int number2) {
	int max = (number1 > number2) ? number1 : number2;
	while (1) {
		if (max % number1 == 0 && max % number2 == 0) {
			return max;
		}

		++max;
	}

	return max;
}

double root(double radicand, double radical) {
	float result = 0.000000;
	float resultPower = result;

	for (size_t i = 2; i <= radical; i++) {
		resultPower *= result;
	}

	while (resultPower < radicand) {
		result += 0.000001;
		resultPower = result;
		for (size_t i = 2; i <= radical; i++) {
			resultPower *= result;
		}
	}

	if (resultPower > radicand) {
		result -= 0.000001;
	}

	return result;
}

double power(double base, fraction fraction) {
	// Inicia algumas variáveis para as contas //
	float result = 1;
	float rootRadicand = base;
	float numerator = fraction.numerator;
	float denominator = fraction.denominator;
	float expoent = numerator / denominator;

	// Menos conta a se fazer //
	if (expoent == 1) {
		return base;
	}

	// Inicia as contas //
	if (expoent > 0) {
		for (size_t i = 1; i < numerator; i++) rootRadicand *= base;
		result = rootRadicand;
	} else if (expoent == 0) {
		return result;
	} else if (expoent < 0) {
		for (size_t i = 1; i > numerator; i--) rootRadicand /= base;
		result = rootRadicand;
	}

	// Menos conta a se fazer //
	if (denominator == 1) {
		return rootRadicand;
	}

	// Retorna a raíz em que, o numerador vira o expoente do radicando e o denominador, o índice da raíz //
	return root(rootRadicand, denominator);
}

fraction decimalToFraction(float number) {
	float number2 = number;
	int denominator = 1;
	int numerator = number;
	int decimals = 0;
	int iGcd = 1;

	while (number2 != (int)number2) {
		number2 *= 10;
		decimals++;
	}

	for (int i = 1; i <= decimals; i++) {
		denominator *= 10;
		numerator = (int)number2;
	}

	iGcd = gcd(numerator, denominator);
	numerator /= iGcd;
	denominator /= iGcd;

	return (fraction){numerator, denominator};
}

double factorial(double number) {
	double n = (int)number;
	if (n == 0) return 1;
	return (n * factorial(n - 1));
}

bool even(int number) {
	if (number % 2)
		return true;
	else
		return false;
}

// text //

#define MAX_TEXT_BUFFER_LENGTH 1024

// TEXT FORMAT FUNCTION GOT FROM RAYLIB (https://github.com/raysan5/raylib/blob/master/src/rtext.c)

strng textFormat(const char* text, ...) {
#define MAX_TEXTFORMAT_BUFFERS 4  // Maximum number of static buffers for text formatting
	// Create an array of buffers so strings don't expire until MAX_TEXTFORMAT_BUFFERS invocations
	static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = {0};
	static int index = 0;

	strng currentBuffer = buffers[index];
	memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH);  // Clear buffer before using

	if (text != NULL) {
		va_list args;
		va_start(args, text);
		int requiredByteCount = vsnprintf(currentBuffer, MAX_TEXT_BUFFER_LENGTH, text, args);
		va_end(args);

		// If requiredByteCount is larger than the MAX_TEXT_BUFFER_LENGTH, then overflow occurred
		if (requiredByteCount >= MAX_TEXT_BUFFER_LENGTH) {
			// Inserting "..." at the end of the string to mark as truncated
			strng truncBuffer = buffers[index] + MAX_TEXT_BUFFER_LENGTH - 4;  // Adding 4 bytes = "...\0"
			snprintf(truncBuffer, 4, "...");
		}

		index += 1;	 // Move to next buffer for next function call
		if (index >= MAX_TEXTFORMAT_BUFFERS) index = 0;
	}

	return currentBuffer;
}

strng textInsert(strng text, strng inserted, int position) {
	strng finalText = text;
	strcpy(finalText, text);

	for (size_t i = position; i <= position + strlen(inserted); i++) {
		finalText[i] = inserted[i - position];
	}

	for (size_t i = position; i <= strlen(text); i++) {
		finalText[i + strlen(inserted)] = text[i];
	}

	return finalText;
}

// files //

FILE* fopencheck(strng fileName, strng mode) {
	return (fopen(fileName, mode)) ? fopen(fileName, mode) : NULL;
}

char* LoadFileText(const char* fileName) {
	char* text = NULL;

	if (fileName != NULL) {
		FILE* file = fopen(fileName, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			unsigned int size = (unsigned int)ftell(file);
			fseek(file, 0, SEEK_SET);

			if (size > 0) {
				text = (char*)calloc(size + 1, sizeof(char));
				if (text != NULL) {
					unsigned int count = (unsigned int)fread(text, sizeof(char), size, file);

					if (count < size) text = (char*)realloc(text, count + 1);
					text[count] = '\0';
				}
			}

			fclose(file);
		}
	}

	return text;
}

strng getFileExtension(strng fileName) {
	strng dot = strrchr(fileName, '.');

	if (!dot || (dot == fileName)) return NULL;

	return dot;
}

strng getFileNameNoExt(strng fileName) {
	int dotLocation = strlen(fileName) - strlen(getFileExtension(fileName));

	strng fileNameNoExt = fileName;
	strcpy(fileNameNoExt, fileName);
	fileNameNoExt[dotLocation] = '\0';

	return fileNameNoExt;
}

#endif
