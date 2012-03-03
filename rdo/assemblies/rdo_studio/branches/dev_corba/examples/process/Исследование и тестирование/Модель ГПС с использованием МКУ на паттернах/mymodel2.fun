$Sequence Интервал_прихода_заготовок : real
$Type = exponential 12347
$End

$Sequence Обработка_на_станке_1 : real
$Type = normal 12346
$End

$Sequence Время_захвата : real
$Type = uniform 67891
$End


$Sequence Время_отпуска : real
$Type = uniform 67892
$End


$Sequence Обработка_на_станке_2 : real
$Type = exponential 12345
$End


$Function Новый_накопитель : such_as Накопители.Положение = ВхНак
$Type = list
$Parameters
	текущее_положение : such_as Накопители.Положение
$Body
	ВхНак 	  = ВхНакСт1 
	ВыхНакСт1 = ВхНакСт2 
	ВыхНакСт2 = ВыхНак	
$End

$Function Новое_положение : such_as Детали.Положение = ВхНак
$Type = list
$Parameters
	текущее_положение : such_as Детали.Положение
$Body
	ВхНак     = state1 
	ВыхНакСт1 = state2 
	ВыхНакСт2 = state3	
$End

$Function Время_на_участке : integer = 0
$Type = list
$Parameters
	положение_накопителя : such_as Накопители.Положение
$Body
	ВхНак     = 6
	ВыхНакСт1 = 3
	ВыхНакСт2 = 5	
$End