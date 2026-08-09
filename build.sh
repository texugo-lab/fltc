os="$OSTYPE"

fileName="compiler.c"
executableName="fltc"
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

if [ "$os" == "linux" ]; then
	location="$HOME/.local/bin/"
	gcc $fileName -Wall -Wextra -o $location$executableName
elif [ "$os" == "win" ]; then
	location="/c/program\ files"
	gcc $fileName -Wall -Wextra -o $location$executableName.exe
elif [ "$os" == "macos" ]; then
	location="$HOME/.local/bin"
	gcc $fileName -Wall -Wextra -o $location$executableName.app
fi
