unit TopBox_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, Grids;

type
  // Куда переместилась фишка
  ChipDirect = (cdRight, cdLeft, cdTop, cdBottom, cdNone, cdError);

  PTopBox = ^TTopBox;
  TTopBox = class(TMemo)
  private
    // При изменении размеров
    OldLeft, OldTop, OldWidth, OldHeight: integer;
    constructor Create(AOwner : TComponent); override;
    destructor  Destroy; override;
    procedure BoxMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    procedure BoxMouseDown(Sender: TObject; Button: TMouseButton;
                        Shift: TShiftState; X, Y: Integer);
    procedure BoxMouseUp(Sender: TObject; Button: TMouseButton;
                      Shift: TShiftState; X, Y: Integer);
    procedure BoxEnter(Sender: TObject);
  public
    Curr, Pred, Next : PTopBox;
    NumberTopBox, TopBoss, ChipMesto, DeepTop: integer;
    MoneyPath, MoneyEndPath, MoneyRule: extended;
    NumberChip  : ShortInt;
    Direct      : ChipDirect;
    MainTop     : boolean;
    procedure ReDrawText;
  end;

  function  FindTopBox(Number: integer): PTopBox;
  function  FindChild(Value: PTopBox): PTopBox;
  function  FindMainChild(Value: PTopBox): PTopBox;
  procedure NewTopBox;
  procedure DelAllTopBox;

implementation

uses Var_Unit, Game_Unit, Result_Top_Unit;

var
  dx, dy: Integer; // Запомнить коор. мыши для перетаскивания Box'а

procedure TTopBox.ReDrawText;
begin
  Lines.Clear;
  Lines.Add('');
  Lines.Add('');
  Lines[0] := IntToStr(NumberTopBox)+' ('+FloatToStr(MoneyPath)+'/'+
    FloatToStr(MoneyEndPath)+'/'+FloatToStr(MoneyRule)+')';
  Lines[1] := 'Фишка'+IntToStr(NumberChip)+' = '+IntToStr(ChipMesto);
  case Direct of
    cdRight : Lines[2] := 'вправо';
    cdLeft  : Lines[2] := 'влево';
    cdTop   : Lines[2] := 'вверх';
    cdBottom: Lines[2] := 'вниз';
    cdNone  : Lines[2] := 'начало поиска';
    cdError : Lines[2] := 'ошибка';
  end;
end;

function FindTopBox(Number: integer): PTopBox;
var TopTemp: PTopBox;
begin
  TopTemp := TopBoxFirst;
  while TopTemp <> nil do
    if TopTemp^.NumberTopBox <> Number
      then TopTemp := TopTemp^.Next
      else break;
  Result := TopTemp;
end;

function FindChild(Value: PTopBox): PTopBox;
var TopTemp: PTopBox;
begin
  TopTemp := TopBoxFirst;
  while TopTemp <> nil do
    if TopTemp^.TopBoss = Value^.NumberTopBox then break
                                              else TopTemp := TopTemp^.Next;
  Result := TopTemp;
end;

function FindMainChild(Value: PTopBox): PTopBox;
var TopTemp: PTopBox;
begin
  TopTemp := TopBoxFirst;
  while TopTemp <> nil do
    if (TopTemp^.MainTop) and
       (TopTemp^.TopBoss = Value^.NumberTopBox) then break
                                                else TopTemp := TopTemp^.Next;
  Result := TopTemp;
end;

constructor TTopBox.Create(AOwner : TComponent);
begin
  inherited Create(AOwner);
  if TopBoxFirst = nil Then TopBoxFirst := TopBox;
  Curr := TopBox;
  Pred := TopBoxLast;
  Next := nil;
  TopBoxLast := TopBox;
  if Pred <> nil then Pred^.Next := TopBox;
// Инициализация Memo
  Visible:= false;
  Parent := GraphForm.TopScrollBox;
  try
    with Font do
    begin
      Charset := RUSSIAN_CHARSET;
      Color   := clBlack;
      Height  := -11;
      Name    := 'MS Sans Serif';
      Pitch   := fpDefault;
      Size    := 8;
      Style   := [];
    end;
  except
    ParentFont := true;
    Font.Color := clBlack;
  end;
  NumberTopBox:= -1;
  TopBoss     := -1;
  DeepTop     := -1;
  NumberChip  := -1;
  ChipMesto   := -1;
  Direct      := cdError;
  MainTop     := false;
  MoneyPath   := 0;
  MoneyEndPath:= 0;
  MoneyRule   := 0;
  Alignment   := taCenter;
  Ctl3D       := False;
  Color       := clWhite;
  OnEnter     := BoxEnter;
  OnMouseDown := BoxMouseDown;
  OnMouseMove := BoxMouseMove;
  OnMouseUp   := BoxMouseUp;
  Width  := 100;
  Height := 45;
  Visible := true;
