inherited PlaneInfo: TPlaneInfo
  Tag = 12
  Left = 260
  Top = 190
  Caption = 'Параметры расчета сменно-суточных заданий'
  ClientHeight = 268
  ClientWidth = 358
  OldCreateOrder = True
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  inherited ShadowS: TShape
    Width = 358
    Height = 247
  end
  inherited BorderS: TShape
    Width = 358
    Height = 247
  end
  object KoefBevel: TBevel [2]
    Left = 15
    Top = 90
    Width = 197
    Height = 2
  end
  inherited CaptionS: TShape
    Width = 358
  end
  object PrecisionL1: TLabel [6]
    Left = 36
    Top = 58
    Width = 48
    Height = 13
    Caption = 'Скорость'
  end
  object PrecisionL2: TLabel [7]
    Left = 188
    Top = 58
    Width = 47
    Height = 13
    Caption = 'Точность'
  end
  object PrecisionL3: TLabel [8]
    Left = 244
    Top = 58
    Width = 8
    Height = 13
    Caption = '%'
  end
  object KoefL: TLabel [9]
    Left = 24
    Top = 84
    Width = 131
    Height = 13
    Caption = ' Весовые коэффициенты '
  end
  object Koef1S: TShape [10]
    Left = 138
    Top = 106
    Width = 100
    Height = 19
    Brush.Color = clOlive
  end
  object Koef2S: TShape [11]
    Left = 138
    Top = 130
    Width = 100
    Height = 19
    Brush.Color = clOlive
  end
  object Koef3S: TShape [12]
    Left = 138
    Top = 154
    Width = 100
    Height = 19
    Brush.Color = clOlive
  end
  object Koef4S: TShape [13]
    Left = 138
    Top = 178
    Width = 100
    Height = 19
    Brush.Color = clOlive
  end
  object Koef1L: TLabel [14]
    Left = 20
    Top = 108
    Width = 53
    Height = 13
    Caption = 'Срочность'
  end
  object Koef2L: TLabel [15]
    Left = 20
    Top = 132
    Width = 81
    Height = 13
    Caption = 'Объем выпуска'
  end
  object Koef3L: TLabel [16]
    Left = 20
    Top = 156
    Width = 107
    Height = 13
    Caption = 'Время вулканизации'
  end
  object Koef4L: TLabel [17]
    Left = 20
    Top = 180
    Width = 76
    Height = 13
    Caption = 'Вемя загрузки'
  end
  object OkB: TDrButton [19]
    Left = 68
    Top = 224
    Width = 75
    Height = 25
    Flat = True
    FlatColor = clActiveCaption
    Caption = 'Ok'
    TabOrder = 6
    TabStop = True
    OnClick = OkBClick
  end
  object CancelB: TDrButton [20]
    Left = 189
    Top = 224
    Width = 75
    Height = 25
    Flat = True
    FlatColor = clActiveCaption
    Caption = 'Cancel'
    TabOrder = 7
    TabStop = True
    OnClick = CancelBClick
  end
  object Koef1E: TEdit [21]
    Left = 136
    Top = 104
    Width = 100
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 1
    Text = '0'
  end
  object Koef2E: TEdit [22]
    Left = 136
    Top = 128
    Width = 100
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 2
    Text = '0'
  end
  object Koef3E: TEdit [23]
    Left = 136
    Top = 152
    Width = 100
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 3
    Text = '0'
  end
  object Koef4E: TEdit [24]
    Left = 136
    Top = 176
    Width = 100
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 4
    Text = '0'
  end
  object KoefLoadB: TDrButton [25]
    Left = 244
    Top = 172
    Width = 97
    Height = 25
    Flat = True
    FlatColor = clActiveCaption
    Caption = 'Импортировать'
    TabOrder = 5
    TabStop = True
    OnClick = KoefLoadBClick
  end
  object PrecisionTB: TTrackBar [26]
    Left = 64
    Top = 36
    Width = 150
    Height = 21
    Max = 100
    Orientation = trHorizontal
    PageSize = 10
    Frequency = 10
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 0
    ThumbLength = 10
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = PrecisionTBChange
  end
end
