unit Misc_Unit;

interface

uses Windows, Var_Unit, SysUtils, Controls;

type

  FMSError = class(Exception)
  public
    constructor CreateStop(const Msg: string);
    constructor CreateStopRes(Ident: Integer);
    constructor CreateStopResFmt(Ident: Integer; const Args: array of const);
    constructor CreateStopSubError(Res: integer; Value: string);
    constructor CreateSubError(Res: integer; Value: string);
  end;

  function GetNormalDir(Value: string): string;
  function GetPopupHelpContext(Value: integer): integer;
  function GetDetailHelpContext(Value: integer): integer;
  function ShowInfoMessage(Value: string): boolean;
  function ShowQueryMessage(Value: string; Flag: UINT): integer;
  function ShowErrorMessage(Value: string): boolean;
  function GetModelLow(Value: string): TModelLow;
  procedure GetMinMaxMetrics(var minX, minY, maxX, maxY: double);
  function dwCheckError(Value: DWORD; Res: integer): DWORD;
  function pCheckError(Value: Pointer; Res: integer): Pointer;
  function GetWinControlFromPoint(pos: TPoint; var sPos: TSmallPoint): TWinControl;
  function isExistPropValue(tempObject: TObject; PropName: string): boolean;

implementation

uses FMS_Unit, FMSObjects_Unit, Const_Unit, ConstRes_Unit, Forms, TypInfo;

var bStopIcon: boolean = false;

constructor FMSError.CreateStop(const Msg: string);
begin
  bStopIcon := true;
  Create(Msg);
end;

constructor FMSError.CreateStopRes(Ident: Integer);
begin
  bStopIcon := true;
  CreateRes(Ident);
end;

constructor FMSError.CreateStopResFmt(Ident: Integer; const Args: array of const);
begin
  bStopIcon := true;
  CreateResFmt(Ident, Args);
end;

constructor FMSError.CreateStopSubError(Res: integer; Value: string);
begin
  bStopIcon := true;
  CreateSubError(Res, Value);
end;

constructor FMSError.CreateSubError(Res: integer; Value: string);
begin
  Create(Format('%s:%s%s', [LoadStr(Res), MyEOL, Value]));
end;

function GetNormalDir(Value: string): string;
begin
  Value  := ExtractFileDir(Value);
  Result := Value;
  if Value <> '' then if Value[Length(Value)] <> '\' then Result := Value + '\';
end;

function GetPopupHelpContext(Value: integer): integer;
begin
  Result := Value and $FFFF;
end;

function GetDetailHelpContext(Value: integer): integer;
begin
  Result := (Value shr 16) and $FFFF;
end;

function dwCheckError(Value: DWORD; Res: integer): DWORD;
begin
  Result := Value;
  if Pointer(Value) = nil then
    raise FMSError.CreateResFmt(ErrorFunction, [LoadStr(Res)]);
end;

function pCheckError(Value: Pointer; Res: integer): Pointer;
begin
  Result := Value;
  if Value = nil then
    raise FMSError.CreateResFmt(ErrorFunction, [LoadStr(Res)]);
end;

function ShowInfoMessage(Value: string): boolean;
begin
  Application.MessageBox(PChar(Value+'.'), PChar(LoadStr(MainCaption)), MB_ICONINFORMATION+MB_OK);
  Result := false;
end;

function ShowQueryMessage(Value: string; Flag: UINT): integer;
begin
  Result := Application.MessageBox(PChar(Value+'?'), PChar(LoadStr(MainCaption)), MB_ICONQUESTION+Flag);
end;

function ShowErrorMessage(Value: string): boolean;
var flag: UINT;
begin
  flag := MB_OK;
  if bStopIcon then flag := flag + MB_ICONSTOP
               else flag := flag + MB_ICONWARNING;
  Application.MessageBox(PChar(Value+'.'), PChar(LoadStr(MainCaption)), flag);
  bStopIcon := false;
  Result := false;
end;

function GetModelLow(Value: string): TModelLow;
begin
  Value := LowerCase(Value);
  if Value = 'uniform' then Result := mlUniform
    else if Value = 'normal' then Result := mlNormal
      else if Value = 'exponential' then Result := mlExponential
        else Result := mlConst;
end;

procedure GetMinMaxMetrics(var minX, minY, maxX, maxY: double);
var i: integer;

  procedure CheckMinMax(X1, Y1, X2, Y2: double);
  begin
    if X1 < minX then minX := X1;
    if Y1 < minY then minY := Y1;
    if X1 + X2 > maxX then maxX := X1 + X2;
    if Y1 + Y2 > maxY then maxY := Y1 + Y2;
  end;

