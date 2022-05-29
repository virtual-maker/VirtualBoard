$GitVersion = dotnet-gitversion | ConvertFrom-Json
$GitVersion.NuGetVersion
$GitVersion.MajorMinorPatch

# Build VirtualBoard solution
Invoke-MsBuild -Path .\VirtualBoard.sln -MsBuildParameters "/target:Clean;Build /property:Configuration=Release;Platform=Win32"

# Apply Git version to NuGet and project configuration files
& "$PSScriptRoot\BuildScripts\update-version.ps1" $GitVersion.NuGetVersion $GitVersion.MajorMinorPatch

# Tidy up VSIX extension folder for NuGet packages
Remove-Item "$PSScriptRoot\VirtualBoardExtension\Packages\*.nupkg"

# Pack NuGet package 
NuGet pack "$PSScriptRoot\BuildNuget\VirtualBoard.nuspec" -OutputDirectory "$PSScriptRoot\VirtualBoardExtension\Packages"

# Build VSIX extension
Invoke-MsBuild -Path .\VirtualBoardExtension.sln -MsBuildParameters "/target:Clean;Build /property:Configuration=Release"
# -LogVerbosityLevel minimal -ShowBuildOutputInNewWindow -PromptForInputBeforeClosing

# Undo changes to NuGet and project configuration files 
& "$PSScriptRoot\BuildScripts\update-version.ps1"
