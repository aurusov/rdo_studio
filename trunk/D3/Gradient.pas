(********************************************************************** )

GRADIENT - модуль содержит три компоненты.

TGRADIENT - пр€моугольна€ область,
заполненна€ плавным переходом от одного цвета к другому. ƒл€ изменени€ "плавности"
перехода используйте свойство STEPS. “ип заполнени€ зависит от значени€ свойства
STYLE. COLOR и COLOR2 содержат цвета заливки.

TGRADSHAPE - јналог TSHAPE, но позвол€ющий заливку плавным переходом тонов.

TPICTURESHAPE - јналог TSHAPE, с картинкой внутри.  артинка может быть расположена
в углу изображени€, по центру, раст€нута на все поле или многократно повторена,
как мозаика. ƒл€ указани€ типа расположеи€ картинки используйте свойство
PICTURE STYLE.

GRADIENT (C) Alexander V. Demsky, Moscow, 1995. 2:5020/373.16 AKA 351.18 @FIDOnet

¬ы можете использовать (не распростран€ть!) этот файл совершенно свободно до
тех пор, пока не вносите в него каких-либо изменений. ¬се остальные способы
использовани€, распространени€ и публикации необходимо согласовывать с автором.

You may use (not to distribute!) this file absolutly free until you change it.
To distribute or publicate it or it's part contact with author.
Sorry for terrible english.

( **********************************************************************)
unit Gradient;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, ExtCtrls;

type
  TGradientStyle=(gsVertical,gsHorizontal,gsCircle,gsSquare,gsRectangle);
  TGradient = class(TPaintBox)
  private
    { Private declarations }
    FOnPaint:TNotifyEvent;
    FSteps:integer;
    FColor2:TColor;
    FStyle:TGradientStyle;
    procedure SetColor2(AVal:TColor);
    procedure SetSteps(AVal:integer);
    procedure SetStyle(AVal:TGradientStyle);
  protected
    { Protected declarations }
    procedure xDraw(Canvas:TCanvas;AMode:TPenMode);
    procedure Paint; override;
  public
    { Public declarations }
    constructor Create(AOwner : TComponent); override;
  published
    { Published declarations }
    property Color2:TColor read FColor2 write SetColor2;
    property Steps:Integer read FSteps write SetSteps;
    property Style:TGradientStyle read FStyle write SetStyle;
    property OnPaint:TNotifyEvent read FOnPaint write FOnPaint;
  end;

  TGradShape = class(TGradient)
  private
    { Private declarations }
    FOnPaint:TNotifyEvent;
    FCanvas:TBitMap;
    FShape:TShapeType;
    FPen:TPen;
    procedure SetPen(AVal:TPen);
    procedure SetShape(AVal:TShapeType);
    procedure PenCh(Sender : TObject);
  protected
    { Protected declarations }
    procedure Paint; override;
  public
    { Public declarations }
    constructor Create(AOwner : TComponent); override;
    destructor Destroy; override;
  published
    { Published declarations }
    property Shape:TShapeType read FShape write SetShape;
    property Pen:TPen read FPen write SetPen;
    property OnPaint:TNotifyEvent read FOnPaint write FOnPaint;
  end;

  TPictureLayout = (plNormal,plCentered,plStretched,plTiled);
  TPictureShape = class(TPaintBox)
  private
    { Private declarations }
    FOnPaint:TNotifyEvent;
    FPicture:TPicture;
    FShape:TShapeType;
    FPen:TPen;
    FPL : TPictureLayout;
    procedure SetPen(AVal:TPen);
    procedure SetShape(AVal:TShapeType);
    procedure SetPicture(AVal:TPicture);
    procedure SetPL(AVal:TPictureLayout);
    procedure PenCh(Sender : TObject);
  protected
    { Protected declarations }
    procedure Paint; override;
  public
    { Public declarations }
    constructor Create(AOwner : TComponent); override;
    destructor Destroy; override;
  published
    { Published declarations }
    property Picture:TPicture read FPicture write SetPicture;
    property PictureLayout:TPictureLayout read FPL write SetPL;
    property Shape:TShapeType read FShape write SetShape;
    property Pen:TPen read FPen write SetPen;
    property OnPaint:TNotifyEvent read FOnPaint write FOnPaint;
  end;

procedure Register;

