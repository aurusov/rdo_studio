//---------------------------------------------------------------------------

#ifndef main_unitH
#define main_unitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TCSpinEdit *CSpinEdit1;
    TCSpinEdit *CSpinEdit2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TListBox *ListBox1;
    TLabel *Label4;
    TLabel *Label5;
    TCSpinEdit *CSpinEdit3;
	TButton *ButtonDelete;
    TButton *Button4;
    TCheckBox *CheckBox1;
    TScrollBox *ScrollBox1;
    TPaintBox *PaintBox1;
	TButton *ButtonInsert;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall PaintBox1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PaintBox1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ButtonInsertClick(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
