//#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// Allgemeine Informationen über eine Assembly werden über die folgenden
// Attribute gesteuert. Ändern Sie diese Attributwerte, um die Informationen zu ändern,
// die einer Assembly zugeordnet sind.
//
[assembly:AssemblyTitleAttribute(L"VirtualHardwareWrapper")];
[assembly:AssemblyDescriptionAttribute(L"Virtual Board C++ access wrapper to .NET VirtualHardwareNet library")];
[assembly:AssemblyConfigurationAttribute(L"Release")];
[assembly:AssemblyCompanyAttribute(L"github.com/virtual-maker/VirtualBoard")];
[assembly:AssemblyProductAttribute(L"VirtualBoard")];
[assembly:AssemblyCopyrightAttribute(L"Copyright (c) Virtual Maker / Immo Wache 2022")];
[assembly:AssemblyTrademarkAttribute(L"written by Immo Wache")];
[assembly:AssemblyCultureAttribute(L"")];

//
// Versionsinformationen für eine Assembly bestehen aus den folgenden vier Werten:
//
//      Hauptversion
//      Nebenversion
//      Buildnummer
//      Revision
//
// Sie können alle Werte angeben oder für die Revisions- und Buildnummer den Standard
// übernehmen, indem Sie "*" eingeben:

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];