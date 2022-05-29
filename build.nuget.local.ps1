$GitVersion = dotnet-gitversion | ConvertFrom-Json
$GitVersion.NuGetVersion
$GitVersion.MajorMinorPatch

# Build VirtualBoard solution
Invoke-MsBuild -Path .\VirtualBoard.sln -MsBuildParameters "/target:Clean;Build /property:Configuration=Release;Platform=Win32"

# Apply Git version to NuGet and project configuration files
& "$PSScriptRoot\BuildScripts\update-version.ps1" $GitVersion.NuGetVersion $GitVersion.MajorMinorPatch

# Pack NuGet package 
NuGet pack "$PSScriptRoot\BuildNuget\VirtualBoard.nuspec" -OutputDirectory "$PSScriptRoot\BuildNuget\bin"

# Copy to local NuGet repository
$NuGetRepo = 'C:\NuGetTools\NuGetScratch\'
if (Test-Path -Path $NuGetRepo) {
    Copy-Item "$PSScriptRoot\BuildNuget\bin\*.nupkg" -Destination "C:\NuGetTools\NuGetScratch\"
} else {
    "Warning: Local NuGet repository [$NuGetRepo] not found."
}
# Undo changes to NuGet and project configuration files 
& "$PSScriptRoot\BuildScripts\update-version.ps1"