$Constant
	тоннаж : real = 60.0
	вагоны_в_составе: integer = 60
$End

$Sequence ф_приход_за€вок: integer [0..3000]
$Type = normal 3487478
$End

$Sequence ф_вес_груза : real 
$Type = uniform 34556
$End

$Sequence ф_тип_маршрута: such_as за€вки.маршрут
$Type = by_hist 7833
$Body
    1 2 50.0
	2 3 20.0
	3 3 30.0
$End

$Sequence ф_создани€_состава : real[0..100000]
$Type = normal 235
$End

$Sequence ф_погрузки : real[0..100000]
$Type = normal 768434
$End

$Function ф_врем€_в_пути : real 
$Type = algorithmic
$Parameters
  х : real 
$Body
	result = х*10/500
$End

$Sequence ф_прохождение_таможни : real[0..100000]
$Type = normal 545478
$End

$Sequence ф_смена_колеи : real[0..100000]
$Type = normal 54747848
$End

$Sequence ф_разгрузка : real[0..100000]
$Type = normal 321258
$End

$Sequence ф_мойка : real[0..100000]
$Type = normal 45789211
$End

$Sequence ф_возврата_вагонов_в_парк : real[0..100000]
$Type = normal 80504030890
$End