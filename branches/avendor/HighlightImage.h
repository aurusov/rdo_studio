//---------------------------------------------------------------------------
#ifndef HighlightImageH
#define HighlightImageH
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TNotifyHighLight)(System::TObject* Sender, const bool HighLighted);

//---------------------------------------------------------------------------
class THighlightImage : public TImage
{
private:
  bool FHighlight;
  void SetHighlight(const bool Value);
  void __fastcall WndProc(Messages::TMessage &Message);
  virtual void __fastcall SetParent(TWinControl* AParent);
public:
  __fastcall THighlightImage(TComponent* Owner);
  __property bool Highlight = { read=FHighlight, write=SetHighlight };
  void UpdateImage(const bool Value);
  int               NormImageIndex;
  TCursor           NormCursor;
  int               HighlImageIndex;
  TCursor           HighlCursor;
  TCustomImageList* Images;
  TCustomImageList* HotImages;
  TNotifyHighLight  OnHighLight;
};
#endif
