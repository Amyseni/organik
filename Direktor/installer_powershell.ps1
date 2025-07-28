# get steam install directory from the registry
# check if the first argument is a path to SYNTHETIK.exe
$synthetikExePath = ""
$gameInstallDir = ""
$process = Get-Process -Name "steam" -ErrorAction SilentlyContinue

if ($process) {
    Write-Host "Steam is running. If it does not close automatically, please close it manually."
    Start-Process -FilePath "taskkill.exe" -ArgumentList "/F /IM steam.exe" -NoNewWindow -Wait -PassThru
    $loops = 0
    $maxLoops = 10
    $TimeoutSeconds = 5
    while (-not $process.HasExited -and $loops -lt $maxLoops) {
        Write-Host "Waiting for Steam to close... ($($loops + 1)/$maxLoops)"
        $loops++
        Start-Sleep -Seconds $TimeoutSeconds
        $process.Refresh() # Refresh the process object to get updated status
    }
    if (-not $process.HasExited) {
        Write-Host "Steam did not close after $($loops * $TimeoutSeconds) seconds. Please close Steam and try again."
        exit 1
    } else {
        Write-Host "Steam closed successfully."
    }
}

$steamInstallDir = (Get-ItemPropertyValue -Path "Registry::HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam" -Name "InstallPath")
if (-not $steamInstallDir) {
    $steamInstallDir = (Get-ItemPropertyValue -Path "Registry::HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Steam" -Name "InstallPath")
}
if (-not $steamInstallDir) {
    Write-Host "Steam installation directory not found in registry."
    $steamInstallDir = "C:\Program Files (x86)\Steam"
}

while (-not (Test-Path -Path $steamInstallDir)) {
    Write-Host "Couldn't find Steam."
    Write-Host "Please specify the path to your Steam installation directory (e.g C:\Program Files (x86)\Steam)"
    $steamInstallDir = Read-Host " > "
}

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

$synthetikExePath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe"

# download the latest version of organik from GitHub
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

$synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
$synthetikMd5 = ""
if (-not $synthetikHash) {
    Write-Host "SYNTHETIK executable not found in game directory. Forcing beta update."
    $installBeta = $true
} else {
    $synthetikMd5 = $synthetikHash.Hash.ToLower()
}
$betaMd5 = "e8d2eaab3bd61fba4cba936b18e7d04b"

