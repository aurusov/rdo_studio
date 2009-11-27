$Pattern  Образец_Поломка_Ресурса : keyboard
$Parameters
	Индекс: integer
$Relevant_resources
	Ресурс : Ресурсы Keep NoChange
$Time = 0.0
$Body
Ресурс
	Choice from Ресурс.Индекс = Индекс
	Convert_begin
		Состояние set новое_состояние_ресурса(Ресурс.Состояние)
$End
