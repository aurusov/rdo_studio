object FormFix: TFormFix
  Left = 263
  Top = 199
  BorderStyle = bsDialog
  Caption = 'Восстановление индексов базы данных'
  ClientHeight = 203
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 0
    Top = 0
    Width = 320
    Height = 162
    Align = alClient
    Color = clTeal
    Ctl3D = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
  end
  object Panel: TPanel
    Left = 0
    Top = 162
    Width = 320
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object Button: TButton
      Left = 124
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Закрыть'
      TabOrder = 0
      OnClick = ButtonClick
    end
  end
end
