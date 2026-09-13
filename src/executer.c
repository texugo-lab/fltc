#include <stdio.h>
#include <unistd.h>

#include "essentials.h"
#include "lexer.c"
#include "parser.c"

#pragma once

void compile(strng fileName, strng cFileName) {
	strng fileText = LoadFileText(fileName);
	FILE* cOutput = fopen(cFileName, WRITE);

	Tokens lexed = lexer(fileText);

	{
		fprintf(cOutput,
				"#include <stdio.h>\n"
				"#include <stdarg.h>\n"
				"\n"
				"#define DECIMAL_PRECISION 15\n"
				"\n"
				"typedef char* strng;\n"
				"\n"
				"strng textFormat(strng string, ...){\n"
				"	char buf[1024];\n"
				"	va_list vl;\n"
				"	va_start(vl, string);\n"
				"	vsnprintf(buf, sizeof(buf), string, vl);\n"
				"	va_end(vl);\n"
				"	strng result = buf;\n"
				"	return result;\n"
				"}\n\n");
		if (lexed.hasBool) {
			fprintf(cOutput,
					"#define bool _Bool\n"
					"#define true 1\n"
					"#define false 0\n"
					"\n");
		}
		if (lexed.hasNumber) {
			fprintf(cOutput,
					"typedef unsigned char unmbr8;\n"
					"typedef unsigned int unmbr16;\n"
					"typedef unsigned long unmbr32;\n"
					"typedef unsigned long long unmbr64;\n"
					"typedef char nmbr8;\n"
					"typedef int nmbr16;\n"
					"typedef long nmbr32;\n"
					"typedef long long nmbr64;\n"
					"typedef float fnmbr16;\n"
					"typedef double fnmbr32;\n"
					"typedef long double fnmbr64;\n"
					"typedef nmbr32 nmbr;\n"
					"\n");
		}
	}
	fprintf(cOutput, "int main(int argc, strng argv[]){\n");

	fprintf(cOutput, "%s\n", parse(lexed));
	fprintf(cOutput, "\treturn 0;\n}");
	fclose(cOutput);
}
