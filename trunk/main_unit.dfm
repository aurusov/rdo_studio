object Form1: TForm1
  Left = 223
  Top = 120
  Width = 747
  Height = 548
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 185
    Height = 521
    Align = alLeft
    Caption = 'Инструменты'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 77
      Height = 13
      Caption = 'Размеры поля:'
    end
    object Label2: TLabel
      Left = 8
      Top = 56
      Width = 84
      Height = 13
      Caption = 'По горизонтали:'
    end
    object Label3: TLabel
      Left = 8
      Top = 83
      Width = 73
      Height = 13
      Caption = 'По вертикили:'
    end
    object Label4: TLabel
      Left = 8
      Top = 280
      Width = 75
      Height = 13
      Caption = 'Точки провода'
    end
    object Label5: TLabel
      Left = 8
      Top = 112
      Width = 80
      Height = 13
      Caption = 'Размер клетки:'
    end
    object CSpinEdit1: TCSpinEdit
      Left = 96
      Top = 51
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 0
      Value = 10
    end
    object CSpinEdit2: TCSpinEdit
      Left = 96
      Top = 80
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 1
      Value = 10
    end
    object ListBox1: TListBox
      Left = 8
      Top = 296
      Width = 169
      Height = 97
      ItemHeight = 13
      TabOrder = 2
    end
    object CSpinEdit3: TCSpinEdit
      Left = 96
      Top = 109
      Width = 65
      Height = 22
      TabStop = True
      ParentColor = False
      TabOrder = 3
      Value = 25
    end
    object Button3: TButton
      Left = 8
      Top = 400
      Width = 121
      Height = 25
      Caption = 'Удалить провод'
      Enabled = False
      TabOrder = 4
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 8
      Top = 432
      Width = 75
      Height = 25
      Caption = 'ПУСК!'
      TabOrder = 5
      OnClick = Button4Click
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 144
      Width = 169
      Height = 137
      Caption = 'Провод:'
      TabOrder = 6
      object Label6: TLabel
        Left = 8
        Top = 76
        Width = 107
        Height = 13
        Caption = '№ текущего провода'
      end
      object Button2: TButton
        Left = 8
        Top = 104
        Width = 75
        Height = 25
        Caption = 'Добавить'
        Enabled = False
        TabOrder = 0
        OnClick = Button2Click
      end
      object CSpinEdit4: TCSpinEdit
        Left = 120
        Top = 75
        Width = 41
        Height = 22
        TabStop = True
        ParentColor = False
        TabOrder = 1
        Value = 1
      end
      object Memo1: TMemo
        Left = 8
        Top = 16
        Width = 129
        Height = 49
        TabOrder = 2
      end
    end
    object CheckBox1: TCheckBox
      Left = 8
      Top = 472
      Width = 161
      Height = 17
      Caption = 'Сохранить файлы модели'
      TabOrder = 7
    end
  end
  object ScrollBox1: TScrollBox
    Left = 185
    Top = 0
    Width = 554
    Height = 521
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
