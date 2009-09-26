$Pattern фиктивный_патерн : rule trace
$Relevant_resources
	_Парикмахерская: Парикмахерская Keep
$Body
_Парикмахерская
	Choice from _Парикмахерская.количество_в_очереди > 0
	Convert_rule
		состояние_парикмахера set Занят
$End
