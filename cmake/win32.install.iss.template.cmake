; rdo inno setup install script

#define AppName      "${PROJECT_NAME}"
#define AppVersion   "${PROJECT_VERSION_LONG}"
#define AppPublisher "${PROJECT_VENDOR_LONG}"
#define AppURL       "${ORG_WEBSITE}"
#define AppGUID      "a217a564-2c58-4170-bae9-3bf3630b0baa"

#define AppName "${PROJECT_NAME}"
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
Compression = lzma2
SolidCompression = yes
OutputBaseFilename = ${PROJECT_NAME}-${PROJECT_VERSION_LONG}

[Languages]

Name: ru; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]

Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]

Name: "{group}\RAO Studio";           Filename: "{app}\{#AppRdoStudioExeName}"; WorkingDir: "{app}"
Name: "{group}\Uninstall {#AppName}"; Filename: "{uninstallexe}"

Name: "{commondesktop}\{#AppRdoStudioExeName}"; Filename: "{app}\{#AppRdoStudioExeName}"; Tasks: desktopicon

[Files]

