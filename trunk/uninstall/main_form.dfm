object MainForm: TMainForm
  Left = 290
  Top = 181
  BorderStyle = bsDialog
  Caption = 'Удаление программы '#39'Участок РТИ'#39
  ClientHeight = 158
  ClientWidth = 408
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 12
    Top = 112
    Width = 377
    Height = 9
    Shape = bsTopLine
  end
  object Label1: TLabel
    Left = 12
    Top = 12
    Width = 58
    Height = 13
    Caption = 'Удаляется:'
  end
  object ProgressBarL: TLabel
    Left = 80
    Top = 12
    Width = 3
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object UninstallB: TButton
    Left = 216
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Удаление'
    TabOrder = 0
    OnClick = UninstallBClick
  end
  object CloseB: TButton
    Left = 316
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Выход'
    TabOrder = 1
    OnClick = CloseBClick
  end
  object ProgressBar: TProgressBar
    Left = 12
    Top = 29
    Width = 377
    Height = 16
    Min = 0
    Max = 100
    TabOrder = 2
  end
end
