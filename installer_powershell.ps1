# get steam install directory from the registry
# check if the first argument is a path to SYNTHETIK.exe
$synthetikExePath = ""
$gameInstallDir = ""
$process = Get-Process -Name "steam" -ErrorAction SilentlyContinue

Add-Type -AssemblyName System.Windows.Forms

$steamInstallDir = (Get-ItemPropertyValue -Path "Registry::HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam" -Name "InstallPath")
if (-not $steamInstallDir) {
    $steamInstallDir = (Get-ItemPropertyValue -Path "Registry::HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Steam" -Name "InstallPath")
}
if (-not $steamInstallDir) {
    Write-Host "Steam installation directory not found in registry."
    $steamInstallDir = "C:\Program Files (x86)\Steam"
}
$synthetikExePath = ""
while (-not (Test-Path -Path $steamInstallDir) -and -not (Test-Path -Path $synthetikExePath)) {
    Write-Host "Couldn't find Steam."
    Write-Host "Please specify the location of SYNTHETIK.exe (e.g. steamapps/common/Synthetik/SYNTHETIK.exe)"
    $FileBrowser = New-Object System.Windows.Forms.OpenFileDialog -Property @{ Title = "Select SYNTHETIK.exe -- ('Browse Local Files' on Steam)"; Filter = "Executable Files (*.exe)|*.exe" }
    if ($FileBrowser.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
        $synthetikExePath = $FileBrowser.FileName
        $steamInstallDir = Split-Path -Path $synthetikExePath -Parent | Split-Path -Parent
    }
}

if (Test-Path -Path $synthetikExePath) {
    Write-Host "Using game install: $synthetikExePath"
}
else {
    Write-Host "Using Steam installation directory: $steamInstallDir"
    # get the path to Synthetik executable
    $libraryFoldersVDF = Join-Path -Path $steamInstallDir -ChildPath "steamapps/libraryfolders.vdf"
    if (Test-Path -Path $libraryFoldersVDF) {
        $libraryFoldersContent = Get-Content -Path $libraryFoldersVDF -Raw

        # find all library folders in the file
        $libraryFolders = [regex]::Matches($libraryFoldersContent, '"path"\s+"([^"]+)"')
        Write-Host "Found $($libraryFolders.Count) library folders in $libraryFoldersVDF"
        if ($libraryFolders.Count -gt 0) {
            foreach ($folder in $libraryFolders) {
                $libraryFolderPath = $folder.Groups[1].Value
                Write-Host "Checking $libraryFolderPath"
                if (Test-Path -Path $libraryFolderPath) {
                    $gameInstallDir = Join-Path -Path $libraryFolderPath -ChildPath "steamapps/common/Synthetik"
                    if (Test-Path -Path $gameInstallDir) {
                        Write-Host "Found SYNTHETIK installation path: $gameInstallDir"
                        break
                    }
                }
            }
        }
    }
}
if (-not (Test-Path -Path $synthetikExePath) -and (Test-Path -Path $gameInstallDir)) {
    $synthetikExePath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe"
    exit 1
}
else if (-not (Test-Path -Path $gameInstallDir)) {
    $gameInstallDir = Split-Path -Path $synthetikExePath -Parent
}
else
{
    Write-Host "Something went wrong trying to find the game installation directory. If this problem persists,"
    Write-Host "Please come find me on Discord: https://discord.gg/KXPgRzSpBY @amyseni for assistsance"
    exit 1
}

$synthetikExePath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe"
$currentExeHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
$currentMd5 = ""
if ($currentExeHash) {
    $currentMd5 = $currentExeHash.Hash.ToLower()
}

$installOk = $false
$betaMd5 = "e8d2eaab3bd61fba4cba936b18e7d04b"
# Check for existing installation
$gameExePath = Join-Path -Path $gameInstallDir -ChildPath "game.exe"
$dinput8Path = Join-Path -Path $gameInstallDir -ChildPath "dinput8.dll"
$modsPath = Join-Path -Path $gameInstallDir -ChildPath "mods"
$existingInstallation = (Test-Path -Path $gameExePath -and) -and (Test-Path -Path $modsPath)

