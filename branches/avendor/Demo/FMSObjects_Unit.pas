unit FMSObjects_Unit;

interface

uses Windows, SysUtils, Classes, Graphics, ComCtrls, Var_Unit;

type

  // ---- Базовый объект
  TFMSObject = class(TCollectionItem)
  private
    FMSBitmap, FMSMaskBitmap: TBitmap;
    FName: string;
    BossTreeNode: TTreeNode;
    function ExistRDOName(Value: string; Node: TTreeNode): boolean;
    procedure SetName(Value: string);
  public
    ItemID: integer;
    doFree: boolean;
  {$WARNINGS OFF}
    constructor Create(ParentCollection: TCollection; Value: string);
    destructor  Destroy; override;
    function PictureExist: boolean;
    procedure DrawPicture(W, H: double; doScale: boolean);
    procedure DrawPictureInRect(r: TRect; DstDC: HDC);
    procedure ReadPicture(ResName: string);
    procedure SavePicture;
    property Name: string read FName write SetName;
  end;
  {$WARNINGS ON}

  TFMSObjects = class(TCollection)
  public
    function IndexOf(Value: TFMSObject): integer;
  end;

  // ---- ОТО
  TOTO = class(TFMSObject)
  public
    RLow: TModelLow;
    RLowValue1, RLowValue2, RTimeMin, RTimeMax: double;
    otoX, otoY, otoDX, otoDY, nakopX, nakopY: double;
    nakopValue, nakopPointNumber: integer;
    pOtoBusy, pOtoWait, pOtoRepair, pNakopBusy, pNakopWait: boolean;
    constructor Create(Value: string);
  end;

  TOTOs = class(TFMSObjects)
  private
    function GetObject(Index: integer): TOTO;
    procedure SetObject(Index: integer; Value: TOTO);
  public
    property Obj[Index: integer]: TOTO read GetObject write SetObject; default;
  end;

  // ---- ТУ
  TTU = class(TFMSObject)
  public
    Speed, TimeLoad, tuX, tuY, tuDX, tuDY: double;
    OTOPos: TFMSObject;
    PredPointNumber: integer;
    pTuTransport, pTuFree, pTuEmpty, pTuLoad: boolean;
    constructor Create(Value: string);
  end;

  TTUs = class(TFMSObjects)
  private
    function GetObject(Index: integer): TTU;
    procedure SetObject(Index: integer; Value: TTU);
  public
    property Obj[Index: integer]: TTU read GetObject write SetObject; default;
  end;

  // ---- Склад
  TSklad = class(TFMSObject)
  public
    skladX, skladY, skladDX, skladDY: double;
    skladValue, skladPointNumber: integer;
    pNakopBusy, pNakopWait: boolean;
  end;

  TSklads = class(TFMSObjects)
  private
    function GetObject(Index: integer): TSklad;
    procedure SetObject(Index: integer; Value: TSklad);
  public
    property Obj[Index: integer]: TSklad read GetObject write SetObject; default;
  end;

  // ---- Номенклатура
  TTP = record
    OTO: TFMSObject;
    Time: double;
  end;

  TNomenk = class(TFMSObject)
  private
    TPList: TList;
    function GetTP(Index: integer): TTP;
  public
    Low: TModelLow;
    LowValue1, LowValue2: double;
    AllValue, StartParty, WorkParty: integer;
    constructor Create(Value: string);
    destructor Destroy; override;
    function TPCount: integer;
    procedure AddTP(Value: Pointer);
    property TP[Index: integer]: TTP read GetTP;
  end;

  TNomenks = class(TFMSObjects)
  private
    function GetObject(Index: integer): TNomenk;
    procedure SetObject(Index: integer; Value: TNomenk);
  public
    property Obj[Index: integer]: TNomenk read GetObject write SetObject; default;
  end;

  // ---- Узлы
  TFMSPoint = record
    Count: integer;
    PointCoor: Variant;
    PointRel: Variant;
  end;

  // ---- Константы
  TFMSConst = record
    Scale, dX, dY, minX, minY, maxX, maxY, WorkTime: double;
    doWorkTime: boolean;
  end;

var
  OTOs    : TOTOs;
  TUs     : TTUs;
  Sklads  : TSklads;
  Nomenks : TNomenks;
  ActiveFMSObject: TFMSObject;
  FMSPoint: TFMSPoint;
  FMSConst: TFMSConst;

implementation

uses FMS_Unit, Const_Unit, ConstRes_Unit, Misc_Unit;

