#include <sys/wait.h>

// LUA
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>

#include "src/essentials.h"
#include "src/executer.c"

#define HELP                                            \
	"Correct Usage:\n"                                  \
	"    fltc -i <InputFile> -o <OutputFile> [Flags]\n" \
	"Flags:\n"                                          \
	"    -c             -> Generate C File\n"           \
	"    -asm           -> Generate Assembly File\n"    \
	"\n"                                                \
	"    -s | --script  -> Run in script mode\n"        \
	"    -h | --help    -> Print this documentation\n"  \
	"    -v | --version -> Print current fltc version\n"
#define VERSION "1.0.1\n"

void dir(strng dirName);
void touch(strng name);
void rm(strng name);
void run(int argc, strng argv[], bool running);

strng flags[] = {"-s", "--script", "-i", "-o", "-asm", "-c", "-h", "--help", "-v", "--version"};

int main(int argc, strng argv[]) {
	// CHECK GCC
	FILE* fp = popen("which gcc", "r");
	char path[1035];
	if (fgets(path, sizeof(path), fp) == NULL) {
		printf("gcc not found!\n");
		exit(EXIT_FAILURE);
	}
	pclose(fp);

	// FORMAT ARGV
	strng args[argc - 1];
	for (int i = 1; i < argc; ++i) {
		args[i - 1] = strdup(argv[i]);
	}
	run(argc - 1, args, false);

	return EXIT_SUCCESS;
}

void dir(strng dirName) {
	if (fork()) {
		wait(NULL);
	} else {
		execlp("mkdir", "mkdir", dirName, NULL);
	}
}
void touch(strng name) {
	if (fork()) {
		wait(NULL);
	} else {
		execlp("touch", "touch", name, NULL);
	}
}
void rm(strng name) {
	if (fork()) {
		wait(NULL);
	} else {
		execlp("rm", "rm", "-rf", name, NULL);
	}
}

void run(int argc, strng argv[], bool running) {
	strng inFileName = "\0";
	strng outFileName = "\0";
	strng compileFolder = ".fltc";

	bool hasInput = false;
	bool hasOutput = false;

	int mode = 0;

	for (int i = 0; i < argc; ++i) {
		bool flagExists = false;

		for (size_t j = 0; j < sizeof(flags) / sizeof(flags[0]); ++j) {
			if (streq(flags[j], argv[i])) {
				flagExists = true;
			}
		}
		if (!flagExists) {
			printf("Unknown Flag:\n\t%s\n", argv[i]);
			exit(EXIT_SUCCESS);
		}

		if ((streq(argv[i], "-s") || streq(argv[i], "--script"))) {
			if (running == true) {
				printf("Cannot run script mode in script mode\n");
				exit(EXIT_FAILURE);
			}
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			if (luaL_loadfile(L, ".fltc.lua") || lua_pcall(L, 0, 0, 0)) {
				printf("Could not open '.fltc.lua':\n%s\n", lua_tostring(L, -1));
				exit(EXIT_FAILURE);
			}

			lua_getglobal(L, "flt_args");
			lua_len(L, -1);
			lua_Integer len = lua_tointeger(L, -1);
			lua_pop(L, 1);

			strng args[len];

			for (int i = 0; i < len; ++i) {
				lua_rawgeti(L, -1, i + 1);
				args[i] = (strng)lua_tostring(L, -1);
				lua_pop(L, 1);
			}

			lua_close(L);

			run(len, args, true);
			return;
		}
		if (streq(argv[i], "-i")) {
			inFileName = argv[i + 1];
			++i;
			hasInput = true;
			continue;
		} else if (streq(argv[i], "-o")) {
			outFileName = argv[i + 1];
			++i;
			hasOutput = true;
			continue;
		}

		else if (str2eq(argv[i], "-h", "--help")) {
			printf(HELP);
			exit(EXIT_SUCCESS);
		} else if (str2eq(argv[i], "-v", "--version")) {
			printf(VERSION);
			exit(EXIT_SUCCESS);
		}

		else if (streq(argv[i], "-c")) {
			mode = 1;
		} else if (streq(argv[i], "-asm")) {
			mode = 2;
		}
	}
	if (!hasInput || !hasOutput) {
		printf(HELP);
		exit(EXIT_SUCCESS);
	}
	if (mode == 0) {
		dir(compileFolder);

		strng cFileName = textFormat(".fltc/%s.c", outFileName);
		compile(inFileName, cFileName);
		if (fork() > 0)
			wait(NULL);
		else
			execlp("gcc", "gcc", cFileName, "-o", outFileName, NULL);
		remove(cFileName);

		rm(compileFolder);
	} else if (mode == 1) {
		strng cFileName = textFormat("%s", outFileName);
		if (streq((cFileName + (strlen(cFileName) - 2)), ".c"))
			cFileName = textFormat("%s.c", cFileName);
		compile(inFileName, cFileName);
	} else if (mode == 2) {
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
}
