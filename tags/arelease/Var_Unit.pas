unit Var_Unit;

interface

uses IniFiles, TopBox_Unit;

type

  ChipNumType = record
    Mesto: integer;
    Look : boolean;
  end;

var
  MouseX, MouseY: integer;   // Координаты в активном ScrollBox'е
                             //   (для вставки нового box'а, arrow)
  ScrollX, ScrollY: integer; // Координаты TopScrollBox.Horz|Vert ScrollBar
  IniFile: TIniFile;    // ini file (*.ini)
// TopBox
  TopBox, TopBoxFirst, TopBoxLast: PTopBox;
  // Как перерисовывать/перемещать Box
  TopBoxMode: (Normal, MyBoxDrag, SizeUp, SizeDown, SizeLeft, SizeRight,
               SizeUpLeft, SizeUpRight, SizeDownLeft, SizeDownRight);
  MyTopBoxChange: boolean; // Перерисовка/перемещение Box'а
  // Для первоначальной отрисовке в 'Создание ситуации'
  OldChip: array [1..6] of ChipNumType;

implementation

end.
