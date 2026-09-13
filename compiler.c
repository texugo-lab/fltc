#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "src/essentials.h"
#include "src/executer.c"

#define HELP                                            \
	"Correct Usage:\n"                                  \
	"    fltc -i <InputFile> -o <OutputFile> [Flags]\n" \
	"Flags:\n"                                          \
	"    -c             -> Generate C File\n"           \
	"    -asm           -> Generate Assembly File\n"    \
	"\n"                                                \
	"    -h | --help    -> Print this documentation\n"  \
	"    -v | --version -> Print current fltc version\n"

#define VERSION \
	"1.0.1\n"

void dir(strng dirName) {
	pid_t pid = fork();
	if (pid > 0) {
		wait(NULL);
	} else {
		execlp("mkdir", "mkdir", dirName, NULL);
	}
}
void touch(strng name) {
	if (fork() > 0) {
		wait(NULL);
	} else {
		execlp("touch", "touch", name, NULL);
	}
}
void rm(strng name) {
	pid_t pid = fork();
	if (pid > 0) {
		wait(NULL);
	} else {
		execlp("rm", "rm", "-rf", name, NULL);
	}
}

strng flags[] = {"-i", "-o", "-asm", "-c", "-h", "--help", "-v", "--version"};

int main(int argc, strng argv[]) {
	strng inFileName = "\0";
	strng outFileName = "\0";

	strng compileFolder = ".fltc";

	bool hasInput = false;
	bool hasOutput = false;

	bool C = false;
	bool ASM = false;

	for (int i = 1; i < argc; ++i) {
		bool flagExists = false;
		for (size_t j = 0; j < sizeof(flags) / sizeof(flags[0]); ++j) {
			if (strcmp(flags[j], argv[i]) == 0) {
				flagExists = true;
			}
		}
		if (!flagExists) {
			printf("Unknown Flag:\n\t%s\n", argv[i]);
			exit(EXIT_SUCCESS);
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
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf(HELP);
			exit(EXIT_SUCCESS);
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			printf(VERSION);
			exit(EXIT_SUCCESS);
		} else if (strcmp(argv[i], "-c") == 0) {
			C = true;
			ASM = false;
		} else if (strcmp(argv[i], "-asm") == 0) {
			ASM = true;
			C = false;
		}
	}

	if (!hasInput || !hasOutput) {
		printf(HELP);
		return EXIT_SUCCESS;
	}

	if (!C && !ASM) {
		dir(compileFolder);

		strng cFileName = textFormat(".fltc/%s.c", outFileName);
		compile(inFileName, cFileName);
		if (fork() > 0)
			wait(NULL);
		else
			execlp("gcc", "gcc", cFileName, "-o", outFileName, NULL);
		remove(cFileName);

		rm(compileFolder);
	} else if (C) {
		strng cFileName = textFormat("%s", outFileName);
		if (strcmp((cFileName + (strlen(cFileName) - 2)), ".c") != 0)
			cFileName = textFormat("%s.c", cFileName);
		compile(inFileName, cFileName);
	} else if (ASM) {
		dir(compileFolder);

		strng cFileName = textFormat(".fltc/%s.c", outFileName);
		compile(inFileName, cFileName);
		if (fork() > 0)
			wait(NULL);
		else
			execlp("gcc", "gcc", "-S", cFileName, "-o", outFileName, NULL);
		remove(cFileName);

		rm(compileFolder);
	}
	return EXIT_SUCCESS;
}
