unit DrEditCanvas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type

  TDrEditCanvas = class(TEdit)
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
  RegisterComponents('Dron', [TDrEditCanvas]);
end;

constructor TDrEditCanvas.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FCanvas := TControlCanvas.Create;
  TControlCanvas(FCanvas).Control := Self;
end;

destructor TDrEditCanvas.Destroy;
begin
  FCanvas.Free;
  inherited Destroy;
end;

end.
