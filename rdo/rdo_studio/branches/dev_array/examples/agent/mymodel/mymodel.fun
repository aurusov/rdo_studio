$Sequence время_между_приходами_заявок: real
$Type = exponential 349687
$End

$Function агент_обрабатывающий_заявку: such_as агенты.имя_агента
$Type = algorithmic
$Parameters
	заявка: such_as входящие_заявки.предмет_заказа
$Body
	if заявка == вал or заявка == корпус or заявка == шестерня result = производственный
	if заявка == документация result = другой
$End
