//---------------------------------------------------------------------------

#ifndef main_formH
#define main_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
  TButton *UninstallB;
  TButton *CloseB;
  TBevel *Bevel1;
  TLabel *Label1;
  TLabel *ProgressBarL;
  TProgressBar *ProgressBar;
  void __fastcall UninstallBClick(TObject *Sender);
  void __fastcall CloseBClick(TObject *Sender);
private:
  int FilesDeleted;
  int FilesCount;
  AnsiString ProgramsRoot;
  AnsiString ShotCutRoot;
  
  AnsiString GetNormalDirFromDir(const AnsiString& Value);
  void Uninstall();
  void EmptyDir(const AnsiString Dir);

public:
  __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
