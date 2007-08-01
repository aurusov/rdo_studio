unit Result_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons;

type
  TResultForm = class(TForm)
    ResultPanel: TPanel;
    SostL: TLabel;
    SostT: TLabel;
    ModTimeL: TLabel;
    TimeL: TLabel;
    MemoryL: TLabel;
    MoneyL: TLabel;
    OpenTopL: TLabel;
    TopL: TLabel;
    TopInL: TLabel;
    TopChildL: TLabel;
    ModTimeT: TLabel;
    TimeT: TLabel;
    MemoryT: TLabel;
    MoneyT: TLabel;
    OpenTopT: TLabel;
    TopT: TLabel;
    TopInT: TLabel;
    TopChildT: TLabel;
    Bevel: TBevel;
    WaitPanel: TPanel;
    ResultImage: TImage;
    OkButton: TSpeedButton;
    BevelOk: TBevel;
    procedure OkButtonClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  ResultForm: TResultForm;

implementation

{$R *.DFM}

procedure TResultForm.OkButtonClick(Sender: TObject);
begin
  Close;
end;

end.
