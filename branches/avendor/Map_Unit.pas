unit Map_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, DrScrollBoxCanvas, ToolWin, ComCtrls, Menus, RXCtrls,
  DrSpeedButton, ClipBrd, Var_Unit;

type

  TMapForm = class(TForm)
    MapScrollBox: TDrScrollBoxCanvas;
    MapToolBar: TToolBar;
    MapZoomPlus: TDrSpeedButton;
    MapZoomMinus: TDrSpeedButton;
    MapZoomPage: TDrSpeedButton;
    ToolButton2: TToolButton;
    MapExitButton: TDrSpeedButton;
    MapSaveButton: TDrSpeedButton;
    ToolButton1: TToolButton;
    ToolButton3: TToolButton;
    TextButton: TDrSpeedButton;
    PathButton: TDrSpeedButton;
    StatusBar1: TStatusBar;
    procedure MapScrollBoxResize(Sender: TObject);
    procedure MapExitButtonClick(Sender: TObject);
    procedure MapZoomPlusClick(Sender: TObject);
    procedure MapZoomMinusClick(Sender: TObject);
    procedure MapZoomPageClick(Sender: TObject);
    procedure MapSaveButtonClick(Sender: TObject);
    procedure TextButtonClick(Sender: TObject);
    procedure PathButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    maxW, maxH: integer;
    OnlyTUPath: TOnlyTUPath;
    doBitBlt, doDrawCanvas: boolean;
  public
    Scale: double;
  end;

var
  MapForm: TMapForm;

implementation

uses FMSObjects_Unit, Const_Unit, Misc_Unit, ConstRes_Unit;

{$R *.DFM}

procedure TMapForm.FormCreate(Sender: TObject);
begin
  OnlyTUPath := tpNone;
  TextButton.Caption := Cap[ShowText];
  doBitBlt := true;
  doDrawCanvas := true;
  Scale := FMSConst.Scale;
end;

procedure TMapForm.MapScrollBoxResize(Sender: TObject);
const PointWidth = 0.08;
var
  i, j: integer;
  r: TRect;
  s: string;
  d1, d2: double;
  mDC: HDC;
  mBitmap: HBITMAP;
  mBrush : HBRUSH;
  mPen   : HPEN;


  function GetIntegerRect(X1, Y1, X2, Y2: double): TRect;
  begin
    Result.Left   := Round(X1*Scale);
    Result.Top    := Round(Y1*Scale);
    Result.Right  := Round((X1+X2)*Scale);
    Result.Bottom := Round((Y1+Y2)*Scale);
    if Result.Left   > maxW then maxW := Result.Left;
    if Result.Right  > maxW then maxW := Result.Right;
    if Result.Top    > maxH then maxH := Result.Top;
    if Result.Bottom > maxH then maxH := Result.Bottom;
    with MapScrollBox do
    begin
      dec(Result.Left,   HorzScrollBar.Position);
      dec(Result.Right,  HorzScrollBar.Position);
      dec(Result.Top,    VertScrollBar.Position);
      dec(Result.Bottom, VertScrollBar.Position);
    end;
  end;

  procedure DrawShowText(s: string; Rect: TRect; uFormat: UINT);
  begin
    if ShowText then
    begin
      SetTextColor(mDC, ColorRef(clWhite));
      DrawText(mDC, PChar(s), Length(s), Rect, uFormat);
    end;
  end;

