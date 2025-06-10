#!/bin/bash

# Default manifest file path
manifest_file="manifest.json"

# Parse command line options
while getopts ":m:" opt; do
    case $opt in
        m)
            manifest_file="$OPTARG"
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

# Shift the option arguments so that $1 refers to the first non-option argument
shift $((OPTIND-1))

# Check if the key argument is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 [-m manifest_path] <key>"
    exit 1
fi

# Check if the manifest file exists
if [ ! -f "$manifest_file" ]; then
    echo "Error: $manifest_file not found."
    exit 1
fi

# Read the value corresponding to the provided key
key="$1"
value=$(grep -E "\"$key\":\s*\"?[^\"]+\"?" "$manifest_file" | sed -E 's/.*"'"$key"'":\s*"([^"]+)".*/\1/')

# Check if the key exists in the manifest
if [ -z "$value" ]; then
    echo "Error: Key '$key' not found in $manifest_file."
    exit 1
fi

# Print the value
echo "$value"
