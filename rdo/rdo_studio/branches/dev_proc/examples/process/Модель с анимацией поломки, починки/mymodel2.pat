$Pattern  Образец_Поломка_Станка11 : keyboard
$Relevant_resources
	rel_ST11 : ST11 Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from 0 = 0
	first
	Convert_begin
		Состояние set новое_состояние11(ST11.Состояние)
$End

$Pattern  Образец_Поломка_Станка12 : keyboard
$Relevant_resources
	rel_ST11 : ST12	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from 0 = 0
	first
	Convert_begin
		Состояние set новое_состояние12(ST12.Состояние)
$End

$Pattern  Образец_Поломка_Станка13 : keyboard
$Relevant_resources
	rel_ST11 : ST13	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from 0 = 0
	first
	Convert_begin
		Состояние set новое_состояние13(ST13.Состояние)
$End

$Pattern  Образец_Поломка_Станка2 : keyboard
$Relevant_resources
	rel_ST11 : STANOK2	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from 0 = 0
	first
	Convert_begin
		Состояние set новое_состояние2(STANOK2.Состояние)
$End

$Pattern  Образец_Поломка_Робота : keyboard
$Relevant_resources
	rel_ST11 : ROBOT	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from 0 = 0
	first
	Convert_begin
		Состояние set новое_состояние_робота(ROBOT.Состояние)
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