begin
  if doDrawCanvas then
  begin
    doDrawCanvas := false;
    mBitmap := DWORD(nil);
    mBrush  := DWORD(nil);
    mPen    := DWORD(nil);
    try
      try
        try
          maxW := 0;
          maxH := 0;
          with MapScrollBox do
          begin
            mDC := dwCheckError(CreateCompatibleDC(Canvas.Handle), ErrorCreateCompatibleDC);
            mBitmap := dwCheckError(CreateCompatibleBitmap(Canvas.Handle, ClientWidth, ClientHeight), ErrorCreateCompatibleBitmap);
            r := Rect(0, 0, ClientWidth, ClientHeight);
            SelectObject(mDC, Font.Handle);
          end;
          SelectObject(mDC, mBitmap);
          SetBkMode(mDC, TRANSPARENT);
          SetBkColor(mDC, ColorRef(clTeal));
          // закраска DC
          mBrush := CreateSolidBrush(ColorRef(clTeal));
          SelectObject(mDC, mBrush);
          FillRect(mDC, r, mBrush);
          DeleteObject(mBrush);
          // Транспортный путь
          mPen := CreatePen(PS_DOT, 1, ColorRef(clBlack));
          SelectObject(mDC, mPen);
          for i := 0 to FMSPoint.Count-2 do
            for j := i+1 to FMSPoint.Count-1 do
              if FMSPoint.PointRel[i, j] > 0 then
              begin
                r := GetIntegerRect(FMSPoint.PointCoor[i, 0], FMSPoint.PointCoor[i, 1], 0, 0);
                MoveToEx(mDC, r.Left, r.Top, nil);
                r := GetIntegerRect(FMSPoint.PointCoor[j, 0], FMSPoint.PointCoor[j, 1], 0, 0);
                LineTo(mDC, r.Left, r.Top);
              end;
          DeleteObject(mPen);
          if OnlyTUPath = tpNone then
          begin
            // Вывести всю компоновку
            // ТУ
            mPen := CreatePen(PS_SOLID, 1, ColorRef(clYellow));
            SelectObject(mDC, mPen);
            mBrush := CreateSolidBrush($F00000);
            SelectObject(mDC, mBrush);
            for i := 0 to TUs.Count-1 do
              with TUs[i] do
              begin
                r := GetIntegerRect(tuX, tuY, tuDX, tuDY);
                if PictureExist then
                begin
                  DrawPictureInRect(r, mDC);
                  r := GetIntegerRect(tuX, tuY, 0.5, 0.5);
                end;
                Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
                s := IntToStr(ItemID);
                r := GetIntegerRect(tuX, tuY, tuDX, tuDY);
                DrawShowText(s, r, DT_SINGLELINE or DT_VCENTER or DT_CENTER);
              end;
            DeleteObject(mBrush);
            // ОТО
            mBrush := CreateSolidBrush($008000);
            SelectObject(mDC, mBrush);
            for i := 0 to OTOs.Count-1 do
              with OTOs[i] do
              begin
                // ОТО
                r := GetIntegerRect(otoX, otoY, otoDX, otoDY);
                if not PictureExist then
                begin
                  Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
                  r := GetIntegerRect(nakopX, nakopY, 2, 1);
                end else begin
                  DrawPictureInRect(r, mDC);
                  r := GetIntegerRect(nakopX, nakopY, 2, 0.3);
                  Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
                  r := GetIntegerRect(otoX, otoY, 0.5, 0.5);
                end;
                Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
                // Текст
                s := ' '+Name;
                r := GetIntegerRect(otoX, otoY, otoDX, otoDY);
                DrawShowText(s, r, DT_SINGLELINE or DT_VCENTER or DT_LEFT);
                dec(r.Right, 2);
                s := IntToStr(ItemID);
                r := GetIntegerRect(otoX, otoY, otoDX, 1);
                DrawShowText(s, r, DT_SINGLELINE or DT_VCENTER or DT_RIGHT);
              end;
            // Склад
            for i := 0 to Sklads.Count-1 do
              with Sklads[i] do
              begin
                r := GetIntegerRect(skladX, skladY, skladDX, skladDY);
                if not PictureExist then Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom)
                else begin
                  DrawPictureInRect(r, mDC);
                  if i <> 0 then begin
                    r := GetIntegerRect(skladX, skladY, 2, 0.3);
                    Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
                  end;
                end;
                case i of
                  0: s := 'вх';
                  1: s := 'вых';
                  else s := '';
                end;
                r := GetIntegerRect(skladX, skladY, skladDX, skladDY);
                DrawShowText(s, r, DT_SINGLELINE or DT_VCENTER or DT_CENTER);
              end;
          end else begin
            mPen := CreatePen(PS_SOLID, 1, ColorRef(clYellow));
            SelectObject(mDC, mPen);
            mBrush := CreateSolidBrush(ColorRef(clYellow));
            SelectObject(mDC, mBrush);
            // Вывести только вершины
            for i := 0 to FMSPoint.Count-1 do
            begin
              d1 := FMSPoint.PointCoor[i, 0];
              d2 := FMSPoint.PointCoor[i, 1];
              // Точка
              r := GetIntegerRect(d1-PointWidth, d2-PointWidth, PointWidth*2, PointWidth*2);
              Rectangle(mDC, r.Left, r.Top, r.Right, r.Bottom);
              // Номер вершины
              s := Format('%d ', [i+1]);
              r := GetIntegerRect(d1+PointWidth*2, d2+PointWidth*2, 0, 0);
              SetTextColor(mDC, ColorRef(clWhite));
              DrawText(mDC, PChar(s), Length(s), r, DT_CALCRECT or DT_SINGLELINE or DT_LEFT);
              DrawText(mDC, PChar(s), Length(s), r, DT_SINGLELINE or DT_LEFT);
              if OnlyTUPath = tpAll then
              begin
                // Координаты вершины
                s := Format('(%3.2f; %3.2f)', [d1, d2]);
                SetTextColor(mDC, ColorRef(clNavy));
                TextOut(mDC, r.Right, r.Top, PChar(s), Length(s));
                s := Format('%d (%3.2f; %3.2f)', [i+1, d1, d2]);
              end;
              // Установили размеры окна с учетом выведенного текста
              DrawText(mDC, PChar(s), Length(s), r, DT_CALCRECT);
              with MapScrollBox do
              begin
                if r.Right  + HorzScrollBar.Position > maxW then maxW := r.Right  + HorzScrollBar.Position;
                if r.Bottom + VertScrollBar.Position > maxH then maxH := r.Bottom + VertScrollBar.Position;
              end;
            end;
          end;
          MapScrollBox.HorzScrollBar.Range := maxW+10;
          MapScrollBox.VertScrollBar.Range := maxH+10;

          if doBitBlt then
            BitBlt(MapScrollBox.Canvas.Handle,
                   0, 0, MapScrollBox.ClientWidth, MapScrollBox.ClientHeight,
                   mDC, 0, 0, SRCCOPY);

          doDrawCanvas := true;
        finally
          DeleteDC(mDC);
          DeleteObject(mPen);
          DeleteObject(mBrush);
          DeleteObject(mBitmap);
        end;
      except
        doDrawCanvas := false;
        raise;
      end;
    except
      on E: FMSError do raise FMSError.CreateStopSubError(ErrorMapScrollBoxResize, E.Message);
      else raise FMSError.CreateStopRes(ErrorMapScrollBoxResize);
    end;
  end;
