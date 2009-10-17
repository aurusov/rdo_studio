$Constant
	количество_деталей : integer = 25
$End

$Function время_операции : integer[0..10]
$Type = table
$Parameters
	номер_операции: integer[1..3]
	номер_типа_детали: integer[1..3]
	номер_станка: integer[1..4]
$Body
	10	0	0
	7	0	0
	0	0	0

	6	8	7
	4	0	0
	0	4	5

	0	10	0
	0	0	0
	0	7	0

	0	0	8
	0	0	0
	0	0	7
$End

$Sequence розыгрыш_типа : integer[1..3]
$Type = by_hist 123456789
$Body
	1	2	1
	2	3	1
	3	3	1
$End