//---------------------------------
//            FMSObject
//---------------------------------
constructor TFMSObject.Create(ParentCollection: TCollection; Value: string);
var
  i, j: integer;
  s: string;
  tempNode: TTreeNode;
  tempList: TCollection;
  flag: boolean;
begin
  if Self is TOTO then i := TVOTO
    else if Self is TTU then i := TVTU
      else if Self is TSklad then i := TVSklad
        else if Self is TNomenk then i := TVNomenk
          else raise FMSError.CreateRes(ErrorCreateFMSClass);
  inherited Create(ParentCollection);
  doFree := false;
  case i of
    TVOTO:    begin tempNode := OTOTreeNode;    tempList := OTOs; end;
    TVTU:     begin tempNode := TUTreeNode;     tempList := TUs; end;
    TVSklad:  begin tempNode := SkladTreeNode;  tempList := Sklads; end;
    TVNomenk: begin tempNode := NomenkTreeNode; tempList := Nomenks; end;
    else raise FMSError.CreateRes(ErrorCreateFMSClass);
  end;
  Value := Trim(Value);
  if Value = '' then
  begin
    case i of
      TVOTO:    Value := LoadStr(OTONewNameStr);
      TVTU:     Value := LoadStr(TUNewNameStr);
      TVSklad:  Value := LoadStr(SkladNewNameStr);
      TVNomenk: Value := LoadStr(NomenkNewNameStr);
    end;
    s := Value;
    j := 1;
    while ExistRDOName(Value, tempNode) do
    begin
      inc(j);
      Value := s+IntToStr(j);
    end;
  end;
  // Создать ID (внутри одного типа)
  if i = TVSklad then ItemID := -1 else ItemID := 1;
  repeat
    flag := true;
    for j := 0 to tempList.Count - 2 do
      if TFMSObject(tempList.Items[j]).ItemID = ItemID then
      begin
        if i = TVSklad then dec(ItemID) else inc(ItemID);
        flag := false;
      end;
  until flag;
  FMSBitmap := TBitmap.Create;
  FMSBitmap.Transparent := false;
  FMSMaskBitmap := TBitmap.Create;
  FMSMaskBitmap.Transparent := false;
  BossTreeNode := FMSForm.TreeView.Items.AddChildObject(tempNode, Value, Self);
  BossTreeNode.ImageIndex    := 2;
  BossTreeNode.SelectedIndex := 3;
  Name := Value;
  FMSForm.PlaneChange := true;
end;

destructor TFMSObject.Destroy;
begin
  doFree := true;
  FMSBitmap.Free;
  FMSMaskBitmap.Free;
  BossTreeNode.Free;
  inherited Destroy;
  FMSForm.PlaneChange := true;
  FMSForm.UpDateFMSObject;
end;

function TFMSObject.ExistRDOName(Value: string; Node: TTreeNode): boolean;
var i: integer;
begin
  Result := false;
  with Node do
    for i := 0 to Count-1 do
      if UpperCase(TFMSObject(Item[i].Data).Name) = UpperCase(Value) then
      begin
        Result := true;
        exit;
      end;
end;

procedure TFMSObject.SetName(Value: string);
begin
  if FName <> Value then
  begin
    FName := Value;
    BossTreeNode.Text   := Value;
    FMSForm.PlaneChange := true;
  end;
end;

function TFMSObject.PictureExist: boolean;
begin
  Result := not ((FMSBitmap.Width = 0) or (FMSBitmap.Height = 0));
end;

procedure TFMSObject.DrawPicture(W, H: double; doScale: boolean);
begin
  try
    with FMSForm.PicImage do
    begin
      Picture.Bitmap.Assign(nil);
      if PictureExist then
      begin
        if doScale then
        begin
          Width   := Round(W * FMSConst.Scale);
          Height  := Round(H * FMSConst.Scale);
        end else begin
          Width   := FMSBitmap.Width;
          Height  := FMSBitmap.Height;
        end;
        Picture.Bitmap.Width  := Width;
        Picture.Bitmap.Height := Height;
        with Picture.Bitmap do
        begin
          Canvas.Brush.Color := FMSForm.Color;
          Canvas.Brush.Style := bsSolid;
          Canvas.FillRect(Rect(0, 0, Width, Height));
          DrawPictureInRect(Rect(0, 0, Width, Height), Canvas.Handle);
