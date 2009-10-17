$Pattern образец_прихода_детали : irregular_event trace
$Relevant_resources
	_деталь: детали Create
$Time = 0
$Body
_деталь
	Convert_event
	trace
		тип set розыгрыш_типа
$End

$Pattern образец_операции_1 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep Keep
	_станок: станки Keep Keep
$Time = время_операции(1, тип_детали, _станок.номер)
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 0 and _деталь.тип = тип_детали
	Convert_begin
		статус set обрабатывается
	Convert_end
		статус set свободна
		операция set _деталь.операция + 1
_станок
	Choice from _станок.статус = свободен and время_операции(1, тип_детали, _станок.номер) > 0
	Convert_begin
		статус set занят
	Convert_end
		статус set свободен
$End

$Pattern образец_операции_2 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep Keep
	_станок: станки Keep Keep
$Time = время_операции(2, тип_детали, _станок.номер)
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 1 and _деталь.тип = тип_детали
	Convert_begin
		статус set обрабатывается
	Convert_end
		статус set свободна
		операция set _деталь.операция + 1
_станок
	Choice from _станок.статус = свободен and время_операции(2, тип_детали, _станок.номер) > 0
	Convert_begin
		статус set занят
	Convert_end
		статус set свободен
$End

$Pattern образец_операции_3 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep Keep
	_станок: станки Keep Keep
$Time = время_операции(3, тип_детали, _станок.номер)
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 2 and _деталь.тип = тип_детали
	Convert_begin
		статус set обрабатывается
	Convert_end
		статус set свободна
		операция set _деталь.операция + 1
_станок
	Choice from _станок.статус = свободен and время_операции(3, тип_детали, _станок.номер) > 0
	Convert_begin
		статус set занят
	Convert_end
		статус set свободен
$End

$Pattern образец_пропуска_операции_1 : rule trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 0 and _деталь.тип = тип_детали and For_All(станки: (время_операции(1, тип_детали, станки.номер) = 0) )
	Convert_rule
		операция set _деталь.операция + 1
$End

$Pattern образец_пропуска_операции_2 : rule trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 1 and _деталь.тип = тип_детали and For_All(станки: (время_операции(2, тип_детали, станки.номер) = 0) )
	Convert_rule
		операция set _деталь.операция + 1
$End

$Pattern образец_пропуска_операции_3 : rule trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep
$Body
_деталь
	Choice from _деталь.статус = свободна and _деталь.операция = 2 and _деталь.тип = тип_детали and For_All(станки: (время_операции(3, тип_детали, станки.номер) = 0) )
	Convert_rule
		операция set _деталь.операция + 1
$End

$Pattern образец_увеличения_приоритета : keyboard trace
$Parameters
	номер_типа_деталей : integer
$Relevant_resources
	_приоритет: приоритеты NoChange Keep
$Time = 0
$Body
_приоритет
	Choice from _приоритет.номер = номер_типа_деталей
	Convert_end
		значение set _приоритет.значение + 1
$End

$Pattern образец_уменьшения_приоритета : keyboard trace
$Parameters
	номер_типа_деталей : integer
$Relevant_resources
	_приоритет: приоритеты NoChange Keep
$Time = 0
$Body
_приоритет
	Choice from _приоритет.номер = номер_типа_деталей and _приоритет.значение > 1
	Convert_end
		значение set _приоритет.значение - 1
$End
