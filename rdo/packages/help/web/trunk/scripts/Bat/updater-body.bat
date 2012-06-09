@echo off
set PATH1=%1
set PATH2=%2
set PATH3=%PATH1%\web\scripts\Perl

TortoiseProc.exe /command:cleanup /PATH:%PATH1% /nodlg /externals /cleanup  /closeonend:1 /noui
TortoiseProc.exe /command:update /PATH:%PATH1% /closeonend:1

TortoiseProc.exe /command:cleanup /PATH:%PATH2% /nodlg /externals /cleanup  /closeonend:1 /noui
TortoiseProc.exe /command:update /PATH:%PATH2% /closeonend:1

cd %PATH3%
copy /Y index-body.tmp %PATH2%
copy /Y index-head.tmp %PATH2%
copy /Y help-index.pl %PATH2%
copy /Y keywords-body.tmp %PATH2%
copy /Y keywords-head.tmp %PATH2%
copy /Y keyword-index.pl %PATH2%

cd %PATH2%
help-index.pl
keyword-index.pl
copy /Y keywords.html %PATH1%
copy /Y index.html %PATH1%