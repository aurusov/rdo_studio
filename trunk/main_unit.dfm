object Form1: TForm1
  Left = 221
  Top = 111
  BorderStyle = bsDialog
  Caption = 'RAO-platareader'
  ClientHeight = 303
  ClientWidth = 501
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010002002020100000000000E80200002600000010101000000000002801
    00000E0300002800000020000000400000000100040000000000800200000000
    0000000000000000000000000000000000000000800000800000008080008000
    00008000800080800000C0C0C000808080000000FF0000FF000000FFFF00FF00
    0000FF00FF00FFFF0000FFFFFF00000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000088800000000000000000000
    0000000099988888888888800000000000000000999999999999998000000000
    0000000099900000000009800000000000000000000000000000098000000000
    0000000000000000000009800000000000000000000000088800098000000000
    0000000000000099980009800000000000000000000000999800098000000000
    0000000000000099900009800000000000000000000000098000098000000000
    0000000000000009800009800000000000088800000000098000098000000000
    0099988888888889000009800000000000999999999999990000098000000000
    0099900000000000000009800000000000000000000000000000098000000000
    0000000000000000000009800000000000000000000000000000098000000000
    0000000000000000000099900000000000000000000000000000999000000000
    000000000000000000009990000094514514BEFBEFBEFFFFFFFFBEFBEFBEFFFF
    FFFFBEFBEFBE94514514BEFBEFBEFFFFFFFFBEFBEFBEFFF8FFFFBEF0001E9450
    0014BEF1EF9EFFFFFF9FBEFBEF9EFFFFE39FBEFBC39E94514194BEFBC79EFFFF
    E79FBEFBE79EFE3FE79FBC000F9E94000514BC7BEF9EFFFFFF9FBEFBEF9EFFFF
    FF9FBEFBEF1E94514514FFFFFF1F280000001000000020000000010004000000
    0000C00000000000000000000000000000000000000000000000000080000080
    000000808000800000008000800080800000C0C0C000808080000000FF0000FF
    000000FFFF00FF000000FF00FF00FFFF0000FFFFFF0009990000000000000999
    9999999999900999000000000090000000000000009000000000000000900000
    0000000000900000000999000090000000099900009000000009990000900000
    0000900000900000000090000090000000009000009009990000900009990999
    999990000999099900000000099900000000000000008FFF0000800100008F7D
    0000FFFD0000DF7D0000FFFD0000DE3D0000CA290000DE3D0000FF7D0000DF7D
    0000FF7D00008F780000802800008FF80000FFFF0000}
  OldCreateOrder = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 185
    Height = 303
    Align = alLeft
    Caption = 'Настройки'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 316
      Width = 77
      Height = 13
      Caption = 'Размеры поля:'
      Visible = False
    end
    object Label2: TLabel
      Left = 8
      Top = 344
      Width = 84
      Height = 13
      Caption = 'По горизонтали:'
      Visible = False
    end
    object Label3: TLabel
      Left = 8
      Top = 371
      Width = 73
      Height = 13
      Caption = 'По вертикили:'
      Visible = False
    end
    object Label4: TLabel
      Left = 8
      Top = 28
      Width = 44
      Height = 13
      Caption = 'Провода'
    end
    object Label5: TLabel
      Left = 8
      Top = 400
      Width = 80
      Height = 13
      Caption = 'Размер клетки:'
      Visible = False
    end
    object CSpinEdit1: TCSpinEdit
      Left = 96
      Top = 339
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 0
      Value = 10
      Visible = False
      OnChange = PaintBox1Paint
    end
    object CSpinEdit2: TCSpinEdit
      Left = 96
      Top = 368
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 1
      Value = 10
      Visible = False
      OnChange = PaintBox1Paint
    end
    object ListBox1: TListBox
      Left = 8
      Top = 44
      Width = 169
      Height = 97
      ItemHeight = 13
      TabOrder = 2
    end
    object CSpinEdit3: TCSpinEdit
      Left = 96
      Top = 397
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 3
      Value = 25
      Visible = False
      OnChange = PaintBox1Paint
    end
    object ButtonDelete: TButton
      Left = 8
      Top = 184
      Width = 81
      Height = 25
      Caption = 'Удалить'
      Enabled = False
      TabOrder = 4
      OnClick = ButtonDeleteClick
    end
    object Button4: TButton
      Left = 8
      Top = 256
      Width = 81
      Height = 25
      Caption = 'ПУСК!'
      TabOrder = 5
      OnClick = Button4Click
    end
    object CheckBox1: TCheckBox
      Left = 8
      Top = 232
      Width = 161
      Height = 17
      Caption = 'Сохранить файлы модели'
      TabOrder = 6
    end
    object ButtonInsert: TButton
      Left = 8
      Top = 152
      Width = 81
      Height = 25
      Caption = 'Добавить'
      Enabled = False
      TabOrder = 7
      OnClick = ButtonInsertClick
    end
  end
  object ScrollBox1: TScrollBox
    Left = 185
    Top = 0
    Width = 316
    Height = 303
    Align = alClient
    TabOrder = 1
    object PaintBox1: TPaintBox
      Left = 0
      Top = 0
      Width = 550
      Height = 510
      OnMouseDown = PaintBox1MouseDown
      OnMouseMove = PaintBox1MouseMove
      OnPaint = PaintBox1Paint
    end
  end
end