end;

destructor TTopBox.Destroy;
var
   TopBoxPred: PTopBox;
begin
  TopBox := Curr;
  if TopBoxFirst = TopBoxLast then
  begin
    // Этот блок лишний, но на всякий случай
    TopBox      := nil;
    TopBoxFirst := nil;
    TopBoxLast  := nil;
  end else begin
    if TopBox = TopBoxFirst then TopBoxFirst := TopBox^.Next;
    if TopBox = TopBoxLast  then TopBoxLast  := TopBox^.Pred;
    TopBoxPred := TopBox^.Pred;
    if TopBoxPred <> nil then TopBoxPred^.Next := TopBox^.Next;
    if TopBox^.Next <> nil then TopBox^.Next^.Pred := TopBoxPred;
  end;
  inherited destroy;
  try
    GraphForm.FActiveTopBox := nil;
    GraphForm.ActiveTopBox  := nil;
  except
  end;
end;

procedure NewTopBox;
begin
  new(TopBox);
  TopBox^ := TTopBox.Create(GraphForm.TopScrollBox);
  GraphForm.ActiveTopBox := TopBox;
end;

procedure DelAllTopBox;
var TopTemp, TopTemp1: PTopBox;
begin
  try
    GraphForm.TopScrollBox.OnResize := nil;
  except
  end;
  TopTemp := TopBoxFirst;
  while TopTemp <> nil do
  begin
    TopTemp1 := TopTemp^.Next;
    TopTemp.Free;
    TopTemp := TopTemp1;
  end;
  try
    GraphForm.TopScrollBox.OnResize := GraphForm.TopScrollBoxResize;
  except
  end;
end;

procedure TTopBox.BoxEnter(Sender: TObject);
begin
  GraphForm.TopScrollBox.SetFocus;
end;

procedure TTopBox.BoxMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  Box: PTopBox;
begin
  GraphForm.TopScrollBox.SetFocus;
  GraphForm.ActiveTopBox := Curr;
  if (ssDouble in Shift) and (Button = mbLeft) then
  begin
    ResultTopForm.OnShow(Self);
    ResultTopForm.Visible := true;
    exit;
  end;
  MouseCapture := true;
  GraphForm.TopScrollBox.HorzScrollBar.Position := ScrollX;
  GraphForm.TopScrollBox.VertScrollBar.Position := ScrollY;
  BringToFront;
  dx := -X;
  dy := -Y;
  MyTopBoxChange := true;
  Box := TopBoxFirst;
  while Box <> nil do
  begin
    Box^.Enabled := false;
    Box := Box^.Next;
  end;
  Enabled := true;
  OldLeft   := Left;
  OldTop    := Top;
  OldWidth  := Width;
  OldHeight := Height;
end;

procedure TTopBox.BoxMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var Box: PTopBox;
begin
  TopBoxMode := Normal;
  MyTopBoxChange := false;
  Cursor := crArrow;
  MouseCapture := false;
  Box := TopBoxFirst;
  while Box <> nil do
  begin
    Box^.Enabled := true;
    Box := Box^.Next;
  end;
  SendToBack;
  GraphForm.TopScrollBoxResize(Self);
end;

procedure TTopBox.BoxMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  LeftTemp, TopTemp, WidthTemp, HeightTemp: integer;
  delta, PanelWidth, PanelHeight: integer;
