#!/usr/bin/env bash

os="$OSTYPE"

fileName="compiler.c"
executableName="fltc"
params="-Wall -Wextra -g $(pkg-config --libs lua5.4)"
location="./"

case "$OSTYPE" in
	solaris*) export os="solaris" ;;
	darwin*)  export os="macos" ;;
	linux*)   export os="linux" ;;
	bsd*)     export os="bsd" ;;
	msys*)    export os="win" ;;
	cygwin*)  export os="win" ;;
	*)        export os="unknown" ;;
esac

command mkdir ~/.config/fltc/ >/dev/null 2>&1 && echo "✅ Config Generated at $HOME/.config/fltc/" || echo "✅ Config Found at $HOME/.config/fltc/"
command which gcc >/dev/null 2>&1 && export GCC="true" || export GCC="false"
if [ "$os" == "linux" ]; then
	if [ "$GCC" == "false" ]; then
		echo "❌ GCC not found"
		sudo apt install gcc
	elif [ "$GCC" == "true" ]; then
		echo "✅ GCC found"
	fi
	location="/usr/local/bin"
	sudo gcc $fileName $params -o $location/$executableName
elif [ "$os" == "win" ]; then
	if [ "$GCC" == "false" ]; then
		echo "❌ GCC not found"
		echo "- Install GCC to be able to Install FLTC"
	elif [ "$GCC" == "true" ]; then
		echo "✅ GCC found"
		location="$HOME/bin"
		sudo gcc $fileName $params -o $location/$executableName.exe
	fi
elif [ "$os" == "macos" ]; then
	location="/usr/local/bin"
	sudo gcc $fileName $params -o $location/$executableName.app
fi