//          Picture.Bitmap.Canvas.Pixels[0, Picture.Bitmap.Height-1] := clWhite;
        end;
        Left    := (Parent.Width  - Width)  div 2;
        Top     := (Parent.Height - Height) div 2;
        if Left < 0 then Left := 0;
        if Top  < 0 then Top  := 0;
        FMSForm.PicImagePanel.Caption := '';
        FMSForm.PicImageZoomButton.Visible := (Width > Parent.Width) or (Height > Parent.Height);
      end else begin
        FMSForm.PicImagePanel.Caption := 'Рисунок отсутствует';
        FMSForm.PicImageZoomButton.Visible := false;
      end;
    end;
  except
    FMSForm.PicImagePanel.Caption := 'Ошибка отображения';
    FMSForm.PicImageZoomButton.Visible := false;
    raise FMSError.CreateResFmt(ErrorDrawBitmap, [Name]);
  end;
end;

procedure TFMSObject.DrawPictureInRect(r: TRect; DstDC: HDC);
begin
  TransparentStretchBlt(
    DstDC,
    r.Left, r.Top, r.Right-r.Left, r.Bottom-r.Top,

    FMSBitmap.Canvas.Handle,
    0, 0, FMSBitmap.Width, FMSBitmap.Height,

    FMSMaskBitmap.Canvas.Handle,
    0, 0);
end;

{
procedure TFMSObject.DrawPictureInRect(r: TRect; DstDC: HDC);
var
  SrcDC, MaskDC, MemoryDC: HDC;
  MemoryBitmap: HBITMAP;
  tempHandle  : THandle;
  tempPalette : HPALETTE;
  crText, crBack: TColorRef;
begin
  SrcDC  := FMSBitmap.Canvas.Handle;
  MaskDC := FMSMaskBitmap.Canvas.Handle;
  tempPalette := 0;
  MemoryDC    := CreateCompatibleDC(0);
  try
    MemoryBitmap := CreateCompatibleBitmap(SrcDC, FMSBitmap.Width, FMSBitmap.Height);
    try
      tempHandle  := SelectObject(MemoryDC, MemoryBitmap);
      tempPalette := SelectPalette(SrcDC, SystemPalette16, False);
      SelectPalette(SrcDC, tempPalette, False);
      if tempPalette <> 0
        then tempPalette := SelectPalette(MemoryDC, tempPalette, True)
        else tempPalette := SelectPalette(MemoryDC, SystemPalette16, True);
      RealizePalette(MemoryDC);

      StretchBlt(MemoryDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height,   MaskDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height, SrcCopy);
      StretchBlt(MemoryDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height,    SrcDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height, SrcErase);
      crText := SetTextColor(DstDC, $0);
      crBack := SetBkColor(DstDC, $FFFFFF);
      StretchBlt(DstDC   , r.Left, r.Top, r.Right-r.Left, r.Bottom-r.Top, MaskDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height, SrcAnd);
      StretchBlt(DstDC   , r.Left, r.Top, r.Right-r.Left, r.Bottom-r.Top, MemoryDC, 0, 0, FMSBitmap.Width, FMSBitmap.Height, SrcInvert);
      SetTextColor(DstDC, crText);
      SetTextColor(DstDC, crBack);

    finally
      if tempHandle  <> 0 then SelectObject(MemoryDC, tempHandle);
      DeleteObject(MemoryBitmap);
    end;
  finally
    if tempPalette <> 0 then SelectPalette(MemoryDC, tempPalette, False);
    DeleteDC(MemoryDC);
  end;
end;
}

procedure TFMSObject.ReadPicture(ResName: string);
var
  i, j: integer;
  TransparentColor: TColor;
  r1, r2: TRect;
begin
  try
    if PictureExist then exit;
    FMSBitmap.LoadFromResourceName(0, ResName);
    FMSMaskBitmap.Assign(FMSBitmap);
    // Меняем TransparentColor на clBlack
    TransparentColor := FMSBitmap.Canvas.Pixels[0, FMSBitmap.Height-1];
    if TransparentColor <> clBlack then
      with FMSBitmap.Canvas do
        for i := 0 to FMSBitmap.Width-1 do
          for j := 0 to FMSBitmap.Height-1 do
            if Pixels[i, j] = TransparentColor then Pixels[i, j] := clBlack;
    // Создаем маску
    with FMSMaskBitmap do
    begin
      Mask(TransparentColor);
      Canvas.CopyMode := cmSrcAnd;
      r2 := Rect(0, 0, Width, Height);
      r1 := Rect(-1, -1, r2.Right-1, r2.Bottom-1);
      Canvas.CopyRect(r1, Canvas, r2);
      r1 := Rect(1, -1, Width+1, Height-1);
      Canvas.CopyRect(r1, Canvas, r2);
      r1 := Rect(1, 1, Width+1, Height+1);
      Canvas.CopyRect(r1, Canvas, r2);
      r1 := Rect(-1, 1, Width-1, Height+1);
      Canvas.CopyRect(r1, Canvas, r2);
      Canvas.Pixels[0, Height-1] := clWhite;
    end;
  except
    FMSBitmap.Assign(nil);
    FMSMaskBitmap.Assign(nil);
    raise FMSError.CreateResFmt(ErrorLoadBitmap, [Name]);
  end;
