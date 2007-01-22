$Pattern ќбразец_начала_поворота: rule
$Relevant_resources
	_робот: –оботы Keep
$Body
_робот
	Choice from _робот.действие = нет and _робот.поворот <> _робот.куда_поворот
	Convert_rule
		действие set поворот
$End

$Pattern ќбразец_поворота_робота: operation
$Relevant_resources
	_робот: –оботы Keep Keep
$Time = 1
$Body
_робот
	Choice from _робот.действие = поворот
	Convert_begin
		действие set начали_поворот
	Convert_end
		поворот  set ѕриведение_угла( _робот.поворот + ¬_какую_сторону_крутить( _робот.поворот, _робот.куда_поворот ) )
		действие set ≈ще_поворачивать( _робот.поворот, _робот.куда_поворот )
$End

$Pattern ќбразец_начала_движени€: rule
$Relevant_resources
	_робот: –оботы Keep
$Body
_робот
	Choice from _робот.действие = нет and [abs(_робот.x - _робот.куда_x) > 2 or abs(_робот.y - _робот.куда_y) > 2]
	Convert_rule
		действие    set движение
		направление set ”гол_между( _робот.x, _робот.y, _робот.куда_x, _робот.куда_y )
$End

$Pattern ќбразец_перемещени€_робота: operation
$Relevant_resources
	_робот: –оботы Keep Keep
$Time = 1
$Body
_робот
	Choice from _робот.действие = движение
	Convert_begin
		действие set начали_движение
	Convert_end
		x           set _робот.x + ѕроверить_приращение(_робот.x, _робот.куда_x, Ќаправление_движени€(_робот.x, _робот.куда_x) * cos(_робот.направление))
		y           set _робот.y - ѕроверить_приращение(_робот.y, _робот.куда_y, Ќаправление_движени€(_робот.y, _робот.куда_y) * sin(_робот.направление))
		действие    set ≈ще_двигатьс€( _робот.x, _робот.y, _робот.куда_x, _робот.куда_y )
$End

$Pattern ќбразец_поворота_первого_робота: keyboard
$Parameters
	Ќаправление_поворота: integer
$Relevant_resources
	_робот: –обот_1 Keep NoChange
$Time = 1
$Body
_робот
	Convert_begin
		поворот set _робот.поворот + Ќаправление_поворота * 5
$End

$Pattern ќбразец_перемещени€_первого_робота: keyboard
$Parameters
	Ќаправление_движени€: integer
$Relevant_resources
	_робот: –обот_1 Keep NoChange
$Time = 1
$Body
_робот
	Convert_begin
		x set _робот.x + Ќаправление_движени€ * 5 * cos((_робот.поворот) * pi / 180)
		y set _робот.y - Ќаправление_движени€ * 5 * sin((_робот.поворот) * pi / 180)
$End

$Pattern ќбразец_врем€_вперед : irregular_event
$Relevant_resources
	_робот: –обот_1 Keep
$Time = 10
$Body
_робот
	Convert_event
		поворот set _робот.поворот
$End
