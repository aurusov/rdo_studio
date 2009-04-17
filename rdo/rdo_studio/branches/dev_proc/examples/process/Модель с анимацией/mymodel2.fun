$Constant
	x_st1 	: integer = 150
	y_st1 	: integer = 100
	gx_st1	: integer = 100
 	gy_st1	: integer = 50
	
	x_st2 	: integer = 400
	y_st2 	: integer = 100
	gx_st2	: integer = 80
 	gy_st2	: integer = 30
	
	x1		: integer = 150 
	y1 		: integer = 160
	//l  : integer = 100
	h  		: integer = 10
	hag		: integer = 7
	
	h2		: integer = 15
	
	x2		: integer = 400 
	y2 		: integer = 150
	
	x3		: integer = 20 
	y3 		: integer = 200
	
	x4		: integer = 300 
	y4 		: integer = 200
	
	x5		: integer = 500 
	y5 		: integer = 200
	
	//l  : integer = 100
	
$End

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