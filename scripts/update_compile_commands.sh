#!/bin/bash

# Usage: ./update_compile_commands.sh -f SECRETS_FILE [-j JSON_FILE]

while getopts ":f:j:" opt; do
  case $opt in
    f) secrets_file="$OPTARG" ;;
    j) json_file="$OPTARG" ;;
    \?) echo "Invalid option: -$OPTARG" >&2; exit 1 ;;
    :) echo "Option -$OPTARG requires an argument." >&2; exit 1 ;;
  esac
done

# Check if the required secrets file is provided
if [[ -z "$secrets_file" ]]; then
  echo "Usage: $0 -f SECRETS_FILE [-j JSON_FILE]"
  exit 1
fi

# Check if the secrets file exists
if [[ ! -f "$secrets_file" ]]; then
  echo "Secrets file not found: $secrets_file"
  exit 1
fi

# If JSON file is not provided, search for it in the current directory
if [[ -z "$json_file" ]]; then
  json_file=$(find . -name "compile_commands.json" -type f -print -quit)
fi

# Check if the JSON file exists
if [[ -z "$json_file" ]]; then
  echo "No compile_commands.json file found in the current directory."
  exit 1
fi

# Read old and new compiler paths from the secrets file
source "$secrets_file"

# Use sed to replace the old compiler with the new one in the JSON file
sed -i "s|$old_compiler|$new_compiler|g" "$json_file"

# Fix the compiler flags
sed -i "s|-std:c++latest|-std=gnu++latest|g" "$json_file"
sed -i "s|-MD /GL /Oi /Gy /permissive- /sdl /W3 /Zi /EHsc /FoCMakeFiles|-o CMakeFiles|g" "$json_file"
sed -i "s|/FdCMakeFiles/Src.dir/ /FS -c|-c|g" "$json_file"