if (Test-Path -Path $gameExePath -or (Test-Path -Path $gameInstallDir -ChildPath "mods/aurie")) {
    Write-Host "Detected existing mod installation (YYTK build, game.exe present)."
    if (-not (Test-Path -Path $dinput8Path)) {
        Write-Host "No conflicting new install found. Okay to upgrade to latest Organik version?"
        if (MessageBox-Show("Upgrade from (old) YYTK Organik build to (latest) ZHL version?", "Confirm Upgrade", "OKCancel", "Question") -ne "OK") {
            Write-Host "Installation cancelled by user."
            exit 0
        }
        Write-Host "93 Remove-Item -Path $path $gameInstallDir -ChildPath \"mods\" -Recurse -Force -ErrorAction SilentlyContinue"
        Write-Host "94 Remove-Item -Path $gameInstallDir -ChildPath \"SYNTHETIK.exe\" -Force -ErrorAction SilentlyContinue"
        Write-Host "95 Move-Item -Path $gameExePath -Destination $synthetikExePath -Force *> $null"
    }
    else {
        Write-Host "WARNING: conflicting mod versions detected (dinput8.dll present)."
        $existingInstallation = $true
    }
}
if ($existingInstallation) {
    if (MessageBox-Show("Existing YYTK installation of the mod detected. Please confirm ONLY if you want to remove any existing mod files and reinstall from scratch.", "Warning", "OKCancel", "Warning") -ne "OK") {
        Write-Host "Installation cancelled by user."
        exit 0
    }
    if (MessageBox-Show("This will DELETE any existing mod files and request for steam to verify your local files. Potentially reinstalling the entire game. CLICK OKAY TO PROCEED WITH FORCED REINSTALL.", "Final Warning", "OKCancel", "Warning") -ne "OK") {
        Write-Host "Installation cancelled by user."
        exit 0
    }

    Write-Host "Removing existing mod files..."
    Write-Host "113 Remove-Item -Path $path $gameInstallDir -ChildPath \"mods\" -Recurse -Force -ErrorAction SilentlyContinue"
    Write-Host "114 Remove-Item -Path $gameInstallDir -ChildPath \"SYNTHETIK.exe\" -Force -ErrorAction SilentlyContinue"
    Write-Host "115 Move-Item -Path $gameExePath -Destination $synthetikExePath -Force *> $null"
    Write-Host "116 Remove-Item -Path $gameInstallDir -ChildPath \"dinput8.dll\" -Force -ErrorAction SilentlyContinue"
    Write-Host "Requesting Steam to verify local files..."
    Write-Host "118 Start-Process \"steam://validate/528230\""
    Write-Host "Waiting for Steam to finish verifying files..."
    While ( 1 ) {
        Start-Sleep -Seconds 5
        $currentExeHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
        if ($currentExeHash) {
            $currentMd5 = $currentExeHash.Hash.ToLower()
        }
        if ($currentMd5 -eq $betaMd5) {
            Write-Host "SYNTHETIK beta version detected successfully!"
            $installOk = $true
            break
        }
    }
}
$synthetikExePath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe"
$currentExeHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
$currentMd5 = ""
if ($currentExeHash) {
    $currentMd5 = $currentExeHash.Hash.ToLower()
}

if ($existingInstallation -or -not (Test-Path -Path $synthetikExePath) -or -not ($currentMd5 -eq $betaMd5)) {
    While ( 1 )
    {
        Start-Sleep -Seconds 10

        $currentExeHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
        $currentMd5 = ""
        if ($currentExeHash) {
            $currentMd5 = $currentExeHash.Hash.ToLower()
        }
        if (Test-Path -Path $gameExePath -and) -and -not ($currentMd5 -eq $betaMd5) {
            Write-Host "SYNTHETIK nonbeta version detected."
            Start-Process "steam://gameproperties/528230/betas"
            MessageBox-Show("After verifying local files, non-beta install was identified. Opening Steam game properties... Please select '2.3.7' from the dropdown menu.", "Info", "OK", "Info")
            Write-Host "Waiting for user to select beta branch in Steam..."
            While ( 1 )
            {
                Start-Sleep -Seconds 3
                $currentExeHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
                $currentMd5 = ""
                if ($currentExeHash) {
                    $currentMd5 = $currentExeHash.Hash.ToLower()
                }
                if ($currentMd5 -eq $betaMd5) {
                    Write-Host "SYNTHETIK beta version detected successfully!"
                    $installOk = $true
                    break
                }
            }
            if (!$installOk) {
                Write-Host "Failed to detect SYNTHETIK beta version after multiple attempts."
                Write-Host "Please ensure you've selected 'beta - 2.3.7' in Steam and the game has finished updating."
                Write-Host "You can try running this installer again after the update is complete."
                Write-Host "For support, please join our Discord: https://discord.gg/KXPgRzSpBY"
                exit 1
            }
        }
    }
}

