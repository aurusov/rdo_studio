unit DrSpeedButton;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  RXCtrls;

type
  TDrSpeedButton = class(TRxSpeedButton)
  private
    FColor, FTempColor: TColor;
    FOnEnabled, FOnMouseEnter, FOnMouseLeave: TNotifyEvent;
    function  GetEnabled: boolean;
    procedure SetEnabled(Value: boolean);
    procedure CMMouseEnter(var msg: TMessage); message CM_MOUSEENTER;
    procedure CMMouseLeave(var msg: TMessage); message CM_MOUSELEAVE;
  public
    constructor Create(Sender: TComponent); override;
  published
    property CaptionDownColor: TColor read FColor write FColor;
    property Enabled: boolean read GetEnabled write SetEnabled;
    property OnEnabled: TNotifyEvent read FOnEnabled write FOnEnabled;
    property OnMouseEnter: TNotifyEvent read FOnMouseEnter write FOnMouseEnter;
    property OnMouseLeave: TNotifyEvent read FOnMouseLeave write FOnMouseLeave;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Dron', [TDrSpeedButton]);
end;

constructor TDrSpeedButton.Create(Sender: TComponent);
begin
  inherited Create(Sender);
  Height := 40;
  Width  := 76;
  Flat   := true;
  FColor := clNavy;
  FTempColor := Font.Color;
end;

function TDrSpeedButton.GetEnabled: boolean;
begin
  Result := inherited Enabled;
end;

procedure TDrSpeedButton.SetEnabled(Value: boolean);
begin
  inherited Enabled := Value;
  if Assigned(OnEnabled) then OnEnabled(Self);
end;

procedure TDrSpeedButton.CMMouseEnter(var msg: TMessage);
begin
  inherited;
  FTempColor := Font.Color;
  Font.Color := CaptionDownColor;
  if Assigned(OnMouseEnter) then OnMouseEnter(Self);
end;

procedure TDrSpeedButton.CMMouseLeave(var msg: TMessage);
begin
  inherited;
  Font.Color := FTempColor;
  if Assigned(OnMouseLeave) then OnMouseLeave(Self);
end;

end.
