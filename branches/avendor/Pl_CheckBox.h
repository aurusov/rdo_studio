//---------------------------------------------------------------------------
#ifndef Pl_CheckBoxH
#define Pl_CheckBoxH
//---------------------------------------------------------------------------
class PACKAGE TPaulCheckBox : public TWinControl
{
private:
  TControlCanvas* canvas;

  bool bFocus;

  virtual void __fastcall SetEnabled(const bool Value);

  virtual void __fastcall WndProc(Messages::TMessage &Message);

  TColor FBackColor;
  void __fastcall SetBackColor(const TColor Value);

  TColor FDisabledColor;
  void __fastcall SetDisabledColor(const TColor Value);

  bool FChecked;
  void __fastcall SetChecked(const bool Value);

  DYNAMIC void __fastcall Click(void);
public:
  __fastcall TPaulCheckBox(TComponent* Owner);
  __fastcall ~TPaulCheckBox(void);
  virtual void __fastcall Repaint(void);
__published:
  __property bool Checked = {read=FChecked, write=SetChecked};
  __property TColor BackColor = { read=FBackColor, write=SetBackColor, default=clBtnFace };
  __property TColor DisabledColor = { read=FDisabledColor, write=SetDisabledColor, default=clBtnFace};
  __property Caption;
  __property Font;
  __property TabOrder;
  __property TabStop;
  __property ShowHint;
  __property OnClick;
};
#endif
