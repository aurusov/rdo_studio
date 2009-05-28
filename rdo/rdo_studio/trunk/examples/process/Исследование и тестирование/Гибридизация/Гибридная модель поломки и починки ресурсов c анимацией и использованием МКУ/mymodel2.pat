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
		Состояние set новое_состояние11(Станок.Состояние)
$End

$Pattern  Образец_Поломка_Станка2 : keyboard
$Relevant_resources
	Станок : STANOK2	Keep NoChange
$Time = 0.0
$Body
Станок
	Convert_begin
		Состояние set новое_состояние2(Станок.Состояние)
$End

$Pattern  Образец_Поломка_Робота : keyboard 
$Relevant_resources
	Робот : ROBOT	Keep NoChange
$Time = 0.0
$Body
Робот
	Convert_begin
		Состояние set новое_состояние_робота(Робот.Состояние)
$End

/*
$Pattern  Образец_Починка : keyboard
$Relevant_resources
	rel_ST11 : ST11	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from rel_ST11.Состояние = Сломан
	first
	Convert_begin
		Состояние set Свободен
$End*/