unit DrSpinEditCanvas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, RXSpin;

type

  TDrSpinEditCanvas = class(TRxSpinEdit)
  private
    FCanvas: TCanvas;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
    property Canvas: TCanvas read FCanvas;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Dron', [TDrSpinEditCanvas]);
end;

constructor TDrSpinEditCanvas.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FCanvas := TControlCanvas.Create;
  TControlCanvas(FCanvas).Control := Self;
end;

destructor TDrSpinEditCanvas.Destroy;
begin
  FCanvas.Free;
  inherited Destroy;
end;

end.
