$VerbosePreference="Continue"
$ErrorActionPreference="Stop"

$version = $args[0]
if (!$version) {
    $version = "0.0.0"
}

$vsixIdentity = $args[1]
if (!$vsixIdentity) {
    $vsixIdentity = "0.0.0"
}

Write-Output "Set version: $version"
Write-Output "Set VSIX identity: $vsixIdentity"

if ($psISE) {
    $RootPath = Split-Path -Path $psISE.CurrentFile.FullPath 
    "A"       
} else {
    $RootPath = $PSScriptRoot
    "B"
}

# Import functions to get and set XML node attribute and inner text
$ModulePath = Join-Path -Path $RootPath -ChildPath "get-set-xml.psm1"
Import-Module $ModulePath

# Update version number in NuGet package specification (.nuspec)
$FullPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "BuildNuget"
$FullPath = Join-Path -Path $FullPath -ChildPath "VirtualBoard.nuspec"
[xml]$content = Get-Content -Path $FullPath
Set-XmlElementsTextValue -XmlDocument $content -ElementPath "package.metadata.version" -TextValue $version
$content.Save($FullPath)

# Update ArduinoProjectTemplate with NuGet package version
$FullPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "ProjectTemplates"
$FullPath = Join-Path -Path $FullPath -ChildPath "ArduinoProjectTemplate"
$FullPath = Join-Path -Path $FullPath -ChildPath "MyTemplate.vstemplate"
[xml]$content = Get-Content $FullPath
$content.VSTemplate.WizardData.packages.package.version = $version
$content.Save($FullPath)
# zip template and copy to VSIX project
$SourcePath = (get-item $FullPath).Directory.FullName
$DestPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "VirtualBoardExtension"
$DestPath = Join-Path -Path $DestPath -ChildPath "ProjectTemplates"
$DestPath = Join-Path -Path $DestPath -ChildPath "VirtualMaker"
$DestPath = Join-Path -Path $DestPath -ChildPath "ArduinoProjectTemplate.zip"
$global:ProgressPreference = 'SilentlyContinue'
Compress-Archive -Path $SourcePath -DestinationPath $DestPath -Force >$null

# Update MySensorsProjectTemplate with NuGet package version
$FullPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "ProjectTemplates"
$FullPath = Join-Path -Path $FullPath -ChildPath "MySensorsProjectTemplate"
$FullPath = Join-Path -Path $FullPath -ChildPath "MyTemplate.vstemplate"
[xml]$content = Get-Content $FullPath
$content.VSTemplate.WizardData.packages.package.version = $version
$content.Save($FullPath)
# zip template and copy to VSIX project
$SourcePath = (get-item $FullPath).Directory.FullName
$DestPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "VirtualBoardExtension"
$DestPath = Join-Path -Path $DestPath -ChildPath "ProjectTemplates"
$DestPath = Join-Path -Path $DestPath -ChildPath "VirtualMaker"
$DestPath = Join-Path -Path $DestPath -ChildPath "MySensorsProjectTemplate.zip"
$global:ProgressPreference = 'SilentlyContinue'
Compress-Archive -Path $SourcePath -DestinationPath $DestPath -Force >$null

# Update extension project item content NuGet package file name
$FullPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "VirtualBoardExtension"
$FullPath = Join-Path -Path $FullPath -ChildPath "VirtualBoardExtension.csproj"
[xml]$content = Get-Content -Path $FullPath
# ############################################################
$AttributeValue = "Packages\VirtualBoard.$($version).nupkg"
Set-XmlElementsAttributeValue -XmlDocument $content -ElementPath "Project/ItemGroup/Content[starts-with(@Include,'Packages\VirtualBoard.')]" -AttributeName "Include" -AttributeValue $AttributeValue -NodeSeparatorCharacter '/'
$content.Save($FullPath)

# Update version numbers in VSIX manifest (source.extension.vsixmanifest)
$FullPath = Join-Path -Path (get-item $RootPath).Parent.FullName -ChildPath "VirtualBoardExtension"
$FullPath = Join-Path -Path $FullPath -ChildPath "source.extension.vsixmanifest"
[xml]$content = Get-Content -Path $FullPath
Set-XmlElementsAttributeValue -XmlDocument $content -ElementPath "PackageManifest/Metadata/Identity" -AttributeName "Version" -AttributeValue $vsixIdentity -NodeSeparatorCharacter '/'
$AttributeValue = "VirtualBoard.$($version).nupkg"
Set-XmlElementsAttributeValue -XmlDocument $content -ElementPath "PackageManifest/Assets/Asset[starts-with(@Type,'VirtualBoard.')]" -AttributeName "Type" -AttributeValue $AttributeValue -NodeSeparatorCharacter '/'
$AttributeValue = "Packages\VirtualBoard.$($version).nupkg"
Set-XmlElementsAttributeValue -XmlDocument $content -ElementPath "PackageManifest/Assets/Asset[starts-with(@Type,'VirtualBoard.')]" -AttributeName "Path" -AttributeValue $AttributeValue -NodeSeparatorCharacter '/'
$content.Save($FullPath)
