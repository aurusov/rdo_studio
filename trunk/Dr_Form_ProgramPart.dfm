inherited ProgramPartForm: TProgramPartForm
  Tag = 5
  Left = 338
  Top = 242
  Caption = 'Дополнение к программе'
  ClientHeight = 167
  ClientWidth = 234
  OldCreateOrder = True
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  inherited ShadowS: TShape
    Width = 234
    Height = 146
  end
  inherited BorderS: TShape
    Width = 234
    Height = 146
  end
  inherited CaptionS: TShape
    Width = 234
  end
  inherited ExitB: TSpeedButton
    Left = 208
  end
  object CorrectionDateL: TLabel [5]
    Left = 20
    Top = 48
    Width = 26
    Height = 13
    Caption = 'Дата'
  end
  object CorrectionDateS: TShape [6]
    Left = 70
    Top = 46
    Width = 150
    Height = 19
    Brush.Color = clTeal
  end
  object LockPart: TDBCheckBox [8]
    Left = 18
    Top = 76
    Width = 179
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Больше не добавлять заказы'
    DataField = 'LockPart'
    DataSource = Data.ProgramPartDS
    TabOrder = 1
    ValueChecked = 'True'
    ValueUnchecked = 'False'
  end
  object OkB: TDrButton [9]
    Left = 38
    Top = 120
    Width = 75
    Height = 25
    Flat = True
    FlatColor = clActiveCaption
    Caption = 'Ok'
    TabOrder = 2
    TabStop = True
    OnClick = OkBClick
  end
  object CancelB: TDrButton [10]
    Left = 126
    Top = 120
    Width = 75
    Height = 25
    Flat = True
    FlatColor = clActiveCaption
    Caption = 'Cancel'
    TabOrder = 3
    TabStop = True
    OnClick = CancelBClick
  end
  object CorrectionDate: TDBEdit [11]
    Left = 68
    Top = 44
    Width = 150
    Height = 19
    Ctl3D = False
    DataField = 'CorrectionDate'
    DataSource = Data.ProgramPartDS
    ParentCtl3D = False
    TabOrder = 0
  end
end
