unit DrScrollBoxCanvas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;

type
  TDrScrollBoxCanvas = class(TScrollBox)
  private
    FCanvas: TCanvas;
    procedure WMPaint(var M:TWMPaint); message WM_PAINT;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
    property Canvas: TCanvas read FCanvas;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Dron', [TDrScrollBoxCanvas]);
end;

procedure TDrScrollBoxCanvas.WMPaint(var M:TWMPaint);
begin
  inherited;
  if Assigned(OnResize) then OnResize(Self);
end;

constructor TDrScrollBoxCanvas.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FCanvas := TControlCanvas.Create;
  TControlCanvas(FCanvas).Control := Self;
end;

destructor TDrScrollBoxCanvas.Destroy;
begin
  FCanvas.Free;
  inherited Destroy;
end;

end.

