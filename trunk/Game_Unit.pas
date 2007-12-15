unit Game_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, TopBox_Unit, Var_Unit, Buttons, ComCtrls, ToolWin, Menus,
  DrScrollBoxCanvas, RXCtrls, DrSpeedButton;

type

  TGraphForm = class(TForm)
    TopScrollBox: TDrScrollBoxCanvas;
    ToolBar: TToolBar;
    ZoomMin: TDrSpeedButton;
    ZoomMax: TDrSpeedButton;
    ToolButton2: TToolButton;
    ZoomOne: TDrSpeedButton;
    ResultTop: TDrSpeedButton;
    Result: TDrSpeedButton;
    GraphMenu: TMainMenu;
    Graph1: TMenuItem;
    Exit1: TMenuItem;
    Zoom1: TMenuItem;
    ZoomIn1: TMenuItem;
    ZoomOut1: TMenuItem;
    Showall1: TMenuItem;
    Topinfo1: TMenuItem;
    Graphinfo1: TMenuItem;
    ToolButton1: TToolButton;
    N1: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure TopScrollBoxResize(Sender: TObject);
    procedure ZoomMinClick(Sender: TObject);
    procedure ZoomOneClick(Sender: TObject);
    procedure ResultTopClick(Sender: TObject);
    procedure ResultClick(Sender: TObject);
    procedure TopScrollBoxClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormDestroy(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure ZoomIn1Click(Sender: TObject);
    procedure ZoomOut1Click(Sender: TObject);
    procedure ResultEnabled(Sender: TObject);
    procedure ResultTopEnabled(Sender: TObject);
    procedure ZoomMinEnabled(Sender: TObject);
    procedure ZoomMaxEnabled(Sender: TObject);
    procedure ZoomOneEnabled(Sender: TObject);
  private
    flagResize: boolean;
    VertSB, HorzSB: integer;
    TopList: TList;
    procedure WMGetMinMaxInfo(var M: TWMGetMinMaxInfo); message WM_GETMINMAXINFO;
    procedure SetActiveTopBox(Value: PTopBox);
  public
    TopCount, ListWidth, ListTop: integer;
    FActiveTopBox: PTopBox;
    procedure ConvertTraceFile;
    property ActiveTopBox: PTopBox read FActiveTopBox write SetActiveTopBox;
  end;

  function PickOut(pkstr: string; pkint: integer): integer;
  function PickOutFloat(pkstr: string; pkint: integer): extended;
  function PickOutStr(pkstr: string; pkint: integer): string;

var
  GraphForm: TGraphForm;

implementation

uses Result_Unit, Result_Top_Unit, RDO_Edit_Unit;

{$R *.DFM}

procedure TGraphForm.SetActiveTopBox(Value: PTopBox);
begin
  if FActiveTopBox <> nil then
  begin
    with FActiveTopBox^ do
      if MainTop then begin
          Color      := clGray;
          Font.Color := clWhite;
        end else begin
          Color      := clWhite;
          Font.Color := clBlack;
      end;
  end;
  FActiveTopBox := Value;
  if Value <> nil then
  begin
    Value^.Color      := clGreen;
    Value^.Font.Color := clWhite;
    if Result.Enabled then
    begin
      ResultTop.Enabled := true;
      if ResultTopForm.Visible then ResultTopForm.OnShow(Self);
    end;
  end else
  begin
    ResultTop.Enabled     := false;
    ResultTopForm.Visible := false;
  end;
end;

procedure TGraphForm.WMGetMinMaxInfo(var M: TWMGetMinMaxInfo);
begin
  inherited;
  M.MinMaxInfo^.ptMinTrackSize.X := 400;
  M.MinMaxInfo^.ptMinTrackSize.Y := 350;
end;

function PickOut(pkstr: string; pkint: integer): integer;
begin
  Result := Round( PickOutFloat(pkstr, pkint) );
end;

function PickOutFloat(pkstr: string; pkint: integer): extended;
var
  j, count: integer;
  str: string;
begin
  try
    DecimalSeparator := '.';
    pkstr:= Trim(pkstr);
    str  := '';
    count:= 0;
    j    := 0;
    while j <= Length(pkstr) do
    begin
      inc(j);
      str := str + pkstr[j];
      if (pkstr[j] = ' ') or (pkstr[j] = ':') or (pkstr[j] = #0) then
      begin
        inc(count);
        if count = pkint then break;
        Delete(pkstr, 1, j);
        pkstr := TrimLeft(pkstr);
        while pkstr[1] = ':' do
        begin
          Delete(pkstr, 1, 1);
          pkstr := TrimLeft(pkstr);
        end;
        j     := 0;
        str   := '';
      end;
    end;
    if count = pkint then Result := StrToFloat(Trim(str))
                     else raise Exc.Create('');
  except
    raise Exc.Create('Ошибка выборки числа из файла трассировки.');
    Result := -2;
  end;
end;

function PickOutStr(pkstr: string; pkint: integer): string;
var
  j, count: integer;
  str: string;
begin
  try
    pkstr:= Trim(pkstr);
    str  := '';
    count:= 0;
    j    := 0;
    while j <= Length(pkstr) do
    begin
      inc(j);
      str := str + pkstr[j];
      if (pkstr[j] = ' ') or (pkstr[j] = ':') then
      begin
        inc(count);
        if count = pkint then break;
        Delete(pkstr, 1, j);
        pkstr := TrimLeft(pkstr);
        while pkstr[1] = ':' do
        begin
          Delete(pkstr, 1, 1);
          pkstr := TrimLeft(pkstr);
        end;
        j     := 0;
        str   := '';
      end;
    end;
    Result := Trim(str);
  except
    raise Exc.Create('Ошибка выборки строки из файла трассировки.');
    Result := 'nil';
  end;
end;

procedure TGraphForm.FormCreate(Sender: TObject);
begin
  FActiveTopBox     := nil;
  flagResize        := false;
  ZoomMin.Enabled   := false;
  ZoomMax.Enabled   := false;
  ZoomOne.Enabled   := false;
  Result.Enabled    := false;
  ResultTop.Enabled := false;
  with TopScrollBox.Canvas.Pen do
  begin
    Color := clBlack;
    Mode  := pmCopy;
    Style := psSolid;
    Width := 1;
  end;
  TopList := TList.Create;
end;

procedure TGraphForm.TopScrollBoxResize(Sender: TObject);
var
  TopTemp: PTopBox;
  l, t: integer;
begin
  if not flagResize then exit;
  with TopScrollBox.Canvas do
  begin
    Brush.Color := Color;
    FillRect(ClientRect);
  end;
  with TopScrollBox.Canvas do
  begin
    TopTemp := TopBoxFirst;
    while TopTemp <> nil do
    begin
      with TopTemp^ do
      begin
        if MainTop then TopScrollBox.Canvas.Pen.Color := clRed
                   else TopScrollBox.Canvas.Pen.Color := clBlack;
        if TopBoss <> 0 then
        begin
          l := Left + Width div 2;
          MoveTo(l-2, Top  );
          LineTo(l-2, Top-2);
          LineTo(l+2, Top-2);
          LineTo(l+2, Top  );
          MoveTo(l  , Top-2);
          if FindTopBox(TopBoss)^ <> nil then
            with FindTopBox(TopBoss)^ do
            begin
              l := Left+Width div 2;
              t := Top+Height-1;
              LineTo(l  , t+2);
              MoveTo(l-1, t  );
              LineTo(l-1, t+2);
              LineTo(l  , t+2);
              LineTo(l  , t  );
              LineTo(l+1, t  );
              LineTo(l+1, t+3);
            end;
        end;
      end;
      TopTemp := TopTemp^.Next;
    end;
  end;
  // Кнопки изменения масштаба
  with TopScrollBox do
    if (ClientWidth = Width-4) and (ClientHeight = Height-4)
      then ZoomMin.Enabled := false
      else ZoomMin.Enabled := true;
  if TopBoxFirst <> nil then
    if TopBoxFirst^.Width >= 100
      then ZoomMax.Enabled := false
      else ZoomMax.Enabled := true;
end;

procedure TGraphForm.ZoomMinClick(Sender: TObject);
var
  TopTemp: PTopBox;
begin
  VertSB := TopScrollBox.VertScrollBar.Position;
  HorzSB := TopScrollBox.HorzScrollBar.Position;
  if Sender is TDrSpeedButton then
    with Sender as TDrSpeedButton do
    begin
      if Name = 'ZoomMin' then TopScrollBox.ScaleBy(100, 140);
      if Name = 'ZoomMax' then TopScrollBox.ScaleBy(140, 100);
    end;
  TopScrollBox.VertScrollBar.Position := VertSB;
  TopScrollBox.HorzScrollBar.Position := HorzSB;
  TopTemp := TopBoxFirst;
  while TopTemp <> nil do
  begin
    TopTemp^.Font.Size := 8;
    TopTemp := TopTemp^.Next;
  end;
end;

procedure TGraphForm.ConvertTraceFile;
var
  i, j, j1, GraphCount, GraphPos, maxDeep, NumTopDeep, maxNumTopDeep: integer;
  NumTop, NumTopBoss, PustoMesto, p, TopLeftNum, k, m: integer;
  str: string;
  TopTemp: PTopBox;
begin
  RDOEdit_Game5.ShowGraphButton.Enabled := false;
  try
    TopScrollBox.OnResize := nil;
    try
      flagResize := false;
      TopScrollBox.Visible := false;
      FActiveTopBox     := nil;
      ActiveTopBox      := nil;
      ZoomMin.Enabled   := false;
      ZoomMax.Enabled   := false;
      ZoomOne.Enabled   := false;
      Result.Enabled    := false;
      ResultTop.Enabled := false;
      ResultForm.WaitPanel.Visible := true;
      ResultForm.Show;
      Application.ProcessMessages;
      DelAllTopBox;
      maxDeep  := 0;
      // Определяем кол-во построенных графоф
      GraphCount := 0;
      GraphPos   := 0;
      for i := 0 to RDOEdit_Game5.TRCRich.ParagraphCount-1 do
      begin
        str := RDOEdit_Game5.TRCRich.Paragraphs[i];
        if Pos('SB ', str) > 0 then
        begin
          inc(GraphCount);
          GraphPos := i;
        end;
        p := Round(i/(RDOEdit_Game5.TRCRich.ParagraphCount-1)*10);
        ResultForm.WaitPanel.Caption := 'Идет обработка... ' + IntToStr(p)+'%';
        Application.ProcessMessages;
//      if Pos('STR', str) > 0 then ShowMessage('Find STR');
      end;
      if GraphCount = 0 then
        raise Exc.Create('В файле трассировки не найдена информация о поиске на графе.');
      // Строим вершины графа
      TopCount := 0;
      i := GraphPos-1;
      repeat
        inc(i);
        str := RDOEdit_Game5.TRCRich.Paragraphs[i];
        if Pos('SO', str) > 0 then
        begin
          while Pos('ST', RDOEdit_Game5.TRCRich.Paragraphs[i+1]) > 0 do
          begin
            if i >= RDOEdit_Game5.TRCRich.ParagraphCount then
              raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
            p := Round((i-GraphPos)/(RDOEdit_Game5.TRCRich.ParagraphCount-30)*89*GraphCount)+10;
            ResultForm.WaitPanel.Caption := 'Идет обработка... ' + IntToStr(p)+'%';
            Application.ProcessMessages;
            NumTop := PickOut(str, 2); // Поиск значений в SO
            // Первая вершина создается просто так
            if NumTop = 1 then
            begin
              NewTopBox;
              inc(TopCount);
              with TopBox^ do
              begin
                NumberTopBox := NumTop;
                TopBoss      := 0;
                DeepTop      := 1;
                NumberChip   := 0;
                ChipMesto    := 0;
                Direct       := cdNone;
                MoneyPath    := 0;
                MoneyEndPath := 0;
                MoneyRule    := 0;
                ReDrawText;
              end;
            end;
            // Остальные вершины
            inc(i);                               // Перешли на ST N|D|R
            str := RDOEdit_Game5.TRCRich.Paragraphs[i];
            if (Pos('STN', str) > 0) or (Pos('STR', str) > 0) then
            begin                                 // Обработка STN и STR
              NumTop     := PickOut(str, 2);
              NumTopBoss := PickOut(str, 3);
              NewTopBox;
              with TopBox^ do
              begin
                MoneyPath   := PickOutFloat(str, 4);
                MoneyEndPath:= PickOutFloat(str, 5) - MoneyPath;
                MoneyRule   := PickOutFloat(str, 8);
              end;
              inc(TopCount);
              inc(i);                             // Перешли на фишку
              str := RDOEdit_Game5.TRCRich.Paragraphs[i];
              if (Pos('SRK', str) = 0) or
                 (Pos('SRK', RDOEdit_Game5.TRCRich.Paragraphs[i+1]) = 0) then
                raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
              with TopBox^ do
              begin
                NumberTopBox := NumTop;
                TopBoss      := NumTopBoss;
                DeepTop      := FindTopBox(TopBoss)^.DeepTop+1;
                if maxDeep < DeepTop then maxDeep := DeepTop;
                NumberChip   := PickOut(str, 5);
                ChipMesto    := ShortInt(PickOut(str, 6));
              end;
              inc(i);                             // Перешли на дырку
              str := RDOEdit_Game5.TRCRich.Paragraphs[i];
              if Pos('SRK', str) = 0 then
                raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
              PustoMesto := PickOut(str, 5);
              dec(PustoMesto, TopBox^.ChipMesto);
              case PustoMesto of
                -3: TopBox^.Direct := cdBottom;
                -1: TopBox^.Direct := cdRight;
                 1: TopBox^.Direct := cdLeft;
                 3: TopBox^.Direct := cdTop;
               else TopBox^.Direct := cdError;
              end;
              TopBox^.ReDrawText;
            end else
            begin                                 // Обработка STD
              inc(i, 2);
            end;
          end;
        end;
        if i >= RDOEdit_Game5.TRCRich.ParagraphCount then
          raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
      until (Pos('SD', str) > 0) or (Pos('SE', RDOEdit_Game5.TRCRich.Paragraphs[i+1]) > 0);
      // Определение решения на графе
      if Pos('SD', str) > 0 then
        repeat
          inc(i);
          str := RDOEdit_Game5.TRCRich.Paragraphs[i];
          with FindTopBox(PickOut(str, 1))^ do
          begin
            Color      := clGray;
            Font.Color := clWhite;
            MainTop    := true;
          end;
          if i >= RDOEdit_Game5.TRCRich.ParagraphCount then
            raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
        until Pos('SE', RDOEdit_Game5.TRCRich.Paragraphs[i+1]) > 0;
      // Результаты поиска
      if Pos('SE', str) = 0 then inc(i);
      str := RDOEdit_Game5.TRCRich.Paragraphs[i];
      with ResultForm do
      begin
        SostT.Caption := '';
        if Pos('SES', str) > 0 then SostT.Caption := 'Решение найдено';
        if Pos('SEN', str) > 0 then SostT.Caption := 'Решение НЕ найдено';
        if Pos('SEM', str) > 0 then SostT.Caption := 'Не хватило памяти для поиска';
        if Pos('SEF', str) > 0 then SostT.Caption := 'Ошибка записи в файл';
        if Pos('SEU', str) > 0 then SostT.Caption := 'Неизвестная ошибка';
        if SostT.Caption = '' then
          raise Exc.Create('Файл трассировки испорчен. Граф не создан.');
        ModTimeT.Caption := PickOutStr(str, 2);
        TimeT.Caption    := PickOutStr(str, 3);
        MemoryT.Caption  := PickOutStr(str, 4);
        MoneyT.Caption   := PickOutStr(str, 5);
        OpenTopT.Caption := PickOutStr(str, 6);
        TopT.Caption     := PickOutStr(str, 7);
        TopInT.Caption   := PickOutStr(str, 8);
        TopChildT.Caption:= PickOutStr(str, 9);
      end;
    except
      on E: Exc do begin
        Close;
        raise;
      end;
      on E: Exception do begin
        Close;
        RDOEdit_Game5.ShowError('Возникла неизвестная ошибка трансляции файла трассировки: '+
          MyEOL+''''+E.Message+'''.'+MyEOL+'Обратитесь к разработчику.');
        exit;
      end;
    else
      Close;
      RDOEdit_Game5.ShowError('Возникла неизвестная ошибка трансляции файла трассировки.'+
        MyEOL+'Обратитесь к разработчику.');
      exit;
    end;
    try
      // Отрисовка графа
      // Находим max кол-во вершин на глубине
      maxNumTopDeep := 0;
      for i := 1 to maxDeep do
      begin
        NumTopDeep := 0;
        TopTemp    := TopBoxFirst;
        while TopTemp <> nil do
        begin
          if TopTemp^.DeepTop = i then inc(NumTopDeep);
          TopTemp := TopTemp^.Next;
        end;
        if maxNumTopDeep < NumTopDeep then maxNumTopDeep := NumTopDeep;
      end;
      // Определили ширину листа
      ListWidth := 110*maxNumTopDeep;
      ListTop   := 10;
      // Расстановка вершин на листе
      for i := 1 to maxDeep do
      begin
        if ((i/maxDeep*100) > 40) and ((i/maxDeep*100) <= 70) then
        begin
          ResultForm.WaitPanel.Caption := 'Идет обработка... 99%';
          Application.ProcessMessages;
        end;
        if (i/maxDeep*100) > 70 then
        begin
          ResultForm.WaitPanel.Caption := 'Идет обработка... 100%';
          Application.ProcessMessages;
        end;
        NumTopDeep := 0;
        TopTemp    := TopBoxFirst;
        while TopTemp <> nil do
        begin
          if TopTemp^.DeepTop = i then inc(NumTopDeep);
          TopTemp := TopTemp^.Next;
        end;
        j  := ListWidth div NumTopDeep;
        j1 := j div 2;
        if NumTopDeep < 3 then
        begin
          TopTemp := TopBoxFirst;
          while TopTemp <> nil do
          begin
            if TopTemp^.DeepTop = i then
            begin
              TopTemp^.Left := j1 - 50;
              TopTemp^.Top  := ListTop;
              inc(j1, j);
            end;
            TopTemp := TopTemp^.Next;
          end;
        end else
        begin
          TopList.Clear;
          TopTemp := TopBoxFirst;
          while TopTemp <> nil do
          begin
            if TopTemp^.DeepTop = i then
            begin
              TopTemp^.Left := FindTopBox(TopTemp^.TopBoss)^.Left;
              TopTemp^.Top  := ListTop;
              TopList.Add(TopTemp);
            end;
            TopTemp := TopTemp^.Next;
          end;
          for k := 0 to TopList.Count-2 do
            for m := k+1 to TopList.Count-1 do
              if TTopBox(TopList.Items[k]^).Left > TTopBox(TopList.Items[m]^).Left then
                TopList.Exchange(k, m);
          for k := 0 to TopList.Count-1 do
          begin
            TTopBox(TopList.Items[k]^).Left := j1-50;
            inc(j1, j);
          end;
        end;
        inc(ListTop, 100);
      end;
      ActiveTopBox := nil;
      ResultForm.WaitPanel.Visible := false;
      ResultForm.Close;
      ResultForm.ShowModal;
      flagResize := true;
      TopScrollBox.Visible := true;
      TopScrollBox.OnReSize(Self);
      ZoomMin.Enabled   := true;
      ZoomMax.Enabled   := true;
      ZoomOne.Enabled   := true;
      Result.Enabled    := true;
      ActiveTopBox      := TopBoxFirst;
    except
      on E: Exc do begin
        Close;
        raise;
      end;
      on E: Exception do begin
        Close;
        RDOEdit_Game5.ShowError('Возникла неизвестная ошибка отображения графа: '+
          MyEOL+''''+E.Message+'''.'+MyEOL+'Обратитесь к разработчику.');
        exit;
      end;
    else
      Close;
      RDOEdit_Game5.ShowError('Возникла неизвестная ошибка отображения графа.'+
        MyEOL+'Обратитесь к разработчику.');
      exit;
    end;
  finally
    TopScrollBox.OnResize := TopScrollBoxResize;
  end;
  RDOEdit_Game5.ShowGraphButton.Enabled := true;
end;

procedure TGraphForm.ZoomOneClick(Sender: TObject);
begin
  with TopScrollBox do
  begin
    if (HorzScrollBar.Range div ClientWidth) > (VertScrollBar.Range div ClientHeight)
      then ScaleBy(100, ((HorzScrollBar.Range div ClientWidth)+1)*100)
      else ScaleBy(100, ((VertScrollBar.Range div ClientHeight)+1)*100);
  end;
end;

procedure TGraphForm.ResultTopClick(Sender: TObject);
begin
  with ResultTopForm do
  begin
    if Visible then Visible := false
               else Visible := true;
  end;
end;

procedure TGraphForm.ResultClick(Sender: TObject);
begin
  with ResultForm do
  begin
    WaitPanel.Visible := false;
    if Visible then Visible := false
               else Visible := true;
  end;
end;

procedure TGraphForm.TopScrollBoxClick(Sender: TObject);
begin
  ActiveTopBox := nil;
end;

procedure TGraphForm.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  ResultForm.Close;
  ResultTopForm.Close;
end;

procedure TGraphForm.FormDestroy(Sender: TObject);
begin
  TopList.Free;
end;

procedure TGraphForm.Exit1Click(Sender: TObject);
begin
  Close;
end;

procedure TGraphForm.ZoomIn1Click(Sender: TObject);
begin
  ZoomMinClick(ZoomMax);
end;

procedure TGraphForm.ZoomOut1Click(Sender: TObject);
begin
  ZoomMinClick(ZoomMin);
end;

procedure TGraphForm.ResultEnabled(Sender: TObject);
begin
  Graphinfo1.Enabled := Result.Enabled;
end;

procedure TGraphForm.ResultTopEnabled(Sender: TObject);
begin
  Topinfo1.Enabled := ResultTop.Enabled;
end;

procedure TGraphForm.ZoomMinEnabled(Sender: TObject);
begin
  ZoomOut1.Enabled := ZoomMin.Enabled;
end;

procedure TGraphForm.ZoomMaxEnabled(Sender: TObject);
begin
  ZoomIn1.Enabled := ZoomMax.Enabled;
end;

procedure TGraphForm.ZoomOneEnabled(Sender: TObject);
begin
  Showall1.Enabled := ZoomOne.Enabled;
end;

end.

