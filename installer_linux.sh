#!/bin/bash

# Function to check for and install required commands
check_and_install_dependency() {
    local cmd=$1
    local package=$2
    if ! command -v "$cmd" &> /dev/null; then
        echo "$cmd could not be found."
        read -p "Do you want to try and install it using your package manager? (y/n) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            if command -v apt-get &> /dev/null; then
                sudo apt-get update && sudo apt-get install -y "$package"
            elif command -v dnf &> /dev/null; then
                sudo dnf install -y "$package"
            elif command -v pacman &> /dev/null; then
                sudo pacman -Syu --noconfirm "$package"
            elif command -v zypper &> /dev/null; then
                sudo zypper install -y "$package"
            else
                echo "Could not determine package manager. Please install '$package' manually."
                exit 1
            fi
            if ! command -v "$cmd" &> /dev/null; then
                echo "Failed to install '$package'. Please install it manually."
                exit 1
            fi
        else
            echo "Installation cancelled. Please install '$package' and run the script again."
            exit 1
        fi
    fi
}

# --- Dependency Checks ---
echo "--- Checking for required tools ---"
check_and_install_dependency "md5sum" "coreutils"
check_and_install_dependency "steamcmd" "steamcmd"
check_and_install_dependency "wget" "wget"
check_and_install_dependency "unzip" "unzip"
echo "--- All required tools are present ---"
echo

# --- Find Steam and Synthetik ---
echo "--- Locating Steam and SYNTHETIK ---"
SYNTHETIK_PATH=""
STEAM_INSTALL_DIR=""

# Common Steam paths
declare -a STEAM_PATHS=(
    "$HOME/.steam/steam"
    "$HOME/.local/share/Steam"
    "$HOME/.var/app/com.valvesoftware.Steam/data/Steam" # Flatpak
)

echo "Searching for Steam installation..."
for path in "${STEAM_PATHS[@]}"; do
    if [ -d "$path" ]; then
        echo "Found potential Steam directory: $path"
        STEAM_INSTALL_DIR="$path"
        break
    fi
done

# Fallback to find command if common paths don't exist
if [ -z "$STEAM_INSTALL_DIR" ]; then
    echo "Steam not found in common locations. Searching the filesystem..."
    echo "This might take a while. Press Ctrl+C to cancel and enter the path manually."
    STEAM_INSTALL_DIR=$(find "$HOME" -name "steamapps" -type d -print -quit 2>/dev/null | sed 's/\/steamapps$//')
fi

# Manual entry if still not found
while [ -z "$STEAM_INSTALL_DIR" ] || [ ! -d "$STEAM_INSTALL_DIR" ]; do
    echo "Could not automatically find the Steam installation directory."
    read -p "Please enter the full path to your Steam directory: " STEAM_INSTALL_DIR
done

echo "Using Steam installation directory: $STEAM_INSTALL_DIR"

LIBRARY_VDF_PATH="$STEAM_INSTALL_DIR/steamapps/libraryfolders.vdf"
GAME_INSTALL_DIR=""

if [ -f "$LIBRARY_VDF_PATH" ]; then
    echo "Found libraryfolders.vdf, parsing for game libraries..."
    # Extract paths from VDF file (e.g., "path" "/path/to/library")
    LIBRARY_PATHS=($(grep -oP '"path"\\s+"\\K[^"]+' "$LIBRARY_VDF_PATH"))
    # Add the default steamapps folder as well
    LIBRARY_PATHS+=("$STEAM_INSTALL_DIR")

    for lib_path in "${LIBRARY_PATHS[@]}"; do
        # Sanitize path in case of escaped backslashes from VDF
        lib_path=$(echo "$lib_path" | sed 's/\\\\/\\/g')
        echo "Checking library: $lib_path"
        if [ -d "$lib_path/steamapps/common/Synthetik" ]; then
            GAME_INSTALL_DIR="$lib_path/steamapps/common/Synthetik"
            echo "Found SYNTHETIK installation path: $GAME_INSTALL_DIR"
            break
        fi
    done
else
    echo "Warning: libraryfolders.vdf not found. Searching only default library."
    if [ -d "$STEAM_INSTALL_DIR/steamapps/common/Synthetik" ]; then
        GAME_INSTALL_DIR="$STEAM_INSTALL_DIR/steamapps/common/Synthetik"
        echo "Found SYNTHETIK installation path: $GAME_INSTALL_DIR"
    fi
fi

if [ -z "$GAME_INSTALL_DIR" ]; then
    echo "Error: Could not find SYNTHETIK installation directory."
    exit 1
fi

SYNTHETIK_EXE_PATH="$GAME_INSTALL_DIR/SYNTHETIK.exe"
echo "--- Location search complete ---"
echo