implementation

constructor TGradient.Create(AOwner : TComponent);
begin
  inherited Create(AOwner);
  FSteps:=16;
  Color:=clWhite;
  FColor2:=clBlack;
  FStyle:=gsVertical;
end;

procedure TGradient.SetColor2(AVal:TColor);
begin
  if AVal <> FColor2 then begin
    FColor2 := AVal;
    Invalidate;
  end;
end;

procedure TGradient.SetStyle(AVal:TGradientStyle);
begin
  if AVal <> FStyle then begin
    FStyle := AVal;
    Invalidate;
  end;
end;

procedure TGradient.SetSteps(AVal:integer);
begin
  if AVal <> FSteps then begin
    if AVal<2 then AVal := 2;
    if AVal > 255 then AVal := 255;
    Fsteps := AVal;
    Invalidate;
  end;
end;

procedure TGradient.Paint;
begin
  xDraw(Canvas,pmCopy);
  if assigned(FOnPaint) then OnPaint(Self);
end;

procedure TGradient.xDraw(Canvas:TCanvas;AMode:TPenMode);
var
  i,xs,zz:integer;
  Red,Green,Blue:array[0..2] of byte;

function Max(x,y:integer):integer;
begin
  if x>y then Result:=x else Result:=y;
end;

begin
  Red[0]:=GetRValue(ColorToRgb(Color));
  Red[1]:=GetRValue(ColorToRgb(FColor2));
  Green[0]:=GetGValue(ColorToRgb(Color));
  Green[1]:=GetGValue(ColorToRgb(FColor2));
  Blue[0]:=GetBValue(ColorToRgb(Color));
  Blue[1]:=GetBValue(ColorToRgb(FColor2));
  Canvas.Pen.Mode:=AMode;
  Canvas.Pen.Width := 0;
  Canvas.Pen.Style:=psClear;
  zz := 0;
  case style of
    gsVertical:zz:=height;
    gsHorizontal:zz:=width;
    gsCircle:zz:=trunc(Sqrt(1.0*width*width+1.0*height*height)/2);
    gsSquare:zz:=max(width,height) div 2;
    gsRectangle:zz:=max(width,height) div 2;
  end;
  if (zz<steps) then xs:=zz else xs:=steps;
  if xs<2 then xs:=2;
  for i:=0 to xs-1 do begin
    Red[2]:=Trunc((Red[1]*1.0-Red[0]*1.0)*i/(xs-1)+red[0]*1.0);
    Green[2]:=Trunc((Green[1]*1.0-Green[0]*1.0)*i/(xs-1)+Green[0]*1.0);
    Blue[2]:=Trunc((Blue[1]*1.0-Blue[0]*1.0)*i/(xs-1)+Blue[0]*1.0);
    Canvas.Brush.Style := bsSolid;
    Canvas.Brush.Color := RGB(Red[2],Green[2],Blue[2]);
    case  Style of
      gsVertical:Canvas.Rectangle(0,trunc(1.0*Height*i/xs),
      Width,trunc(1.0*Height*(i+1)/xs)+2);
      gsHorizontal:Canvas.Rectangle(trunc(1.0*Width*i/xs),0,
      trunc(1.0*Width*(i+1)/xs)+2,Height);
      gsRectangle:Canvas.Rectangle(trunc(0.5*width*i/xs),trunc(0.5*height*i/xs),
      trunc((1-0.5*i/xs)*width),trunc((1-0.5*i/xs)*height));
      gsCircle:Canvas.Ellipse(trunc(width/2.0-(1.0-i/xs)*zz),
      trunc(height/2.0-(1.0-i/xs)*zz),
      trunc(width/2.0+(1.0-i/xs)*zz),trunc(height/2.0+(1.0-i/xs)*zz));
      gsSquare:Canvas.Rectangle(trunc(width/2.0-(1.0-i/xs)*zz),
      trunc(height/2.0-(1.0-i/xs)*zz),
      trunc(width/2.0+(1.0-i/xs)*zz),trunc(height/2.0+(1.0-i/xs)*zz));
    end;
  end;
  Canvas.Pen.Mode := pmCopy;
end;

constructor TGradShape.Create(AOwner : TComponent);
begin
  inherited Create(AOwner);
  FPen:=TPen.Create;
  FPen.OnChange:=PenCh;
  FShape:=stRectangle;