begin
  for i := 0 to OTOs.Count-1 do
    with OTOs[i] do CheckMinMax(otoX, otoY, otoDX, otoDY);
  for i := 0 to TUs.Count-1 do
    with TUs[i] do CheckMinMax(tuX, tuY, tuDX, tuDY);
  for i := 0 to Sklads.Count-1 do
    with Sklads[i] do CheckMinMax(skladX, skladY, skladDX, skladDY);
  with FMSPoint do
    for i := 0 to Count-1 do
      CheckMinMax(PointCoor[i, 0], PointCoor[i, 1], 0, 0);
end;

function GetWinControlFromPoint(pos: TPoint; var sPos: TSmallPoint): TWinControl;
var
  Handle1, Handle2, Handle3: HWND;
  r: TRect;

{  procedure ShowWinText(Value: HWND; st: string);
  var
    s, s2: string;
    l: integer;
  begin
    exit;
    s  := 'nil';
    s2 := 'nil';
    if FindControl(Value) <> nil then s := FindControl(Value).Name;
    if Value <> HWND(nil) then
    begin
      l := GetWindowTextLength(Value);
      if l > 0 then begin
        SetLength(s2, l);
        GetWindowText(Value, PChar(s2), l+1);
      end;
    end;
    ShowInfoMessage(Format('%s %d %d Handle=%x, Name=''%s'', Text=''%s''', [st, pos.x, pos.y, Value, s, s2]));
  end;}

begin
  Result := nil;
  if (pos.x <= High(SmallInt)) and (pos.y <= High(SmallInt)) then
  begin
    sPos    := PointToSmallPoint(pos);
    Handle1 := WindowFromPoint(pos);
    Handle2 := Handle1;
//    ShowWinText(Handle1, 'begin');
    while Handle2 <> HWND(nil) do
    begin
      Handle1 := Handle2;
      if GetWindowRect(Handle2, r) then
      begin
        pos.x   := pos.x - r.Left;
        pos.y   := pos.y - r.Top;
        Handle3 := ChildWindowFromPointEx(Handle2, pos, CWP_SKIPINVISIBLE);
//        ShowWinText(Handle3, 'loop');
        if Handle2 = Handle3 then Handle2 := HWND(nil)
                             else Handle2 := Handle3;
      end else Handle2 := HWND(nil);
    end;
//    ShowWinText(Handle1, 'select');
    Result := FindControl(Handle1);
  end;
end;

function isExistPropValue(tempObject: TObject; PropName: string): boolean;
var PropInfo: PPropInfo;
begin
  Result := false;
  PropInfo := GetPropInfo(tempObject.ClassInfo, PropName);
  if PropInfo <> nil then
  begin
    case PropInfo^.PropType^.Kind of
      tkInteger    : Result := GetOrdProp(tempObject, PropInfo) <> 0;
      tkEnumeration: Result := GetOrdProp(tempObject, PropInfo) <> 0;
      tkFloat      : Result := GetFloatProp(tempObject, PropInfo) <> 0;
      tkString,
      tkLString,
      tkWString    : Result := GetStrProp(tempObject, PropInfo) <> '';
      tkClass      : Result := GetOrdProp(tempObject, PropInfo) <> 0;
      tkMethod     : Result := GetOrdProp(tempObject, PropInfo) <> 0;
    end;
  end;
end;

{
procedure GraphSearch;
var i, j, FarCount, FinishPoint: integer;

  function GetFarRelation(i, j: integer): integer;
  var k: integer;
  begin
    Result := 0;
    try
      inc(FarCount);
      Application.ProcessMessages;
      with FMSPoint do
      begin
        for k := 0 to Count-1 do
        begin
          if k <> FinishPoint then
          begin
            if FarCount < FMSPoint.Count then
            begin
              if PointRel[k, FinishPoint] = 0
                then Result := GetFarRelation(k, FinishPoint)
                else Result := -1;
            end else begin Result := 0; end;
//            PointRel[k, FinishPoint] := Result;
          end;
        end;
      end;
    except
      ShowMessage('Oops');
    end;
  end;

begin
  with FMSPoint do
  begin
    for i := 0 to Count-2 do
      for j := i+1 to Count-1 do
      begin
        FarCount    := 0;
        FinishPoint := j;
        if (i <> FinishPoint) and (PointRel[i, FinishPoint] = 0) then
          PointRel[i, FinishPoint] := GetFarRelation(i, FinishPoint);
      end;
  end;
end;
}

end.
