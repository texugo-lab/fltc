#include <stdlib.h>
#include <unistd.h>

#include "src/essentials.h"
#include "src/executer.c"

strng flags[] = {"-i", "-o", "-asm", "-c"};

int main(int argc, strng argv[]) {
	strng inFileName = "\0";
	strng outFileName = "\0";

	bool hasInput = false;
	bool hasOutput = false;

	for (int i = 1; i < argc; ++i) {
		bool flagExists = false;
		for (size_t j = 0; j < sizeof(flags) / sizeof(flags[0]); ++j) {
			if (strcmp(flags[j], argv[i]) == 0) {
				flagExists = true;
			}
		}
		if (!flagExists) {
			printf("Unknown Flag:\n\t%s\n", argv[i]);
			exit(1);
		}
		if (strcmp(argv[i], "-i") == 0) {
			inFileName = argv[i + 1];
			++i;
			hasInput = true;
			continue;
		} else if (strcmp(argv[i], "-o") == 0) {
			outFileName = argv[i + 1];
			++i;
			hasOutput = true;
			continue;
		}
	}

	if (!hasInput || !hasOutput) {
		printf("Correct Usage:\n");
		printf("	fcc -i [inputFile] -o [outputFile] [Flags]\n");
		return 0;
	}

	strng cFileName = textFormat("%s.c", outFileName);

	compile(inFileName, outFileName);

	strng args[] = {
		"cc",
		cFileName,
		"-o",
		outFileName,
		NULL};

	execvp("cc", args);
	exit(0);
}
