//---------------------------------------------------------------------------
#ifndef HighlightTextH
#define HighlightTextH
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TNotifyHighLight)(System::TObject* Sender, const bool HighLighted);

//---------------------------------------------------------------------------
class THighlightText : public TStaticText
{
private:
  bool FHighlight;
  void SetHighlight(const bool Value);
  void __fastcall WndProc(Messages::TMessage &Message);
  void UpdateText(const bool Value);
  bool FStayHighlighted;
  void SetStayHighlighted(const bool Value);
public:
  __fastcall THighlightText(TComponent* Owner);
  __property bool Highlight = { read=FHighlight, write=SetHighlight };
  __property bool StayHighlighted = { read=FStayHighlighted, write=SetStayHighlighted };
  TImage*     Image;
  TColor      NormColor;
  TCursor     NormCursor;
  TFontStyles NormStyle;
  TColor      HighlColor;
  TCursor     HighlCursor;
  TFontStyles HighlStyle;
  TNotifyHighLight OnHighLight;
};
#endif
