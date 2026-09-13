#pragma once

#ifdef _WIN32
#define OS "WIN"
#define CONFIG "~/.config/fltc/"
#endif
#ifdef __linux__
#define OS "LINUX"
#define CONFIG "~/.config/fltc/"
#endif
#ifdef __APPLE__
#define OS "MACOS"
#define CONFIG "~/.config/fltc/"
#endif
#ifndef OS
#define OS "NONE"
#define CONFIG "~/.config/fltc/"
#endif
