unit Const_Unit;

interface

uses Var_Unit;

const

  FMSModelName = 'fms';
  
  TVOTO    = 0;
  TVTU     = 1;
  TVSklad  = 2;
  TVNomenk = 3;
  TVPoint  = 4;
  TVConst  = 5;
  FMSPanelCount = 6;
  MyEOL = #13#10;

  Cap: array[boolean] of string = ('Показать', 'Убрать');
  LowToStr: array[TModelLow] of string = ('Равномерный', 'Нормальный', 'Экспоненциальный', 'Постоянный');
  LowToInt: array[TModelLow] of integer = (0, 1, 2, 3);
  IntToLow: array[0..3] of TModelLow = (mlUniform, mlNormal, mlExponential, mlConst);

implementation

end.
