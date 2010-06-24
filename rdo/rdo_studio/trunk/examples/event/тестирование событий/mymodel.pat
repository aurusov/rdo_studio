$Pattern событие_прихода_клиента: event
$Relevant_resources
	_Парикмахерская: Парикмахерская Keep
$Body
_Парикмахерская
	Convert_event
		событие_прихода_клиента.planning(time_now + 20);
		количество_в_очереди += 1;
$End

$Pattern событие_окончания_работы: event
$Relevant_resources
	_Парикмахерская: Парикмахерская Keep
$Body
_Парикмахерская
	Convert_event
		событие_прихода_клиента.stopping;
$End
