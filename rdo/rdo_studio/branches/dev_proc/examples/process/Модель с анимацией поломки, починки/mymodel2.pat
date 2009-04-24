$Pattern  Образец_Поломка : keyboard
$Relevant_resources
	rel_ST11 : ST11	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from rel_ST11.Состояние <> Сломан
	first
	Convert_begin
		Состояние set Сломан
$End

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
$End