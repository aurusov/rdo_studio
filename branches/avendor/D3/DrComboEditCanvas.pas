unit DrComboEditCanvas;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Mask, ToolEdit;

type

  TDrComboEditCanvas = class(TComboEdit)
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
  RegisterComponents('Dron', [TDrComboEditCanvas]);
end;

constructor TDrComboEditCanvas.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FCanvas := TControlCanvas.Create;
  TControlCanvas(FCanvas).Control := Self;
end;

destructor TDrComboEditCanvas.Destroy;
begin
  FCanvas.Free;
  inherited Destroy;
end;

end.
