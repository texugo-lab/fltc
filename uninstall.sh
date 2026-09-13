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
	location="/usr/local/bin"
	sudo rm -rf $location/$executableName
elif [ "$os" == "win" ]; then
	location="$HOME/bin"
	sudo rm -rf $location/$executableName
elif [ "$os" == "macos" ]; then
	location="/usr/local/bin"
	sudo rm -rf $location/$executableName
fi
