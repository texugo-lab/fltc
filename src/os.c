#pragma once

#ifdef _WIN32
#define OS "WIN"
#define BIN "/c/program\ files"
#endif
#ifdef __linux__
#define OS "LINUX"
#define BIN "$HOME/.local/bin"
#endif
#ifdef __APPLE__
#define OS "MACOS"
#define BIN "$HOME/local/bin"
#endif
#ifndef OS
#define OS "NONE"
#define BIN "$HOME/usr/bin"
#endif