begin
  ScrollX := GraphForm.TopScrollBox.HorzScrollBar.Position;
  ScrollY := GraphForm.TopScrollBox.VertScrollBar.Position;
  // Box
  if not(MyTopBoxChange) then
  begin
    MouseCapture := false;
    if Ctl3D then delta := 7 else delta := 5;
    if (x > delta) and (x < Width-delta) and
       (y > delta) and (y < Height-delta) then
    begin
      Cursor  := crArrow;
      TopBoxMode := MyBoxDrag;
    end;
    if (x > delta) and (x < Width-delta) and
       (y >=0) and (y <= delta) then
    begin
      Cursor  := crSizeNS;
      TopBoxMode := SizeUp;
    end;
    if (x > delta) and (x < Width-delta) and
       (y >= Height-delta) and (y <= Height) then
    begin
      Cursor  := crSizeNS;
      TopBoxMode := SizeDown;
    end;
    if (x >=0 ) and (x <= delta) and
       (y > delta ) and (y < Height-delta) then
    begin
      Cursor  := crSizeWE;
      TopBoxMode := SizeLeft;
    end;
    if (x >= Width-delta) and (x <= Width) and
       (y > delta) and (y < Height-delta) then
    begin
      Cursor  := crSizeWE;
      TopBoxMode := SizeRight;
    end;
    if (x >= 0) and (x <= delta) and
       (y >= 0) and (y <= delta) then
    begin
      Cursor  := crSizeNWSE;
      TopBoxMode := SizeUpLeft;
    end;
    if (x >= Width-delta) and (x <= Width) and
       (y >= 0) and (y <= delta) then
    begin
      Cursor  := crSizeNESW;
      TopBoxMode := SizeUpRight;
    end;
    if (x >= 0) and (x <= delta) and
       (y >= Height-delta) and (y <= Height) then
    begin
      Cursor  := crSizeNESW;
      TopBoxMode := SizeDownLeft;
    end;
    if (x >= Width-delta) and (x <= Width) and
       (y >= Height-delta) and (y <= Height) then
    begin
      Cursor  := crSizeNWSE;
      TopBoxMode := SizeDownRight;
    end
  end  //then end
  else
  begin
    if GraphForm.ActiveTopBox = nil then
    begin
      BoxMouseUp(Sender, mbRight, Shift, X, Y);
      exit;
    end;
    LeftTemp  := Left;
    WidthTemp := Width;
    TopTemp   := Top;
    HeightTemp:= Height;
    PanelWidth := GraphForm.TopScrollBox.Width;
    PanelHeight:= GraphForm.TopScrollBox.Height;
    Case TopBoxMode of
    MyBoxDrag:
      begin
        LeftTemp := LeftTemp+x+dx;
        TopTemp  := TopTemp+y+dy;
      end;
    SizeUp:
      begin
        TopTemp := TopTemp+y;
        HeightTemp := HeightTemp-y;
      end;
    SizeDown:
      begin
        HeightTemp := y;
      end;
    SizeLeft:
      begin
        LeftTemp  := Left+x;
        WidthTemp := Width-x;
      end;
    SizeRight:
      begin
        WidthTemp := x;
      end;
    SizeUpLeft:
      begin
        LeftTemp   := Left+x;
        WidthTemp  := Width-x;
        TopTemp    := Top+y;
        HeightTemp := Height-y;
      end;
    SizeUpRight:
      begin
        WidthTemp  := x;
        TopTemp    := Top+y;
        HeightTemp := Height-y;
      end;
    SizeDownLeft:
      begin
        LeftTemp   := Left+x;
        WidthTemp  := Width-x;
        HeightTemp := y;
      end;
    SizeDownRight:
      begin
        WidthTemp  := x;
        HeightTemp := y;
      end;
    end; // case end

    if WidthTemp <= 20 then
    begin
      LeftTemp := OldLeft;
      WidthTemp:= OldWidth;
    end;
    if HeightTemp <= 20 then
    begin
      TopTemp   := OldTop;
      HeightTemp:= OldHeight;
    end;

    if TopBoxMode = MyBoxDrag then
    begin
      if LeftTemp < 0 then
      begin
        LeftTemp := 0;
        WidthTemp:= OldWidth;
      end;
      if TopTemp < 0 then
      begin
        TopTemp   := 0;
        HeightTemp:= OldHeight;
      end;
      if (WidthTemp+LeftTemp) > PanelWidth then
      begin
        LeftTemp  := PanelWidth-OldWidth;
        WidthTemp := OldWidth;
      end;
      if (HeightTemp+TopTemp) > PanelHeight then
      begin
        TopTemp   := PanelHeight-OldHeight;
        HeightTemp:= OldHeight;
      end;
    end
    else
    begin
      if LeftTemp < 0 then
      begin
        LeftTemp := 0;
        WidthTemp:= OldWidth+OldLeft;
      end;
      if TopTemp < 0 then
      begin
        TopTemp   := 0;
        HeightTemp:= OldHeight+OldTop;
      end;
      if (WidthTemp+LeftTemp) > PanelWidth then
      begin
        LeftTemp  := OldLeft;
        WidthTemp := PanelWidth-OldLeft;
      end;
      if (HeightTemp+TopTemp) > PanelHeight then
      begin
        TopTemp   := OldTop;
        HeightTemp:= PanelHeight-OldTop;
      end;
    end;
    Left      := LeftTemp;
    Width     := WidthTemp;
    Top       := TopTemp;
    Height    := HeightTemp;
    OldLeft   := Left;
    OldTop    := Top;
    OldWidth  := Width;
    OldHeight := Height;
    MouseCapture := true;
  end; // else end
end;

end.
