$Pattern  Образец_Поломка_Станка : keyboard
$Parameters
	Индекс: integer
$Relevant_resources
	Станок : Станки Keep NoChange
$Time = 0.0
$Body
Станок
	Choice from Станок.Индекс = Индекс
	Convert_begin
		Состояние set новое_состояние(Станок.Состояние)
$End

$Pattern  Образец_Поломка_Робота : keyboard 
$Relevant_resources
	_Робот : Робот	Keep NoChange
$Time = 0.0
$Body
_Робот
	Convert_begin
		Состояние set новое_состояние_робота(_Робот.Состояние)
$End
