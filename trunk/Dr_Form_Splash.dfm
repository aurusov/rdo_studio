inherited SplashForm: TSplashForm
  Left = 268
  Top = 236
  Caption = 'ѕрогресс расчета сменно-суточного задани€ на %s'
  ClientHeight = 85
  ClientWidth = 435
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  inherited ShadowS: TShape
    Width = 435
    Height = 64
  end
  inherited BorderS: TShape
    Width = 435
    Height = 64
  end
  inherited CaptionS: TShape
    Width = 435
  end
  inherited ExitB: TSpeedButton
    Visible = False
  end
  object ProgressBar: TProgressBar [5]
    Left = 28
    Top = 44
    Width = 380
    Height = 16
    Min = 0
    Max = 100
    TabOrder = 0
  end
  inherited ClosePM: TPopupMenu
    inherited CloseM: TMenuItem
      Enabled = False
    end
  end
end
