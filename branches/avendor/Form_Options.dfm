object OptionsForm: TOptionsForm
  Left = 192
  Top = 107
  BorderStyle = bsNone
  Caption = ' '
  ClientHeight = 300
  ClientWidth = 414
  Color = 16316647
  Font.Charset = DEFAULT_CHARSET
  Font.Color = 8478492
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = [fsBold]
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object TopShape: TShape
    Left = -2
    Top = 13
    Width = 415
    Height = 25
    Brush.Color = 8478492
    Pen.Color = 8478492
    Pen.Width = 2
  end
  object Title: TLabel
    Left = 4
    Top = 18
    Width = 401
    Height = 14
    Alignment = taCenter
    AutoSize = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 16053490
    Font.Height = -12
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
    Transparent = True
  end
  object BottomShape: TShape
    Left = -2
    Top = 217
    Width = 415
    Height = 25
    Brush.Color = 8478492
    Pen.Color = 8478492
    Pen.Width = 2
  end
  object BigShape: TShape
    Left = 28
    Top = 252
    Width = 57
    Height = 37
    Brush.Style = bsClear
    Pen.Color = 8478492
    Pen.Width = 2
    Shape = stRoundRect
  end
  object Shape1: TShape
    Left = 142
    Top = 44
    Width = 2
    Height = 133
    Brush.Style = bsClear
    Pen.Color = 8478492
    Pen.Width = 2
  end
  object LangLab: TLabel
    Left = 8
    Top = 40
    Width = 121
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'LangLab'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 8478492
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ModShowLab: TLabel
    Left = 152
    Top = 40
    Width = 253
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'ModShowLab'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 8478492
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PresentLab: TLabel
    Left = 152
    Top = 105
    Width = 253
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'PresentLab'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 8478492
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Shape2: TShape
    Left = 12
    Top = 184
    Width = 389
    Height = 2
    Brush.Style = bsClear
    Pen.Color = 8478492
    Pen.Width = 2
  end
  object ModelsShape: TShape
    Left = 210
    Top = 75
    Width = 32
    Height = 17
    Brush.Style = bsClear
    Pen.Color = 8478492
  end
  object ModDelL: TLabel
    Left = 171
    Top = 76
    Width = 33
    Height = 13
    Caption = 'Delay'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
  end
  object ModDelSL: TLabel
    Left = 255
    Top = 76
    Width = 23
    Height = 13
    Caption = 'sec.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
  end
  object PresentShape: TShape
    Left = 210
    Top = 140
    Width = 32
    Height = 17
    Brush.Style = bsClear
    Pen.Color = 8478492
  end
  object PresDelSL: TLabel
    Left = 255
    Top = 141
    Width = 23
    Height = 13
    Caption = 'sec.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
  end
  object PresDelL: TLabel
    Left = 171
    Top = 141
    Width = 33
    Height = 13
    Caption = 'Delay'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
  end
  object LangBox: TListBox
    Left = 8
    Top = 60
    Width = 121
    Height = 117
    BorderStyle = bsNone
    Color = 16316647
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ItemHeight = 16
    Items.Strings = (
      'English'
      'Русский'
      'Italianodsfhsdkjfdsdsfsdf'
      'Francais'
      'Deutch'
      'Espanol')
    ParentFont = False
    Style = lbOwnerDrawFixed
    TabOrder = 0
    OnDrawItem = LangBoxDrawItem
  end
  object ModelsDelay: TEdit
    Left = 211
    Top = 76
    Width = 30
    Height = 15
    BorderStyle = bsNone
    Color = 16316647
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '30'
  end
  object PresentDelay: TEdit
    Left = 211
    Top = 141
    Width = 30
    Height = 15
    BorderStyle = bsNone
    Color = 16316647
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 6309653
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '30'
  end
end
