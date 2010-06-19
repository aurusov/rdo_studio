$Constant
	тоннаж : real = 60.0
	вагоны_в_составе: integer = 60
$End

/*$Sequence ф_приход_заявок: real [0..3000]
$Type = uniform 3487478
$End*/

$Function ф_приход_заявок : real
$Type = algorithmic
$Parameters
	x: integer
$Body
	result = 720/x
$End


$Sequence ф_вес_груза : real 
$Type = uniform 34556
$End

$Sequence ф_тип_маршрута: such_as заявки.маршрут
$Type = by_hist 1111
$Body
    1 2 5.0
	2 3 2.0
	3 3 3.0
$End

$Sequence ф_создания_состава : real[0..100000]
$Type = uniform 235
$End

$Sequence ф_погрузки : real[0..100000]
$Type = uniform 768434
$End

$Function ф_время_в_пути : real 
$Type = algorithmic
$Parameters
  х : real 
$Body
	result = х*24/500
$End

$Sequence ф_прохождение_таможни : real[0..100000]
$Type = uniform 545478
$End

$Sequence ф_смена_колеи : real[0..100000]
$Type = uniform 54747848
$End

$Sequence ф_разгрузка : real[0..100000]
$Type = uniform 321258
$End

$Sequence ф_мойка : real[0..100000]
$Type = uniform 45789211
$End

$Sequence ф_возврата_вагонов_в_парк : real[0..100000]
$Type = uniform 80504030890
$End