end;

destructor TGradShape.Destroy;
begin
  FPen.Free;
  Inherited destroy;
end;

procedure TGradShape.SetPen(AVal:TPen);
begin
    FPen.Assign(AVal);
end;

procedure TGradShape.PenCh(Sender : TObject);
begin
  Invalidate;
end;

procedure TGradShape.SetShape(AVal:TShapeType);
begin
  if AVal <> FShape then begin
    FShape := AVal;
    Invalidate;
  end;
end;

procedure TGradShape.Paint;
var
  zz,s:integer;
  var r:TRect;
function Min(x,y:integer):integer;
begin
  if x<y then Result:=x else Result:=y;
end;
begin
  r.top:=0;
  r.left:=0;
  r.Right:=width;
  r.bottom := height;
  s:=(pen.width+1) div 2;
  FCanvas:=TBitmap.Create;
  FCanvas.Canvas.Handle := CreateCompatibleDC(Canvas.Handle);
  FCanvas.Width := Width;
  FCanvas.Height := Height;
  with Canvas do
    begin
      FCanvas.Canvas.Brush.Style := bsSolid;
      xDraw (FCanvas.Canvas,pmCopy);
      CopyMode := cmSrcInvert;
      CopyRect(r,FCanvas.Canvas,r);
      CopyMode := cmSrcCopy;
      Brush.Color := clBlack;
      Pen.Style := psClear;
      zz:=min(Height,Width);
      case FShape of
          stRectangle:Rectangle(s,s,Width-s,Height-s);
          stRoundRect:RoundRect(s,s,Width-s,Height-s,Width div 8,Height div 8);
          stEllipse:Ellipse(s,s,Width-s,Height-s);
          stSquare:Rectangle(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
          stRoundSquare:RoundRect(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s,zz div 8,zz div 8);
          stCircle:Ellipse(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
      end;
      CopyMode := cmSrcInvert;
      CopyRect(r,FCanvas.Canvas,r);
      CopyMode := cmSrcCopy;
      Brush.Style := bsClear;
      Pen.Assign(Self.Pen);
      case FShape of
          stRectangle:Rectangle(s,s,Width-s,Height-s);
          stRoundRect:RoundRect(s,s,Width-s,Height-s,Width div 8,Height div 8);
          stEllipse:Ellipse(s,s,Width-s,Height-s);
          stSquare:Rectangle(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
          stRoundSquare:RoundRect(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s,zz div 8,zz div 8);
          stCircle:Ellipse(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
      end;
    end;
  DeleteDC(FCanvas.Handle);
  FCanvas.Free;
  if assigned(FOnPaint) then OnPaint(Self);
end;

constructor TPictureShape.Create(AOwner : TComponent);
begin
  inherited Create(AOwner);
  FPen:=TPen.Create;
  FPicture := TPicture.Create;
  FPen.OnChange:=PenCh;
  FShape:=stRectangle;
end;

destructor TPictureShape.Destroy;
begin
  FPicture.Free;
  FPen.Free;
  Inherited destroy;
end;

procedure TPictureShape.SetPen(AVal:TPen);
begin
    FPen.Assign(AVal);
end;

procedure TPictureShape.PenCh(Sender : TObject);
begin
  Invalidate;
end;

procedure TPictureShape.SetShape(AVal:TShapeType);
begin
  if AVal <> FShape then begin
    FShape := AVal;
    Invalidate;
  end;
end;

procedure TPictureShape.SetPL(AVal:TPictureLayout);
begin
  if AVal <> FPL then begin
    FPL := AVal;
    Invalidate;
  end;
end;

procedure TPictureShape.SetPicture (AVal:TPicture);
begin
  FPicture.Assign(AVal);
  Invalidate;
end;

procedure TPictureShape.Paint;
var
  zz,s:integer;
  r,g,p:TRect;
  i,j:integer;

function Min(x,y:integer):integer;
begin
  if x<y then Result:=x else Result:=y;
end;
begin
  r.top:=0;
  r.left:=0;
  r.Right:=width;
  r.bottom := height;
  s:=(pen.width+1) div 2;
  with Canvas do
    begin
      CopyMode := cmSrcInvert;
      if assigned(FPicture) then
      case FPL of
        plStretched:StretchDraw(r,FPicture.Graphic);
        plNormal:CopyRect(r,FPicture.Bitmap.Canvas,r);
        plCentered:begin
{          p.top:=0;p.left:=0;p.right:=min(FPicture.Bitmap.Width-1,Width-1);
          p.bottom:=min(FPicture.Bitmap.Height-1,Height);
          g:=p;
          OffsetRect(g,(Width-p.Width+1) div 2,
          (Height-p.Height+1) div 2);
          OffsetRect(g,(FPicture.Bitmap.Width-p.Width+1) div 2,
          (FPicture.Bitmap.Height-p.Height+1) div 2);
          CopyRect(p,FPicture.Bitmap.Canvas,g);}
          p.top:=0;p.left:=0;p.right:=FPicture.Bitmap.Width;
          p.bottom:=FPicture.Bitmap.Height;
          g:=p;
          OffsetRect(g,(Width-FPicture.Bitmap.Width) div 2,
          (Height-FPicture.Bitmap.Height) div 2);
          CopyRect(g,FPicture.Bitmap.Canvas,p);
        end;
        plTiled:begin
          p.top:=0;p.left:=0;p.right:=FPicture.Bitmap.Width;
          p.bottom:=FPicture.Bitmap.Height;
          for i:=0 to Width div FPicture.Bitmap.Width +1 do
            for j:=0 to Height div FPicture.Bitmap.Height +1 do
              begin
                g:=p;
                OffsetRect(g,FPicture.Bitmap.width*i,FPicture.Bitmap.Height*j);
                CopyRect(g,FPicture.Bitmap.Canvas,p);
              end;
        end;
      end;
      CopyMode := cmSrcCopy;
      Brush.Color := clBlack;
      Pen.Style := psClear;
      zz:=min(Height,Width);
      case FShape of
          stRectangle:Rectangle(s,s,Width-s,Height-s);
          stRoundRect:RoundRect(s,s,Width-s,Height-s,Width div 8,Height div 8);
          stEllipse:Ellipse(s,s,Width-s,Height-s);
          stSquare:Rectangle(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
          stRoundSquare:RoundRect(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s,zz div 8,zz div 8);
          stCircle:Ellipse(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
      end;
      CopyMode := cmSrcInvert;
      if assigned(FPicture) then
      case FPL of
        plStretched:StretchDraw(r,FPicture.Graphic);
        plNormal:CopyRect(r,FPicture.Bitmap.Canvas,r);
        plCentered:begin
          p.top:=0;p.left:=0;p.right:=FPicture.Bitmap.Width;
          p.bottom:=FPicture.Bitmap.Height;
          g:=p;
          OffsetRect(g,(Width-FPicture.Bitmap.Width) div 2,
          (Height-FPicture.Bitmap.Height) div 2);
          CopyRect(g,FPicture.Bitmap.Canvas,p);
        end;
        plTiled:begin
          p.top:=0;p.left:=0;p.right:=FPicture.Bitmap.Width;
          p.bottom:=FPicture.Bitmap.Height;
          for i:=0 to Width div FPicture.Bitmap.Width +1 do
            for j:=0 to Height div FPicture.Bitmap.Height +1 do
              begin
                g:=p;
                OffsetRect(g,FPicture.Bitmap.width*i,FPicture.Bitmap.Height*j);
                CopyRect(g,FPicture.Bitmap.Canvas,p);
              end;
        end;
      end;
      CopyMode := cmSrcCopy;
      Brush.Style := bsClear;
      Pen.Assign(Self.Pen);
      case FShape of
          stRectangle:Rectangle(s,s,Width-s,Height-s);
          stRoundRect:RoundRect(s,s,Width-s,Height-s,Width div 8,Height div 8);
          stEllipse:Ellipse(s,s,Width-s,Height-s);
          stSquare:Rectangle(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
          stRoundSquare:RoundRect(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s,zz div 8,zz div 8);
          stCircle:Ellipse(s+width div 2 - zz div 2,s+height div 2 - zz div 2,
          width div 2 + zz div 2-s,height div 2 + zz div 2-s);
      end;
    end;
  if assigned(FOnPaint) then OnPaint(Self);
end;

procedure Register;
begin
  RegisterComponents('Samples', [TGradient,TGradShape,TPictureShape]);
end;

end.