if (-not $isZHLVersion) {
    # check if SYNTHETIK.EXE has been replaced with the launcher already.
    if (-not $installBeta) {
        if (($synthetikMd5 -ne $expectedMd5Launcher)) {
            $expectedMd5Launcher = (Get-FileHash -Path $launcherExePath -Algorithm MD5).Hash.ToLower()
            Write-Host "Mod not already installed. Performing full installation."
            
            #check beta against installed version.
            if ($synthetikMd5 -ne $betaMd5) {
                Write-Host "SYNTHETIK executable does not match the beta version. Installing beta..."
                $installBeta = $true
            } else {
                Write-Host "SYNTHETIK executable matches the beta version. No need to install beta."
            }
        } else {
            Write-Host "Found matching launcher, updating mods folder..."
            # copy the mods folder from the extracted Organik to the install path
            $modsSourcePath = Join-Path -Path $organikExtractPath -ChildPath "mods"
            $modsDestPath = $gameInstallDir
            Copy-Item -Path $modsSourcePath -Destination $modsDestPath -Recurse -Force
            Write-Host "Mods folder updated successfully."
            exit 0
        }
    }
}
if ((Test-Path -Path $synthetikExePath) -and ($synthetikMd5 -ne $betaMd5)) {
    $installBeta = $true
}
if ($installBeta) {
    # check if steamcmd is already installed
    $steamCmdExePath = Join-Path -Path $steamCmdAppdataPath -ChildPath "steamcmd.exe"
    if (-not (Test-Path -Path $steamCmdExePath)) {
        # Download steamcmd
        $steamCmdUrl = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip"
        $steamCmdZipPath = Join-Path -Path $env:TEMP -ChildPath "steamcmd.zip"
        Write-Host "Downloading steamcmd from $steamCmdUrl..."
        Invoke-WebRequest -Uri $steamCmdUrl -OutFile $steamCmdZipPath -ErrorAction Stop *> $null

        if (-not (Test-Path -Path $steamCmdZipPath)) {
            Write-Host "Failed to download steamcmd. Please check your internet connection."
            Write-Host "If issues persist, please contact us on Discord: https://discord.gg/KXPgRzSpBY"
            exit 1
        }

        Write-Host "Extracting steamcmd to $steamCmdAppdataPath..."
        Expand-Archive -Path $steamCmdZipPath -DestinationPath $steamCmdAppdataPath *> $null
        
        # check if steamcmd.exe exists
        if (-not (Test-Path -Path $steamCmdExePath)) {
            Write-Host "steamcmd.exe not found in $steamCmdAppdataPath. Something went wrong during the download or extraction."
            Write-Host "Please contact us on Discord: https://discord.gg/KXPgRzSpBY"
            exit 1
        }
    }
    # run steamcmd to set SYNTHETIK to the beta branch
    Read-Host "Please enter your Steam username (required for beta branch installation)" -OutVariable steamUsername
    $steamCmdArgs = @(
        "+login $($steamUsername)",
        "+app_update 528230 -beta beta validate",
        "+exit"
    )
    $steamCmdProcess = Start-Process -FilePath $steamCmdExePath -ArgumentList $steamCmdArgs -NoNewWindow -Wait -PassThru
    
    # check if the process exited successfully
    $synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
    $synthetikMd5 = ""
    if (-not $synthetikHash) {
        Write-Host "Failed to update SYNTHETIK to the beta branch. Trying again..."
        $steamCmdArgs = @(
            "+login $($steamUsername)",
            "+app_update 528230 -beta beta validate",
            "+exit"
        )
        $steamCmdProcess = Start-Process -FilePath $steamCmdExePath -ArgumentList $steamCmdArgs -NoNewWindow -Wait -PassThru
        $synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
        exit 1
    } else {
        $synthetikMd5 = $synthetikHash.Hash.ToLower()
        if ($synthetikMd5 -ne $betaMd5) {
            Write-Host "Failed to update SYNTHETIK to the beta branch. Trying again..."
            $steamCmdArgs = @(
                "+login $($steamUsername)",
                "+app_update 528230 -beta beta validate",
                "+exit"
            )
            $steamCmdProcess = Start-Process -FilePath $steamCmdExePath -ArgumentList $steamCmdArgs -NoNewWindow -Wait -PassThru
            $synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
        }
    }    
    $synthetikHash = (Get-FileHash -Path $synthetikExePath -Algorithm MD5 -ErrorAction SilentlyContinue)
    $synthetikMd5 = $synthetikHash.Hash.ToLower()
    if ($synthetikMd5 -ne $betaMd5) {
        Write-Host "Failed to update SYNTHETIK to the beta branch."
        Write-Host "Please come to our Discord for support: https://discord.gg/KXPgRzSpBY"
        Write-Host "You can also try to set the beta branch manually in Steam: "
        Write-Host "SYNTHETTIK (right click) -> Properties -> Betas -> beta - 2.3.7"
        exit 1
    }
    Write-Host "SYNTHETIK updated to the beta branch successfully."
}

# backup SYNTHETIK.exe
$synthetikBackupPath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe.bak"
Write-Host "Backing up SYNTHETIK.exe to $synthetikBackupPath..."
Copy-Item -Path $synthetikExePath -Destination $synthetikBackupPath -Force

# copy the contents of the temporary Organik directory to the defined gameInstallDir
Write-Host "Copying Organik files to $gameInstallDir..."

#iterate over sourcePath and copy files
Get-ChildItem -Path $organikExtractPath| Copy-Item -Destination $gameInstallDir -Recurse -Force


Write-Host "Organik files copied successfully."
Write-Host "Renaming SYNTHETIK.exe to game.exe"
Move-Item -Path $synthetikExePath -Destination (Join-Path -Path $gameInstallDir -ChildPath "game.exe") -Force *> $null

# Rename AurieLoader-x86.exe to SYNTHETIK.exe
$aurieLoaderPath = Join-Path -Path $organikExtractPath -ChildPath "AurieLoader-x86.exe"
$synthetikExeNewPath = Join-Path -Path $gameInstallDir -ChildPath "SYNTHETIK.exe"
Write-Host "Renaming AurieLoader-x86.exe to SYNTHETIK.exe"
Move-Item -Path $aurieLoaderPath -Destination $synthetikExeNewPath -Force