$Constant
	x_st11 	: integer = 150
	y_st11 	: integer = 100
	gx_st11	: integer = 100
 	gy_st11	: integer = 50
	
	x_st12 	: integer = 150
	y_st12 	: integer = 160
	gx_st12	: integer = 100
 	gy_st12	: integer = 50
	
	x_st13 	: integer = 150
	y_st13 	: integer = 220
	gx_st13	: integer = 100
 	gy_st13	: integer = 50
	
	x_st2 	: integer = 400
	y_st2 	: integer = 100
	gx_st2	: integer = 80
 	gy_st2	: integer = 30
	
	x1		: integer = 50 
	y1 		: integer = 160
	//l  : integer = 100
	h  		: integer = 10
	hag		: integer = 7
	
	h2		: integer = 15
	
	x2		: integer = 400 
	y2 		: integer = 160
	
	x3		: integer = 20 
	y3 		: integer = 450
	
	x4		: integer = 300 
	y4 		: integer = 450
	
	x5		: integer = 500 
	y5 		: integer = 450
	
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