Write-Host "STEUP OK??"
Write-Host "SYNTHETIK.exe path: $synthetikExePath"
Write-Host "Game install dir: $gameInstallDir"
Write-Host "Current SYNTHETIK.exe MD5: $currentMd5"

# Get current SYNTHETIK.exe hash
$synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
$synthetikMd5 = ""
if ($synthetikHash) {
    $synthetikMd5 = $synthetikHash.Hash.ToLower()
}

# download the latest version of organik from GitHub
Write-Host "************************************************************"
Write-Host "* Welcome to the Organik Installer for SYNTHETIK           *"
Write-Host "* This installer will guide you through the installation   *"
Write-Host "* process and ensure you have the correct version of       *"
Write-Host "* SYNTHETIK installed.                                     *"
Write-Host "*                                                          *"
Write-Host "* Please note that this installer requires an active       *"
Write-Host "* internet connection to download the necessary files.     *"
Write-Host "*                                                          *"
Write-Host "* If you encounter any issues during the installation,     *"
Write-Host "* please reach out to us on Discord for support:           *"
Write-Host "* https://discord.gg/KXPgRzSpBY                            *"
Write-Host "************************************************************"
Write-Host ""
Write-Host "FAKE-INSTALL MODE. This is a test run and no files will be permanently installed."

$organikUrl = "https://github.com/Amyseni/organik/releases/latest/download/OrganikFull.zip"
$organikZipPath = Join-Path -Path $env:TEMP -ChildPath "OrganikFull.zip"
Write-Host "Downloading Organik from $organikUrl..."
Invoke-WebRequest -Uri $organikUrl -OutFile $organikZipPath -ErrorAction Stop

# check if the download was successful
if (-not (Test-Path -Path $organikZipPath)) {
    Write-Host "Failed to download Organik. Please check your internet connection."
    Write-Host "If issues persist, please contact us on Discord: https://discord.gg/KXPgRzSpBY"
    exit 1
}
# extract the downloaded zip file to temp path
$organikExtractPath = Join-Path -Path $env:TEMP -ChildPath "Organik"
if (-not (Test-Path -Path $organikExtractPath)) {
    Write-Host "Creating temporary Organik extraction directory at $organikExtractPath..."
    New-Item -Path $organikExtractPath -ItemType Directory -Force *> $null
}
Write-Host "Extracting Organik to $organikExtractPath..."
Expand-Archive -Path $organikZipPath -DestinationPath $organikExtractPath -Force *> $null

# check if Organik appdata directory exists
$organikAppdata = Join-Path -Path $env:APPDATA -ChildPath "Organik"
if (-not (Test-Path -Path $organikAppdata)) {
    Write-Host "Creating Organik appdata directory at $organikAppdata..."
    New-Item -Path $organikAppdata -ItemType Directory -Force *> $null
}

# create the steamcmd directory inside the Organik appdata directory
$steamCmdAppdataPath = Join-Path -Path $organikAppdata -ChildPath $steamCmdExePath
if (-not (Test-Path -Path $steamCmdAppdataPath)) {
    Write-Host "Creating steamcmd directory at $steamCmdAppdataPath..."
    New-Item -Path $steamCmdAppdataPath -ItemType Directory -Force *> $null
}

