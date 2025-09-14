#!/usr/bin/bash

orgDir="$PWD"

# Default settings

build_folder="build"
unset clean
moveOutput() {
	#move output from "$1/x86/Release to $STEAMAPPS/common/Synthetik" for further processing
	if test -d "$1/x86/Release"; then
		cd "$1/x86/Release"
		if test -f Direktor.dll; then
			cp Direktor.dll "$STEAMAPPS/common/Synthetik/NativeMods/"
			echo "Copied Direktor.dll from $1";
		else
			if test -f *.dll; then 
				cp *.dll "$STEAMAPPS/common/Synthetik/NativeMods/"; 
				echo "Copied dlls from $1"
			fi
		fi
	fi
	cd "$orgDir"
}


quitErr() {
	echo "ERROR: "
	errNo="$1"
	cln="$2"
	SCPT="$3"
	LNE="$4"
	shift 4
	targets="$@"
	echo ">		ERRNO: $errNo"
	echo ">		DIR: $PWD"
	echo ">		CLEAN: $cln"
	echo ">		TARGETS: $targets"
	echo ">		SCRIPT: $SCPT"
	echo ">		LINE: $LNE"
}
doBuild() {
	echo "$@"
	targetName="$1"
	shift 1
	trget="$@"
	cd "$trget"
	unset f
	if [ "$clean" = "true" ]; then rm -rf x86; rm -rf x64; f="--fresh"; fi
	trap 'quitErr "$?" "$clean" "${BASH_SOURCE}" "${LINENO}" "$targets"' ERR
	CC=cl CXX=cl fresh="$f" ./build.sh # > "$targetName.buildlog"
	moveOutput "$trget"
}

# Parse command line options
while getopts "cb:t:" opt; do
    case $opt in
		b)
            build_folder="$OPTARG"
            ;;
        t)
        	targets="$OPTARG"
        	;;
        c)
            clean="true"
            ;;

        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

if [ -z "$targets" ] || [[ "$targets" = "all" ]]; then
	for p in $(find "$orgDir" -mindepth 2 -maxdepth 3 -iregex ".*\/setup_cmake\\.sh" -printf "%h/%f\\n"); do
		cd "$orgDir"
		p="${p/\/setup_cmake.sh/}"
		t="${p##*/}"
		echo "Processing target $t"
		if test -f "$p/build.sh"; then 
			doBuild "$t" "$p"
		else
			echo "$p" 
		fi
	done
else
	for t in ${targets//,/ }; do 
		echo "Processing target $t"
		for p in $(find "$orgDir" -mindepth 1 -maxdepth 2 -type d -iregex ".*$t" -printf "%h/%f\\n"); do
			cd "$orgDir"
			if test -f "$p/build.sh"; then 
				doBuild "$t" "$p"
			fi
		done
	done
fi
echo eof