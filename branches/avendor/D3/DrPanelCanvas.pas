unit DrPanelCanvas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

type

  TDrPanelCanvas = class(TPanel)
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
  RegisterComponents('Dron', [TDrPanelCanvas]);
end;

constructor TDrPanelCanvas.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FCanvas := TControlCanvas.Create;
  TControlCanvas(FCanvas).Control := Self;
end;

destructor TDrPanelCanvas.Destroy;
begin
  FCanvas.Free;
  inherited Destroy;
end;

end.

