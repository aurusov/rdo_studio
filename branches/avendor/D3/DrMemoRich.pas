unit DrMemoRich;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Plusmemo;

type

  TDrMemoRich = class(TPlusMemo)
  private
    HiStr : string;
    HiFlag: boolean;
    Par, OldStart, OldLength : integer;
    constructor Create(AOwner : TComponent); override;
  public
    RChange: boolean;
    function LoadFromFile(var Value: string): boolean;
    function SaveToFile(var Value: string): boolean;
    procedure SetHigh(start, length: integer);
    procedure ClearHigh;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Dron', [TDrMemoRich]);
end;

procedure TDrMemoRich.ClearHigh;
var flag: boolean;
begin
  if not HiFlag then exit;
  HiFlag    := false;
  RChange   := false;
  flag      := Modified;                                         
  Paragraphs[Par] := HiStr;
  SelStart  := OldStart;
  SelLength := OldLength;
  Modified  := flag;
  RChange   := true;
end;

procedure TDrMemoRich.SetHigh(start, length: integer);
var
  tempStart, tempLength: integer;
  flag: boolean;
begin
  if HiFlag then exit;
  HiFlag    := true;
  RChange   := false;
  flag      := Modified;
  tempStart := SelStart;
  tempLength:= SelLength;
  OldStart  := tempStart;
  OldLength := tempLength;
  Par       := SelLine;
  HiStr     := Paragraphs[Par];
  SelStart  := start;
  SelLength := length;
  SetHighlight;
  SelStart  := tempStart+1;
  SelLength := tempLength;
  Modified  := flag;
  RChange   := true;
end;

constructor TDrMemoRich.Create(AOwner : TComponent);
begin
  inherited;
  HiFlag  := false;
  RChange := true;
end;

function TDrMemoRich.LoadFromFile(var Value: string): boolean;
var
  fileSt: TFileStream;
begin
  try
    fileSt := TFileStream.Create(Value, fmOpenRead);
    LoadFromStream(fileSt);
    fileSt.Free;
    Result := true;
  except
    if FileExists(Value) then Result := false
                         else Result := true;
  end;
end;

function TDrMemoRich.SaveToFile(var Value: string): boolean;
var
  fileSt: TFileStream;
begin
  Result := true;
  try
    if CharCount = 0
      then DeleteFile(Value)
      else begin
        fileSt := TFileStream.Create(Value, fmCreate);
        SaveToStream(fileSt);
        fileSt.Free;
        Result := true;
      end;
  except
    Result := false;
  end;
end;

end.