# --- Download Organik ---
echo "--- Downloading and Extracting Mod ---"
ORGANIK_URL="https://github.com/Amyseni/organik/releases/latest/download/OrganikFull.zip"
ORGANIK_ZIP_PATH="/tmp/OrganikFull.zip"
ORGANIK_EXTRACT_PATH="/tmp/Organik"

echo "Downloading latest Organik release from $ORGANIK_URL..."
wget -q --show-progress -O "$ORGANIK_ZIP_PATH" "$ORGANIK_URL"
if [ $? -ne 0 ]; then
    echo "Failed to download Organik. Please check your internet connection."
    exit 1
fi

echo "Extracting Organik to $ORGANIK_EXTRACT_PATH..."
rm -rf "$ORGANIK_EXTRACT_PATH" # Clean up previous extraction
mkdir -p "$ORGANIK_EXTRACT_PATH"
unzip -o "$ORGANIK_ZIP_PATH" -d "$ORGANIK_EXTRACT_PATH" > /dev/null
if [ $? -ne 0 ]; then
    echo "Failed to extract Organik.zip."
    exit 1
fi
echo "--- Download complete ---"
echo

# --- MD5 Check and Beta Installation ---
echo "--- Verifying Game Version ---"
BETA_MD5="e8d2eaab3bd61fba4cba936b18e7d04b"
INSTALL_BETA=false

if [ ! -f "$SYNTHETIK_EXE_PATH" ]; then
    echo "SYNTHETIK.exe not found. Forcing beta update."
    INSTALL_BETA=true
else
    CURRENT_MD5=$(md5sum "$SYNTHETIK_EXE_PATH" | awk '{print $1}')
    echo "Current SYNTHETIK.exe MD5: $CURRENT_MD5"
    echo "Required Beta MD5:         $BETA_MD5"
    if [ "$CURRENT_MD5" != "$BETA_MD5" ]; then
        echo "MD5 mismatch. SYNTHETIK needs to be updated to the beta version."
        INSTALL_BETA=true
    else
        echo "SYNTHETIK executable matches the beta version. No update needed."
    fi
fi

if [ "$INSTALL_BETA" = true ]; then
    echo "--- Installing SYNTHETIK Beta Branch ---"
    read -p "Please enter your Steam username (required for beta branch installation): " STEAM_USERNAME
    
    echo "Running steamcmd to update SYNTHETIK. This may take a few minutes..."
    steamcmd +login "$STEAM_USERNAME" +app_update 528230 -beta beta validate +quit

    # Verify MD5 after update
    if [ ! -f "$SYNTHETIK_EXE_PATH" ]; then
        echo "Error: SYNTHETIK.exe still not found after update attempt."
        exit 1
    fi
    
    UPDATED_MD5=$(md5sum "$SYNTHETIK_EXE_PATH" | awk '{print $1}')
    if [ "$UPDATED_MD5" != "$BETA_MD5" ]; then
        echo "Error: Failed to update SYNTHETIK to the beta branch."
        echo "Please try setting the beta branch manually in Steam:"
        echo "SYNTHETIK (right click) -> Properties -> Betas -> beta - 2.3.7"
        exit 1
    fi
    echo "SYNTHETIK updated to the beta branch successfully."
fi
echo "--- Verification complete ---"
echo

# --- Install Mod Files ---
echo "--- Installing Mod ---"
SYNTHETIK_BACKUP_PATH="$GAME_INSTALL_DIR/SYNTHETIK.exe.bak"
echo "Backing up SYNTHETIK.exe to $SYNTHETIK_BACKUP_PATH..."
cp "$SYNTHETIK_EXE_PATH" "$SYNTHETIK_BACKUP_PATH"

echo "Copying Organik files to $GAME_INSTALL_DIR..."
cp -r "$ORGANIK_EXTRACT_PATH"/* "$GAME_INSTALL_DIR/"

echo "Renaming SYNTHETIK.exe to game.exe..."
mv "$SYNTHETIK_EXE_PATH" "$GAME_INSTALL_DIR/game.exe"

AURIE_LOADER_PATH="$GAME_INSTALL_DIR/AurieLoader-x86.exe"
if [ -f "$AURIE_LOADER_PATH" ]; then
    echo "Renaming AurieLoader-x86.exe to SYNTHETIK.exe..."
    mv "$AURIE_LOADER_PATH" "$SYNTHETIK_EXE_PATH"
else
    echo "Warning: AurieLoader-x86.exe not found in mod files. The mod may not launch correctly."
fi

echo "--- Installation Complete! ---"
echo "You can now run SYNTHETIK through Steam as usual."
echo "To uninstall, delete the installed files and restore SYNTHETIK.exe from SYNTHETIK.exe.bak"
