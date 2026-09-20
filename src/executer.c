#include <unistd.h>

#include "essentials.h"
#include "lexer.c"
#include "parser.c"

#pragma once

void compile(strng fileName, strng cFileName) {
	strng fileText = LoadFileText(fileName);
	FILE* cOutput = fopen(cFileName, WRITE);

	Tokens lexed = lexer(fileText);
	strng parsed = parse(lexed);

	{
		fprintf(cOutput,
				"#include <stdio.h>\n"
				"#include <stdarg.h>\n"
				"\n");
	}
	fprintf(cOutput, "int main(int argc, char* argv[]){\n");
	fprintf(cOutput, "%s\n", parsed);
	fprintf(cOutput, "\treturn 0;\n}");
	fclose(cOutput);
}
