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
      Caption = 'Запуск'
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