end;

procedure TMapForm.MapExitButtonClick(Sender: TObject);
begin
  Close;
end;

procedure TMapForm.MapZoomPlusClick(Sender: TObject);
begin
  if (MapScrollBox.HorzScrollBar.Range <= 64000) and (MapScrollBox.VertScrollBar.Range <= 64000) then
  begin
    Scale := Scale + Scale / 20;
    MapScrollBox.OnResize(Self);
  end;
end;

procedure TMapForm.MapZoomMinusClick(Sender: TObject);
begin
  if Scale >= 0.1 then
  begin
    Scale := Scale - Scale / 20;
    MapScrollBox.OnResize(Self);
  end;
end;

procedure TMapForm.MapZoomPageClick(Sender: TObject);
var
  i: integer;
  d: double;
begin
  with MapScrollBox do
    for i := 1 to 3 do
    begin
      if (HorzScrollBar.Range / ClientWidth) > (VertScrollBar.Range / ClientHeight)
        then d := HorzScrollBar.Range/(ClientWidth)
        else d := VertScrollBar.Range/(ClientHeight);
      Scale := Scale / d;
      doBitBlt := i = 3;
      OnResize(Self);
    end;
end;

procedure TMapForm.MapSaveButtonClick(Sender: TObject);
var
  tempImage  : TImage;
  tempFormat : Word;
  tempData   : THandle;
  tempPalette: HPALETTE;
begin
  tempImage := TImage.Create(MapScrollBox);
  try
    try
      with tempImage do
      begin
        Left   := MapScrollBox.ClientRect.Left;
        Top    := MapScrollBox.ClientRect.Top;
        Width  := MapScrollBox.ClientWidth;
        Height := MapScrollBox.ClientHeight;
        BitBlt(Canvas.Handle, Left, Top, Width, Height, MapScrollBox.Canvas.Handle, 0, 0, SRCCOPY);
        Picture.SaveToClipboardFormat(tempFormat, tempData, tempPalette);
      end;
      ClipBoard.SetAsHandle(tempFormat, tempData);
    finally
      tempImage.Free;
    end;
  except
    raise FMSError.CreateRes(ErrorClipboardCopy);
  end;
end;

procedure TMapForm.TextButtonClick(Sender: TObject);
begin
  ShowText := not ShowText;
  MapScrollBox.OnResize(nil);
  TextButton.Caption := Cap[ShowText];
end;

procedure TMapForm.PathButtonClick(Sender: TObject);
begin
  case OnlyTUPath of
    tpNone  : OnlyTUPath := tpAll;
    tpAll   : OnlyTUPath := tpNumber;
    tpNumber: OnlyTUPath := tpNone;
  end;
  TextButton.Enabled := OnlyTUPath = tpNone;
  MapScrollBox.OnResize(nil);
end;

procedure TMapForm.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_ESCAPE   then MapExitButtonClick(nil);
  if Key = VK_ADD      then MapZoomPlusClick(nil);
  if Key = VK_SUBTRACT then MapZoomMinusClick(nil);
  if (Key = Ord('C')) and (Shift = [ssCtrl]) then MapSaveButtonClick(nil);
  with MapScrollBox do
  begin
    if Key = VK_RIGHT then HorzScrollBar.Position := HorzScrollBar.Position + Round(Scale);
    if Key = VK_LEFT  then HorzScrollBar.Position := HorzScrollBar.Position - Round(Scale);
    if Key = VK_UP    then VertScrollBar.Position := VertScrollBar.Position - Round(Scale);
    if Key = VK_DOWN  then VertScrollBar.Position := VertScrollBar.Position + Round(Scale);
    if Key = VK_HOME  then begin
      HorzScrollBar.Position := 0;
      VertScrollBar.Position := 0;
    end;
  end;
end;

end.
