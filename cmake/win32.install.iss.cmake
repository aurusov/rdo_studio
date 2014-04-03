; webot inno setup install script

#define AppName      "${PROJECT_NAME}"
#define AppVersion   "${PROJECT_VERSION_LONG}"
#define AppPublisher "${PROJECT_VENDOR_LONG}"
#define AppURL       "${ORG_WEBSITE}"
#define AppGUID      "CBA4E3C1-B3FE-4917-B879-BE04F4367422"

#define AppName "${PROJECT_NAME}"
#define AppRdoExeName "rdo.exe"
#define AppRdoStudioExeName "rdo_studio.exe"

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName = {#AppName}
AppVersion = {#AppVersion}
AppId = {#AppGUID}
AppPublisher = {#AppPublisher}
AppCopyright = {#AppPublisher}
AppPublisherURL = {#AppURL}
VersionInfoCompany = {#AppPublisher}
DefaultDirName = {pf}\{#AppName}
DefaultGroupName = {#AppName}
UninstallDisplayIcon = {app}\{#AppRdoStudioExeName}
Compression = lzma2
SolidCompression = yes
OutputBaseFilename = ${PROJECT_NAME}-${PROJECT_VERSION_LONG}

[Languages]
Name: ru; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#AppRdoExeName}"; DestDir: "{app}"
Source: "{#AppRdoStudioExeName}"; DestDir: "{app}"

Source: "Qt5Core.dll";    DestDir: "{app}"
Source: "Qt5Gui.dll";     DestDir: "{app}"
Source: "Qt5Widgets.dll"; DestDir: "{app}"

Source: "plugins/platforms/qminimal.dll"; DestDir: "{app}/plugins/platforms"
Source: "plugins/platforms/qwindows.dll"; DestDir: "{app}/plugins/platforms"

Source: "libGLESv2.dll";      DestDir: "{app}"
Source: "libEGL.dll";         DestDir: "{app}"
Source: "icuuc51.dll";        DestDir: "{app}"
Source: "icuin51.dll";        DestDir: "{app}"
Source: "icudt51.dll";        DestDir: "{app}"
;Source: "D3DCompiler_43.dll"; DestDir: "{app}"

Source: "msvcr100.dll"; DestDir: "{app}"
Source: "msvcp100.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\{#AppRdoExeName}";       Filename: "{app}\{#AppRdoExeName}"; WorkingDir: "{app}"
Name: "{group}\{#AppRdoStudioExeName}"; Filename: "{app}\{#AppRdoStudioExeName}"; WorkingDir: "{app}"

Name: "{group}\Uninstall {#AppName}";   Filename: "{uninstallexe}"

Name: "{commondesktop}\{#AppRdoExeName}"; Filename: "{app}\{#AppRdoExeName}"; Tasks: desktopicon
Name: "{commondesktop}\{#AppRdoStudioExeName}"; Filename: "{app}\{#AppRdoStudioExeName}"; Tasks: desktopicon
