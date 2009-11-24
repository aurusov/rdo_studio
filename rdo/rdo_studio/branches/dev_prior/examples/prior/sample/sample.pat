$Pattern образец_прихода_детали : irregular_event trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Create
$Time = 3
$Body
_деталь
	Convert_event
	trace
		тип set тип_детали
$End

$Pattern образец_операции_1 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep Keep
	_станок: станки Keep Keep
	_система: системы Keep Keep	
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
_система
	Choice from true
	Convert_end
		детали_типа_1 set _система.детали_типа_1 + деталь_готова(1, тип_детали)
		детали_типа_2 set _система.детали_типа_2 + деталь_готова(2, тип_детали)
		детали_типа_3 set _система.детали_типа_3 + деталь_готова(3, тип_детали)
$End

$Pattern образец_операции_2 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь: детали Keep Keep
	_станок: станки Keep Keep
	_система: системы Keep Keep	
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
_система
	Choice from true
	Convert_end
		детали_типа_1 set _система.детали_типа_1 + деталь_готова(1, тип_детали)
		детали_типа_2 set _система.детали_типа_2 + деталь_готова(2, тип_детали)
		детали_типа_3 set _система.детали_типа_3 + деталь_готова(3, тип_детали)
$End

$Pattern образец_операции_3 : operation trace
$Parameters
	тип_детали: integer
$Relevant_resources
	_деталь : детали  Keep Keep
	_станок : станки  Keep Keep
	_система: системы Keep Keep	
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
_система
	Choice from true
	Convert_end
		детали_типа_1 set _система.детали_типа_1 + деталь_готова(1, тип_детали)
		детали_типа_2 set _система.детали_типа_2 + деталь_готова(2, тип_детали)
		детали_типа_3 set _система.детали_типа_3 + деталь_готова(3, тип_детали)
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
	_приоритет: приоритеты Keep NoChange
$Time = 0
$Body
_приоритет
	Choice from _приоритет.номер = номер_типа_деталей and _приоритет.значение > 1
	Convert_begin
		значение set _приоритет.значение - 1
$End
