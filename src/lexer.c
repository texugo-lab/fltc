#include <stddef.h>

#include "essentials.h"

#pragma once

#define MAX_TOKENS 0x7fff

typedef enum tok_t {
	NUMBER = (int)'N',
	KEYWORD = (int)'K',
	VARIABLE = (int)'V',
	IDENTIFIER = (int)'I',
	ASSIGNMENT = (int)'A',
	NEWLINE = (int)';',
} TokenType;
typedef struct tok {
	strng token;
	TokenType type;
} Token;
typedef struct toks {
	Token* tokens;
	bool hasNumber;
	bool hasBool;
	bool hasStrng;
} Tokens;

// VARIABLES
#define NUMBER_VAR 0
strng variables[] = {
	"nmbr"};

// KEYWORDS
#define RETURN_KEYWORD 0
strng keywords[] = {
	"return"};

void tokenAppend(Token* tokenTable, Token token) {
	strng word = strdup(token.token);
	for (int i = 0; i < MAX_TOKENS; ++i) {
		if (!tokenTable[i].token || !tokenTable[i].type) {
			tokenTable[i].token = word;
			tokenTable[i].type = token.type;
			break;
		}
	}
}

bool needleInHaystack(char needle, strng haystack) {
	for (size_t i = 0; i <= strlen(haystack); ++i) {
		if (tolower(haystack[i]) == tolower(needle))
			return true;
	}

	return false;
}

Tokens lexer(strng string) {
	Tokens tokenTable = {.tokens = calloc(MAX_TOKENS, sizeof(Token)), .hasStrng = false, .hasBool = false, .hasNumber = false};
	Token* tokens = tokenTable.tokens;
	if (!tokens)
		return (Tokens){NULL};

	for (size_t i = 0; i <= strlen(string); i++) {
		if (string[i] == '\0' || string[i] == ' ')
			;
		else if (string[i] == ';' || string[i] == '\n') {
			char word[2] = {string[i], '\0'};
			tokenAppend(tokens, (Token){.token = word, .type = NEWLINE});
			continue;
		} else if (needleInHaystack(string[i], "_abcdefghijklmnopqrstuvwxyz")) {
			int j = i;
			for (; j <= (int)strlen(string); ++j)
				if (!needleInHaystack(string[j], "_abcdefghijklmnopqrstuvwxyz"))
					break;

			char word[j - i];
			for (int k = i; k <= j; ++k) {
				word[k - i] = string[k];
			}

			word[j - i] = '\0';

			bool keyword = false;
			bool variable = false;

			for (size_t k = 0; k < sizeof(keywords) / sizeof(keywords[0]); ++k)
				if (strcmp(keywords[k], word) == 0) keyword = true;

			for (size_t k = 0; k < sizeof(variables) / sizeof(variables[0]); ++k)
				if (strcmp(variables[k], word) == 0) variable = true;

			if (keyword)
				tokenAppend(tokens, (Token){word, KEYWORD});
			else if (variable) {
				tokenAppend(tokens, (Token){word, VARIABLE});
				if (strcmp(word, variables[NUMBER_VAR]) == 0) {
					tokenTable.hasNumber = true;
				}
			} else
				tokenAppend(tokens, (Token){word, IDENTIFIER});
			i = j - 1;
		}

		// NUMERIC //

		else if (needleInHaystack(string[i], "1234567890.")) {
			int j = i;
			for (; j <= (int)strlen(string); ++j)
				if (!needleInHaystack(string[j], "1234567890."))
					break;

			char word[j - i];
			for (int k = i; k <= j; ++k) {
				word[k - i] = string[k];
			}

			word[j - i] = '\0';
			tokenAppend(tokens, (Token){word, NUMBER});
			i = j - 1;
		} else if (string[i] == '=') {
			char word[2] = {'=', '\0'};
			tokenAppend(tokens, (Token){word, ASSIGNMENT});
		}
	}

	return tokenTable;
}
