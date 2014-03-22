$Sequence Интервал_прихода : real
$Type = exponential 123456789
$End

$Sequence Длительность_обслуживания : real
$Type = uniform 123456789
$End

$Sequence Тип_клиента : such_as Клиенты.тип
$Type = by_hist 123456789
$Body
	Тип1 1.0
	Тип2 5.0
$End
