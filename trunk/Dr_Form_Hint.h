//---------------------------------------------------------------------------
#ifndef Dr_Form_HintH
#define Dr_Form_HintH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class THintForm : public TForm
{
__published:
  TShape *CaptionS;
  TLabel *CaptionL;
  TImage *CaptionImage;
  TSpeedButton *ExitB;
  TPopupMenu *ClosePM;
  TMenuItem *CloseM;
  TShape *BorderS;
  TShape *ShadowS;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall CloseMClick(TObject *Sender);
  void __fastcall ExitBClick(TObject *Sender);
  void __fastcall CaptionSMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall CaptionImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:
  HRGN rgn;

  void __fastcall WndProc(Messages::TMessage &Message);

  bool bActive;
  bool FMouseDown;
  int dX, dY;

  AnsiString __fastcall GetCaption();
  void __fastcall SetCaption(AnsiString Value);

protected:
  DYNAMIC void __fastcall Resize();
  void CheckSize();
public:
  __fastcall THintForm(TComponent* Owner);
  __fastcall ~THintForm();
  void __fastcall Show();
  int __fastcall ShowModal();
  int shadowWidth;
  bool bDesktopCenter;
  __property AnsiString Caption = {read=GetCaption, write=SetCaption};
};
//---------------------------------------------------------------------------
#endif