# check if there is a Launcher.exe in the temp directory
$launcherExePath = Join-Path -Path $organikExtractPath -ChildPath "AurieLoader-x86.exe"
$isZHLVersion = $false
if (-not (Test-Path -Path $launcherExePath)) {
    Write-Host "Launcher.exe not included with Organik. Assuming this is a LibZHL version."
    $isZHLVersion = $true
}

# Check if dinput8.dll exists (indicator of existing mod)
if (Test-Path -Path $dinput8Path) {
    Write-Host "Warning: dinput8.dll found in the game directory. The mod seems to be already installed."
    $forceInstall = Read-Host "Do you want to force the installation anyway? (y/n)"
    if ($forceInstall -ne "y") {
        Write-Host "Installation cancelled."
        exit 0
    }
    Write-Host "Proceeding with force installation..."
}
# Check if we need to install beta
if (-not (Test-Path -Path $synthetikExePath) -or ($synthetikMd5 -ne $betaMd5)) {
    Write-Host "SYNTHETIK beta version (2.3.7) is required for this mod."
    Write-Host "A Steam window will open. Please select 'beta - 2.3.7' from the dropdown menu."
    Write-Host "Press Enter to continue..."
    Read-Host | Out-Null
    
    # Open Steam beta properties
    Start-Process "steam://gameproperties/528230/betas"
    
    Write-Host "Waiting for you to select the beta branch in Steam..."
    Write-Host "After selecting the beta branch, Steam will update the game automatically."
    Write-Host "This script will check periodically if the update is complete."
    
    $attempts = 0
    $maxAttempts = 30
    $found = $false
    
    while (-not $found -and $attempts -lt $maxAttempts) {
        $attempts++
        Write-Host "Checking if SYNTHETIK beta is installed... (Attempt $attempts/$maxAttempts)"
        
        # Check if the file exists and has the correct MD5
        if (Test-Path -Path $synthetikExePath) {
            $currentHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5).Hash.ToLower()
            if ($currentHash -eq $betaMd5) {
                $found = $true
                Write-Host "SYNTHETIK beta version detected successfully!"
            }
        }
        
        if (-not $found) {
            Write-Host "Beta not detected yet. Waiting 10 seconds before checking again..."
            Start-Sleep -Seconds 10
        }
    }
    
    if (-not $found) {
        Write-Host "Failed to detect SYNTHETIK beta version after multiple attempts."
        Write-Host "Please ensure you've selected 'beta - 2.3.7' in Steam and the game has finished updating."
        Write-Host "You can try running this installer again after the update is complete."
        Write-Host "For support, please join our Discord: https://discord.gg/KXPgRzSpBY"
        exit 1
    }
}

# Handle existing installation with game.exe but no dinput8.dll
if ($existingInstallation -and (-not (Test-Path -Path $dinput8Path))) {
    Write-Host "Detected partial mod installation. Restoring original SYNTHETIK.exe..."
    Copy-Item -Path $gameExePath -Destination $synthetikExePath -Force
}

# backup SYNTHETIK.exe
$synthetikBackupPath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe.bak"
Write-Host "Backing up SYNTHETIK.exe to $synthetikBackupPath..."
Copy-Item -Path $synthetikExePath -Destination $synthetikBackupPath -Force

# copy the contents of the temporary Organik directory to the defined gameInstallDir
Write-Host "Copying Organik files to $gameInstallDir..."

# Copy directly to game root folder
Get-ChildItem -Path $organikExtractPath | Copy-Item -Destination $gameInstallDir -Recurse -Force

Write-Host "Organik files copied successfully."
Write-Host "Renaming SYNTHETIK.exe to game.exe"
Move-Item -Path $synthetikExePath -Destination $gameExePath -Force *> $null

# Rename AurieLoader-x86.exe to SYNTHETIK.exe
$aurieLoaderPath = Join-Path -Path $gameInstallDir -ChildPath "AurieLoader-x86.exe"
Write-Host "Renaming AurieLoader-x86.exe to SYNTHETIK.exe"
Move-Item -Path $aurieLoaderPath -Destination $synthetikExePath -Force

Write-Host "Installation completed successfully!"
Write-Host "You can now start the game through Steam as usual."