end;

procedure TFMSObject.SavePicture;
var
  s: string;
begin
  try
    s := '';
    if ClassType = TOTO then s := 'oto' else
      if ClassType = TTU then s := 'tu' else
        if ClassType = TSklad then s := 'sklad' else
          if ClassType = TNomenk then s := 'det';
    s := s + IntToStr((Collection as TFMSObjects).IndexOf(Self)+1);
    FMSBitmap.SaveToFile(FMSForm.FMSModelPath + s + 'p.bmp');
    FMSMaskBitmap.SaveToFile(FMSForm.FMSModelPath + s + 'm.bmp');
  except
    raise FMSError.CreateResFmt(ErrorSaveBitmap, [Name]);
  end;
end;

// Collection
function TFMSObjects.IndexOf(Value: TFMSObject): integer;
begin
  Result := 0;
  while (Result < Count) and (Items[Result] <> Value) do Inc(Result);
  if Result = Count then Result := -1;
end;

//---------------------------------
//            OTO
//---------------------------------
constructor TOTO.Create(Value: string);
begin
  inherited Create(OTOs, Value);
  RLow             := mlUniform;
  RLowValue1       := 10;
  RLowValue2       := 50;
  RTimeMin         := 3;
  RTimeMax         := 5;
  otoX             := 1;
  otoY             := 1;
  otoDX            := 3;
  otoDY            := 2;
  nakopValue       := 25;
  nakopX           := 2;
  nakopY           := 2;
  nakopPointNumber := 1;
  pOtoBusy         := false;
  pOtoWait         := false;
  pOtoRepair       := false;
  pNakopBusy       := false;
  pNakopWait       := false;
end;

// Collection
function TOTOs.GetObject(Index: integer): TOTO;
begin
  Result := (inherited Items[Index]) as TOTO;
end;

procedure TOTOs.SetObject(Index: integer; Value: TOTO);
begin
  inherited Items[Index] := Value;
end;

//---------------------------------
//            TU
//---------------------------------
constructor TTU.Create(Value: string);
begin
  inherited Create(TUs, Value);
  Speed           := 30;
  TimeLoad        := 0.1;
  tuX             := 4;
  tuY             := 4;
  tuDX            := 1;
  tuDY            := 1;
  PredPointNumber := 1;
  OTOPos          := nil;
  pTuTransport    := false;
  pTuFree         := false;
  pTuEmpty        := false;
  pTuLoad         := false;
end;

// Collection
function TTUs.GetObject(Index: integer): TTU;
begin
  Result := (inherited Items[Index]) as TTU;
end;

procedure TTUs.SetObject(Index: integer; Value: TTU);
begin
  inherited Items[Index] := Value;
end;

//---------------------------------
//            Sklad
//---------------------------------
// Collection
function TSklads.GetObject(Index: integer): TSklad;
begin
  Result := (inherited Items[Index]) as TSklad;
end;

procedure TSklads.SetObject(Index: integer; Value: TSklad);
begin
  inherited Items[Index] := Value;
end;

//---------------------------------
//            Nomenk
//---------------------------------
constructor TNomenk.Create(Value: string);
begin
  inherited Create(Nomenks, Value);
  TPList := TList.Create;
  AllValue   := 40;
  StartParty := 10;
  WorkParty  := 4;
  Low        := mlUniform;
  LowValue1  := 10;
  LowValue2  := 35;
end;

destructor TNomenk.Destroy;
var i: integer;
begin
  for i := 0 to TPList.Count - 1 do dispose(TPList[i]);
  TPList.Free;
  inherited Destroy;
end;

function TNomenk.TPCount: integer;
begin
  Result := TPList.Count;
end;

procedure TNomenk.AddTP(Value: Pointer);
begin
  TPList.Add(Value);
end;

function TNomenk.GetTP(Index: integer): TTP;
begin
  Result := TTP(TPList[Index]^);
end;

// Collection
function TNomenks.GetObject(Index: integer): TNomenk;
begin
  Result := (inherited Items[Index]) as TNomenk;
end;

procedure TNomenks.SetObject(Index: integer; Value: TNomenk);
begin
  inherited Items[Index] := Value;
end;

initialization
  ActiveFMSObject := nil;

end.

