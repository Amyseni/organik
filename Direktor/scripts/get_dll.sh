#!/bin/bash

# Check if a folder path is provided as an argument
if [ $# -eq 0 ]; then
  echo "Usage: $0 <folder_path>"
  exit 1
fi

folder_path="$1"

# Check if the provided path is a directory
if [ ! -d "$folder_path" ]; then
  echo "Error: '$folder_path' is not a valid directory."
  exit 1
fi

# Find the first DLL in the specified folder
dll_path=$(find "$folder_path" -name '*.dll' -type f -print -quit)

if [ -z "$dll_path" ]; then
  echo "No DLL files found in '$folder_path'."
  exit 1
fi

# Print the full relative path of the first DLL
echo "$dll_path"
