#!/usr/bin/bash

# Default build folder name
build_folder="build"
unset fresh
# Parse command line options
while getopts ":fb:" opt; do
    case $opt in
        b)
            build_folder="$OPTARG"
            ;;
        f)
            fresh="--fresh"
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

# Create the build folder if it doesn't exist
if [ ! -d "$build_folder" ]; then
    echo "Creating build folder: $build_folder"
    mkdir "$build_folder"
fi

name=$(./scripts/query_manifest.sh name)
version=$(./scripts/query_manifest.sh version)

# Run CMake inside the build folder
(
    cd "$build_folder" || exit 1

    CC=cl CXX=cl cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DPROJ_NAME=$name \
        -DPROJECT_VERSION=$version \
        .. $fresh

    # Check the exit code of the CMake command
    if [ $? -eq 0 ]; then
        echo "CMake configuration successful. Build folder: $PWD"
    else
        echo "CMake configuration failed."
    fi
)
