//---------------------------------------------------------------------------
#include <vcl.h>
#include <classes.hpp>
#include <inifiles.hpp>
#include <DBTables.hpp>
#pragma hdrstop

#include "main_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define def_filescount 105

TMainForm* MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner):
  TForm(Owner),
  FilesDeleted(0),
  FilesCount(def_filescount)
{
  TIniFile* file = NULL;
  try {
    TIniFile* file = new TIniFile("Rubber_uninstall.ini");
    ProgramsRoot = file->ReadString("Uninstall", "ProgramsRoot", "");
    ShotCutRoot = file->ReadString("Uninstall", "ShotCutRoot", "");
    delete file; file = NULL;
  } catch (Exception &e) {
    if (file) delete file;
    throw;
  }
  ProgramsRoot = GetNormalDirFromDir(ProgramsRoot);
  ShotCutRoot  = GetNormalDirFromDir(ShotCutRoot);
  Label1->Visible       = false;
  ProgressBarL->Visible = false;
  ProgressBar->Visible  = false;
  if ( ProgramsRoot.IsEmpty() && ShotCutRoot.IsEmpty() ) {
    Label1->Caption = "Программа 'Участок РТИ' не установлена";
    Label1->Visible     = true;
    UninstallB->Visible = false;
  }
}

void __fastcall TMainForm::UninstallBClick(TObject *Sender)
{
  Label1->Visible       = true;
  ProgressBarL->Visible = true;
  ProgressBar->Visible  = true;
  Uninstall();
  char* str = NULL;
  TFileStream* ini = NULL;
  TStringList* list = NULL;
  try {
    str = StrAlloc(MAX_PATH);
    FillMemory(str, MAX_PATH, '\0');
    GetShortPathName(Application->ExeName.c_str(), str, MAX_PATH);
    AnsiString application = str;
    FillMemory(str, MAX_PATH, '\0');
    GetWindowsDirectory(str, MAX_PATH);
    AnsiString windir = str;
    windir = GetNormalDirFromDir(windir);
    AnsiString inifile = windir + "Rubber_uninstall.ini";
    FillMemory(str, MAX_PATH, '\0');
    GetShortPathName(inifile.c_str(), str, MAX_PATH);
    AnsiString delinifile = str;
    StrDispose(str); str = NULL;
    inifile = windir + "wininit.ini";
    if (FileExists(inifile))
      ini = new TFileStream(inifile, fmOpenWrite);
    else
      ini = new  TFileStream(inifile, fmCreate);
    list = new TStringList();
    list->LoadFromStream(ini);
    int idx = 0;
    if (list->Count) {
       list->IndexOf("[Rename]");
      if (idx == -1)
        idx = list->IndexOf("[RENAME]");
      if (idx == -1)
        idx = list->IndexOf("[rename]");
      if (idx == -1)
        idx = 0;
    } else {
      idx = 0;
    }
    if (!idx) {
      idx = list->Add("[Rename]") + 1;
    } else {
      idx++;
    }
    list->Insert(idx, "nul=" + application);
    idx++;
    list->Insert(idx, "nul=" + delinifile);
    list->SaveToStream(ini);
    delete list; list = NULL;
    delete ini; ini = NULL;
    ProgressBarL->Caption = "Удаление успешно завершено";
    UninstallB->Visible = false;
  } catch (Exception& e) {
    if (str) StrDispose(str);
    if (list) delete list;
    if (ini) delete ini;
    throw;
  }
}

AnsiString TMainForm::GetNormalDirFromDir(const AnsiString& Value)
{
  AnsiString s = Value;

  if (s.Length()) {
    char c = *s.AnsiLastChar();
    if (c != '/' && c != '\\') s += '\\';
  }
  return s;
}

void TMainForm::Uninstall()
{
  AnsiString CurFolder = ShotCutRoot;
  EmptyDir(CurFolder);
  RemoveDir(CurFolder);
  CurFolder = ProgramsRoot + "\\PlanBase";
  EmptyDir(CurFolder);
  RemoveDir(CurFolder);
  CurFolder = ProgramsRoot;
  EmptyDir(CurFolder);
  RemoveDir(CurFolder);
  TStringList* list = NULL;
  try {
    list = new TStringList();
    Session->GetAliasNames(list);
    if (list->IndexOf("Planning") != -1) {
      Session->DeleteAlias("Planning");
      Session->SaveConfigFile();
    }
    delete list; list = NULL;
  } catch (Exception& e) {
    if (list) delete list;
    throw;
  }
}

void TMainForm::EmptyDir(const AnsiString Dir)
{
  TSearchRec rec;
  int Status = FindFirst(Dir + "\\*.*", faReadOnly | faHidden | faSysFile | faArchive, rec);
  while (!Status) {
    DeleteFile(Dir + "\\" + rec.Name);
    FilesDeleted ++;
    ProgressBarL->Caption = rec.Name;
    ProgressBar->Position = (double)(FilesDeleted*100.0/FilesCount);
    Application->ProcessMessages();
    Status = FindNext(rec);
  }
  FindClose(rec);
}

void __fastcall TMainForm::CloseBClick(TObject *Sender)
{
  Close();
}

