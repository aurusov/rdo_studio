unit Var_Unit;

interface

uses Controls, Windows;

type

  TModelLow = (mlUniform, mlNormal, mlExponential, mlConst);
  TOnlyTUPath = (tpNone, tpAll, tpNumber);

  TShowModelResult = procedure(Value: string);

  THelpContextPopupInfo = record
    PopupHelpContext,
    DetailHelpContext : integer;
    sPos              : TSmallPoint;
  end;

var
  ShowText: boolean = true;
  HelpContextPopupInfo: THelpContextPopupInfo;

implementation

initialization
  with HelpContextPopupInfo do
  begin
    PopupHelpContext  := 0;
    DetailHelpContext := 0;
    sPos.x            := 0;
    sPos.y            := 0;
  end;

end.
