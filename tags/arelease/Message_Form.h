//---------------------------------------------------------------------------
#ifndef Message_FormH
#define Message_FormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>

#include "HighlightText.h"
//---------------------------------------------------------------------------
class TMessageForm : public TForm
{
__published:
  TLabel *Title;
  TShape *TopShape;
  TShape *BottomShape;
  TShape *BigShape;
  TImage *Image;
  TLabel *MessageL;
  TImageList *Transp;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormShow(TObject *Sender);
private:
  HRGN rgn;
  double KoefHeight;

  THighlightText* OkB;
  void __fastcall OkBClick(TObject *Sender);

  bool FMouseDown;
  int dX, dY;
  void __fastcall WndProc(Messages::TMessage &Message);

  UINT FType;
  void SetType(const UINT Value);
  AnsiString FMessage;
  void SetMessage(const AnsiString Value);
public:
  __fastcall TMessageForm(void);
  __fastcall ~TMessageForm(void);
  __property UINT Type = { read=FType, write=SetType };
  __property AnsiString Message = { read=FMessage, write=SetMessage };
};
//---------------------------------------------------------------------------
#endif
