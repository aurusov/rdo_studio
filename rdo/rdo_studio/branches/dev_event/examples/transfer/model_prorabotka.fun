$Constant
	тоннаж : real = 60.0
	вагоны_в_составе: integer = 60
$End

$Sequence ф_приход_заявок: integer [0..3000]
$Type = normal 3487478
$End

$Function ф_приведение_к_ближайшему_большему_целому: integer = 0
$Type = algorithmic
$Parameters
	параметр: real
$Body
	if (Round(параметр) <  параметр) result = Round(параметр) + 1
	if (Round(параметр) >= параметр) result = Round(параметр)
$End

$Sequence ф_вес_груза : real 
$Type = uniform 34556
$End

$Sequence ф_тип_маршрута: such_as заявки.маршрут
$Type = by_hist 7833
$Body
    1 2 5.0
	2 3 2.0
	3 3 3.0
$End

$Sequence ф_создания_состава : real
$Type = normal 235
$End

$Sequence ф_погрузки : real
$Type = normal 768434
$End

$Sequence ф_время_в_пути : real
$Type = normal 0876543
$End

$Sequence ф_прохождение_таможни : real
$Type = normal 545478
$End

$Sequence ф_смена_колеи : real
$Type = normal 54747848
$End

$Sequence ф_разгрузка : real
$Type = normal 321258
$End

$Sequence ф_мойка : real
$Type = normal 45789211
$End

$Sequence ф_возврата_вагонов_в_парк : real
$Type = normal 80504030890
$End