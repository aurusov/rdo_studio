$Frame fram_1
$Back_picture = <255 255 255> 1080 680
Show

text    [10, 5,  50, 25, <127 127 127>, <100 255 0>, 'Время:' ]
text    [60, 5, 150, 25, <127 127 127>, <100 255 0>, Time_now ]
//rect	[x_st11, y_st11, gx_st11, gy_st11, <255 255 127>, <200 255 100> ]

//rect	[150,70,730,450, <255 255 255>]

//line	[0, y_mid, 1080, 400, <0  0 0>]
ellipse [x_mid - l_rob/2, y_mid - l_rob/2, l_rob, l_rob, <255 255 255>, <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <255 255 255>, <0 0 0>] 
/*
//----------------------повернутый прямоугольник------------------------------------
triang	[x_st11, y_st11, x_st11 + l1*cos(a1), 
		 y_st11 - l1*sin(a1), x_st11 + l1*cos(a1) + h1*sin(a1),
		 y_st11 - l1*sin(a1)+h1*cos(a1),
		 <127 127 127>, <127 127 127>]
triang	[x_st11 + l1*cos(a1) + h1*sin(a1), y_st11 - l1*sin(a1)+h1*cos(a1), 
		 x_st11+h1*sin(a1), y_st11 + h1*cos(a1),
		 x_st11, y_st11,
		 <127 127 127>, <127 127 127>]
		 
line	[x_st11, y_st11, x_st11 + l1*cos(a1), y_st11 - l1*sin(a1), <0  0 0>]
line	[x_st11 + l1*cos(a1), y_st11 - l1*sin(a1), x_st11 + l1*cos(a1) + h1*sin(a1), y_st11 - l1*sin(a1)+h1*cos(a1), <0  0 0>]
line	[x_st11 + l1*cos(a1) + h1*sin(a1), y_st11 - l1*sin(a1)+h1*cos(a1), x_st11+h1*sin(a1), y_st11 + h1*cos(a1), <0  0 0>]
line	[x_st11+h1*sin(a1), y_st11 + h1*cos(a1), x_st11, y_st11, <0  0 0>]
*/
/*
//----------------------повернутый прямоугольник------------------------------------
triang	[x_q1 - h1*sin(a1), 			  			 	y_q1 - h*cos(a1), 
		 x_q1 - h1*sin(a1) + l1*cos(a1),   				y_q1 - h*cos(a1) - l1*sin(a1), 
		 x_q1 - h1*sin(a1) + l1*cos(a1) + h1*sin(a1), 	y_q1 - h*cos(a1) - l1*sin(a1) + h1*cos(a1),
		 <127 127 127>, <127 127 127>]
triang	[x_q1 - h1*sin(a1) + l1*cos(a1) + h1*sin(a1),	y_q1 - h*cos(a1) - l1*sin(a1) + h1*cos(a1), 
		 x_q1 - h1*sin(a1) + h1*sin(a1), 				y_q1 - h*cos(a1) + h1*cos(a1),
		 x_q1 - h1*sin(a1), 							y_q1 - h*cos(a1),
		 <127 127 127>, <127 127 127>]
		 
line	[x_q1 - h1*sin(a1), 							y_q1 - h*cos(a1),
		 x_q1 - h1*sin(a1) + l1*cos(a1), 				y_q1 - h*cos(a1) - l1*sin(a1),
		 <0  0 0>]
line	[x_q1 - h1*sin(a1) + l1*cos(a1), 				y_q1 - h*cos(a1) - l1*sin(a1), 
		 x_q1 - h1*sin(a1) + l1*cos(a1) + h1*sin(a1), 	y_q1 - h*cos(a1) - l1*sin(a1) + h1*cos(a1), 
		 <0  0 0>]
		 
line	[x_q1 - h1*sin(a1) + l1*cos(a1) + h1*sin(a1), 	y_q1 - h*cos(a1) - l1*sin(a1) + h1*cos(a1), 
		 x_q1 - h1*sin(a1) + h1*sin(a1), 			  	y_q1 - h*cos(a1) + h1*cos(a1), 
		 <0  0 0>]
line	[x_q1 - h1*sin(a1) + h1*sin(a1), 			  	y_q1 - h*cos(a1) + h1*cos(a1), 
		 x_q1 - h1*sin(a1), 						  	y_q1 - h*cos(a1), 
		 <0  0 0>]
*/
ellipse [x_qi1, y_qi1, 3, 3, <255 255 255>, <0 0 0>]

//----------------------очередь накопитель - вход------------------------------------
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cos(ai), 			y_qi - l1*sin(ai),
		 x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai),
		 <127 127 127>, <0 0 127>]

triang	[x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai), 
		 x_qi + h1*sin(ai), 			y_qi + h1*cos(ai),
		 x_qi, 						 	y_qi,
		  <127 127 127>, <127 127 127>]
		  

//----------------------очередь накопитель - по длине очереди------------------------------------
show_if QIN.длина_очереди <= 8
triang	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.длина_очереди*cos(ai), 			   y_qi - h1*QIN.длина_очереди*sin(ai),
		 x_qi + h1*QIN.длина_очереди*cos(ai) + h1*sin(ai),y_qi - h1*QIN.длина_очереди*sin(ai) + h1*cos(ai),
		 <255 255 0>, <0 0 0>]

triang	[x_qi + h1*QIN.длина_очереди*cos(ai) + h1*sin(ai),y_qi - h1*QIN.длина_очереди*sin(ai) + h1*cos(ai), 
		 x_qi + h1*sin(ai), 								   y_qi + h1*cos(ai),
		 x_qi, 						 						   y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.длина_очереди*cos(ai) + h1*sin(ai),y_qi - h1*QIN.длина_очереди*sin(ai) + h1*cos(ai),
		 <255 255 0>]
		 
show_if QIN.длина_очереди > 8
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cos(ai), 			y_qi - l1*sin(ai),
		 x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai),
		 <255 255 0>, <0 0 0>]

triang	[x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai), 
		 x_qi + l1*sin(ai), 			y_qi + h1*cos(ai),
		 x_qi, 						 	y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai),
		 <255 255 0>]

show
//----------------------очередь накопитель - оконтовка------------------------------------

line	[x_qi, 						 	y_qi, 
		 x_qi + l1*cos(ai),			 	y_qi - l1*sin(ai), 
		 <0  0 0>]
line	[x_qi + l1*cos(ai),			 	y_qi - l1*sin(ai), 
		 x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai), 
		 <0  0 0>]
line	[x_qi + l1*cos(ai) + h1*sin(ai),y_qi - l1*sin(ai) + h1*cos(ai), 
		 x_qi + h1*sin(ai), 			y_qi + h1*cos(ai), 
		 <0  0 0>]
line	[x_qi+ h1*sin(ai), 				y_qi + h1*cos(ai), 
		 x_qi, 					     	y_qi, 
		 <0  0 0>]


//----------------------линии очереди накопитель - вход------------------------------------------------		 
		 
line	[x_qi + 1*h1*cos(ai), y_qi - 1*h1*sin(ai), x_qi + h1*sin(ai) + 1*h1*cos(ai), y_qi + h1*cos(ai) - 1*h1*sin(ai), <0 0 0>]
line	[x_qi + 2*h1*cos(ai), y_qi - 2*h1*sin(ai), x_qi + h1*sin(ai) + 2*h1*cos(ai), y_qi + h1*cos(ai) - 2*h1*sin(ai), <0 0 0>]
line	[x_qi + 3*h1*cos(ai), y_qi - 3*h1*sin(ai), x_qi + h1*sin(ai) + 3*h1*cos(ai), y_qi + h1*cos(ai) - 3*h1*sin(ai), <0 0 0>]
line	[x_qi + 4*h1*cos(ai), y_qi - 4*h1*sin(ai), x_qi + h1*sin(ai) + 4*h1*cos(ai), y_qi + h1*cos(ai) - 4*h1*sin(ai), <0 0 0>]
line	[x_qi + 5*h1*cos(ai), y_qi - 5*h1*sin(ai), x_qi + h1*sin(ai) + 5*h1*cos(ai), y_qi + h1*cos(ai) - 5*h1*sin(ai), <0 0 0>]
line	[x_qi + 6*h1*cos(ai), y_qi - 6*h1*sin(ai), x_qi + h1*sin(ai) + 6*h1*cos(ai), y_qi + h1*cos(ai) - 6*h1*sin(ai), <0 0 0>]
line	[x_qi + 7*h1*cos(ai), y_qi - 7*h1*sin(ai), x_qi + h1*sin(ai) + 7*h1*cos(ai), y_qi + h1*cos(ai) - 7*h1*sin(ai), <0 0 0>]

//----------------------число в очереди накопитель - вход------------------------------------------------		 
text    [ x_qi - 10, y_qi, 90, 25, <127 127 127>, <100 255 0>, QIN.длина_очереди ]

//----------------------очередь накопитель - выход------------------------------------
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cos(ao), 			y_qo - l1*sin(ao),
		 x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao),
		 <127 127 127>, <127 127 127>]

triang	[x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao), 
		 x_qo + h1*sin(ao), 			y_qo + h1*cos(ao),
		 x_qo, 						 	y_qo,
		  <127 127 127>, <127 127 127>]

//----------------------очередь накопитель - по длине очереди------------------------------------
show_if QOUT.длина_очереди <= 8
triang	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.длина_очереди*cos(ao), 			   y_qo - h1*QOUT.длина_очереди*sin(ao),
		 x_qo + h1*QOUT.длина_очереди*cos(ao) + h1*sin(ao),y_qo - h1*QOUT.длина_очереди*sin(ao) + h1*cos(ao),
		 <255 255 0>, <0 0 0>]

triang	[x_qo + h1*QOUT.длина_очереди*cos(ao) + h1*sin(ao),y_qo - h1*QOUT.длина_очереди*sin(ao) + h1*cos(ao), 
		 x_qo + h1*sin(ao), 								   y_qo + h1*cos(ao),
		 x_qo, 						 						   y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.длина_очереди*cos(ao) + h1*sin(ao),y_qo - h1*QOUT.длина_очереди*sin(ao) + h1*cos(ao),
		 <255 255 0>]
		 
show_if QOUT.длина_очереди > 8
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cos(ao), 			y_qo - l1*sin(ao),
		 x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao),
		 <255 255 0>, <0 0 0>]

triang	[x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao), 
		 x_qo + l1*sin(ao), 			y_qo + h1*cos(ao),
		 x_qo, 						 	y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao),
		 <255 255 0>]

show
//----------------------очередь накопитель - оконтовка------------------------------------
line	[x_qo, 						 	y_qo, 
		 x_qo + l1*cos(ao),			 	y_qo - l1*sin(ao), 
		 <0  0 0>]
line	[x_qo + l1*cos(ao),			 	y_qo - l1*sin(ao), 
		 x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao), 
		 <0  0 0>]
line	[x_qo + l1*cos(ao) + h1*sin(ao),y_qo - l1*sin(ao) + h1*cos(ao), 
		 x_qo + h1*sin(ao), 			y_qo + h1*cos(ao), 
		 <0  0 0>]
line	[x_qo+ h1*sin(ao), 				y_qo + h1*cos(ao), 
		 x_qo, 					     	y_qo, 
		 <0  0 0>]

//----------------------линии очереди накопитель - выход------------------------------------------------		 
		 
line	[x_qo + 1*h1*cos(ao), y_qo - 1*h1*sin(ao), x_qo + h1*sin(ao) + 1*h1*cos(ao), y_qo + h1*cos(ao) - 1*h1*sin(ao), <0 0 0>]
line	[x_qo + 2*h1*cos(ao), y_qo - 2*h1*sin(ao), x_qo + h1*sin(ao) + 2*h1*cos(ao), y_qo + h1*cos(ao) - 2*h1*sin(ao), <0 0 0>]
line	[x_qo + 3*h1*cos(ao), y_qo - 3*h1*sin(ao), x_qo + h1*sin(ao) + 3*h1*cos(ao), y_qo + h1*cos(ao) - 3*h1*sin(ao), <0 0 0>]
line	[x_qo + 4*h1*cos(ao), y_qo - 4*h1*sin(ao), x_qo + h1*sin(ao) + 4*h1*cos(ao), y_qo + h1*cos(ao) - 4*h1*sin(ao), <0 0 0>]
line	[x_qo + 5*h1*cos(ao), y_qo - 5*h1*sin(ao), x_qo + h1*sin(ao) + 5*h1*cos(ao), y_qo + h1*cos(ao) - 5*h1*sin(ao), <0 0 0>]
line	[x_qo + 6*h1*cos(ao), y_qo - 6*h1*sin(ao), x_qo + h1*sin(ao) + 6*h1*cos(ao), y_qo + h1*cos(ao) - 6*h1*sin(ao), <0 0 0>]
line	[x_qo + 7*h1*cos(ao), y_qo - 7*h1*sin(ao), x_qo + h1*sin(ao) + 7*h1*cos(ao), y_qo + h1*cos(ao) - 7*h1*sin(ao), <0 0 0>]
//----------------------число в очереди накопитель - выход------------------------------------------------		 
text    [ x_qo , y_qi, 90, 25, <127 127 127>, <100 255 0>, QOUT.длина_очереди ]




//----------------------очередь станка 1 - выход ------------------------------------
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cos(a1), 			y_qo1 - l1*sin(a1),
		 x_qo1 + l1*cos(a1) + h1*sin(a1),y_qo1 - l1*sin(a1) + h1*cos(a1),
		 <127 127 127>, <127 127 127>]

triang	[x_qo1 + l1*cos(a1) + h1*sin(a1),y_qo1 - l1*sin(a1) + h1*cos(a1), 
		 x_qo1 + h1*sin(a1), 			y_qo1 + h1*cos(a1),
		 x_qo1, 							y_qo1,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 1 - выход------------------------------------
show_if QEXITST1.длина_очереди <= 8
triang	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.длина_очереди*cos(a1), 			   y_qo1 - h1*QEXITST1.длина_очереди*sin(a1),
		 x_qo1 + h1*QEXITST1.длина_очереди*cos(a1) + h1*sin(a1),y_qo1 - h1*QEXITST1.длина_очереди*sin(a1) + h1*cos(a1),
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + h1*QEXITST1.длина_очереди*cos(a1) + h1*sin(a1),y_qo1 - h1*QEXITST1.длина_очереди*sin(a1) + h1*cos(a1), 
		 x_qo1 + h1*sin(a1), 								   y_qo1 + h1*cos(a1),
		 x_qo1, 						 						   y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.длина_очереди*cos(a1) + h1*sin(a1),y_qo1 - h1*QEXITST1.длина_очереди*sin(a1) + h1*cos(a1),
		 <255 255 0>]
		 
show_if QEXITST1.длина_очереди > 8
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cos(a1), 			y_qo1 - l1*sin(a1),
		 x_qo1 + l1*cos(a1) + h1*sin(a1),y_qo1 - l1*sin(a1) + h1*cos(a1),
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + l1*cos(a1) + h1*sin(a1),y_qo1 - l1*sin(a1) + h1*cos(a1), 
		 x_qo1 + h1*sin(a1), 			y_qo1 + h1*cos(a1),
		 x_qo1, 						 	y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + l1*cos(a1) + h1*sin(a1),y_qo1 - l1*sin(a1) + h1*cos(a1),
		 <255 255 0>]

show
//----------------------очередь станка1 выход - оконтовка------------------------------------


line	[x_qo1, 								y_qo1, 
		 x_qo1 + l1*cos(a1),					y_qo1 - l1*sin(a1), 
		 <0  0 0>]
line	[x_qo1 + l1*cos(a1),					y_qo1 - l1*sin(a1), 
		 x_qo1 + l1*cos(a1) + h1*sin(a1), 	y_qo1 - l1*sin(a1) + h1*cos(a1), 
		 <0  0 0>]
line	[x_qo1 + l1*cos(a1) + h1*sin(a1), 	y_qo1 - l1*sin(a1) + h1*cos(a1), 
		 x_qo1 + h1*sin(a1), 				y_qo1 + h1*cos(a1), 
		 <0  0 0>]
line	[x_qo1 + h1*sin(a1), 				y_qo1 + h1*cos(a1), 
		 x_qo1, 								y_qo1, 
		 <0  0 0>]
//----------------------линии очереди - выход станка 1------------------------------------------------		 
		 
line	[x_qo1 + 1*h1*cos(a1), y_qo1 - 1*h1*sin(a1), x_qo1 + h1*sin(a1) + 1*h1*cos(a1), y_qo1 + h1*cos(a1) - 1*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 2*h1*cos(a1), y_qo1 - 2*h1*sin(a1), x_qo1 + h1*sin(a1) + 2*h1*cos(a1), y_qo1 + h1*cos(a1) - 2*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 3*h1*cos(a1), y_qo1 - 3*h1*sin(a1), x_qo1 + h1*sin(a1) + 3*h1*cos(a1), y_qo1 + h1*cos(a1) - 3*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 4*h1*cos(a1), y_qo1 - 4*h1*sin(a1), x_qo1 + h1*sin(a1) + 4*h1*cos(a1), y_qo1 + h1*cos(a1) - 4*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 5*h1*cos(a1), y_qo1 - 5*h1*sin(a1), x_qo1 + h1*sin(a1) + 5*h1*cos(a1), y_qo1 + h1*cos(a1) - 5*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 6*h1*cos(a1), y_qo1 - 6*h1*sin(a1), x_qo1 + h1*sin(a1) + 6*h1*cos(a1), y_qo1 + h1*cos(a1) - 6*h1*sin(a1), <0 0 0>]
line	[x_qo1 + 7*h1*cos(a1), y_qo1 - 7*h1*sin(a1), x_qo1 + h1*sin(a1) + 7*h1*cos(a1), y_qo1 + h1*cos(a1) - 7*h1*sin(a1), <0 0 0>]
//----------------------число в очереди - выход станок 1------------------------------------------------		 
text    [ x_qo1 + 10, y_qo1 - 30, 90, 25, <127 127 127>, <100 255 0>, QEXITST1.длина_очереди ]


//----------------------очередь станка 1 - вход------------------------------------
triang	[x_qi1, 						y_qi1, 
		 x_qi1 + l1*cos(a1), 			y_qi1 - l1*sin(a1),
		 x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1),
		 <127 127 127>, <127 127 127>]

triang	[x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1), 
		 x_qi1 + h1*sin(a1), 			y_qi1 + h1*cos(a1),
		 x_qi1, 							y_qi1,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 1 - вход------------------------------------
show_if QSTANOK1.длина_очереди <= 8
triang	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.длина_очереди*cos(a1), 			y_qi1 - h1*QSTANOK1.длина_очереди*sin(a1),
		 x_qi1 + h1*QSTANOK1.длина_очереди*cos(a1) + h1*sin(a1),y_qi1 - h1*QSTANOK1.длина_очереди*sin(a1) + h1*cos(a1),
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + h1*QSTANOK1.длина_очереди*cos(a1) + h1*sin(a1),y_qi1 - h1*QSTANOK1.длина_очереди*sin(a1) + h1*cos(a1), 
		 x_qi1 + h1*sin(a1), 								    y_qi1 + h1*cos(a1),
		 x_qi1, 						 						y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.длина_очереди*cos(a1) + h1*sin(a1),y_qi1 - h1*QSTANOK1.длина_очереди*sin(a1) + h1*cos(a1),
		 <255 255 0>]
		 
show_if QSTANOK1.длина_очереди > 8
triang	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cos(a1), 			 y_qi1 - l1*sin(a1),
		 x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1),
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1), 
		 x_qi1 + h1*sin(a1), 			 y_qi1 + h1*cos(a1),
		 x_qi1, 						 y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1),
		 <255 255 0>]

show
//----------------------очередь станка1 вход - оконтовка------------------------------------

line	[x_qi1, 							y_qi1, 
		 x_qi1 + l1*cos(a1),				y_qi1 - l1*sin(a1), 
		 <0  0 0>]
line	[x_qi1 + l1*cos(a1),				y_qi1 - l1*sin(a1), 
		 x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1), 
		 <0  0 0>]
line	[x_qi1 + l1*cos(a1) + h1*sin(a1),y_qi1 - l1*sin(a1) + h1*cos(a1), 
		 x_qi1 + h1*sin(a1), 			y_qi1 + h1*cos(a1), 
		 <0  0 0>]
line	[x_qi1+ h1*sin(a1), 				y_qi1 + h1*cos(a1), 
		 x_qi1, 							y_qi1, 
		 <0  0 0>]

//----------------------линии очереди - вход станка 1------------------------------------------------		 
		 
line	[x_qi1 + 1*h1*cos(a1), y_qi1 - 1*h1*sin(a1), x_qi1 + h1*sin(a1) + 1*h1*cos(a1), y_qi1 + h1*cos(a1) - 1*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 2*h1*cos(a1), y_qi1 - 2*h1*sin(a1), x_qi1 + h1*sin(a1) + 2*h1*cos(a1), y_qi1 + h1*cos(a1) - 2*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 3*h1*cos(a1), y_qi1 - 3*h1*sin(a1), x_qi1 + h1*sin(a1) + 3*h1*cos(a1), y_qi1 + h1*cos(a1) - 3*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 4*h1*cos(a1), y_qi1 - 4*h1*sin(a1), x_qi1 + h1*sin(a1) + 4*h1*cos(a1), y_qi1 + h1*cos(a1) - 4*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 5*h1*cos(a1), y_qi1 - 5*h1*sin(a1), x_qi1 + h1*sin(a1) + 5*h1*cos(a1), y_qi1 + h1*cos(a1) - 5*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 6*h1*cos(a1), y_qi1 - 6*h1*sin(a1), x_qi1 + h1*sin(a1) + 6*h1*cos(a1), y_qi1 + h1*cos(a1) - 6*h1*sin(a1), <0 0 0>]
line	[x_qi1 + 7*h1*cos(a1), y_qi1 - 7*h1*sin(a1), x_qi1 + h1*sin(a1) + 7*h1*cos(a1), y_qi1 + h1*cos(a1) - 7*h1*sin(a1), <0 0 0>]
//----------------------число в очереди - вход станок 1------------------------------------------------		 
text    [ x_qi1 - 19, y_qi1 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK1.длина_очереди ]

//----------------------очередь станка 2 - выход ------------------------------------
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cos(a2), 			 y_qo2 - l1*sin(a2),
		 x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2),
		 <127 127 127>, <127 127 127>]

triang	[x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2), 
		 x_qo2 + h1*sin(a2), 			 y_qo2 + h1*cos(a2),
		 x_qo2, 						 y_qo2,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 2 - выход------------------------------------
show_if QEXITST2.длина_очереди <= 8
triang	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.длина_очереди*cos(a2), 			y_qo2 - h1*QEXITST2.длина_очереди*sin(a2),
		 x_qo2 + h1*QEXITST2.длина_очереди*cos(a2) + h1*sin(a2),y_qo2 - h1*QEXITST2.длина_очереди*sin(a2) + h1*cos(a2),
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + h1*QEXITST2.длина_очереди*cos(a2) + h1*sin(a2),y_qo2 - h1*QEXITST2.длина_очереди*sin(a2) + h1*cos(a2), 
		 x_qo2 + h1*sin(a2), 								    y_qo2 + h1*cos(a2),
		 x_qo2, 						 						y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.длина_очереди*cos(a2) + h1*sin(a2),y_qo2 - h1*QEXITST2.длина_очереди*sin(a2) + h1*cos(a2),
		 <255 255 0>]
		 
show_if QEXITST2.длина_очереди > 8
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cos(a2), 			 y_qo2 - l1*sin(a2),
		 x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2),
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2), 
		 x_qo2 + h1*sin(a2), 			 y_qo2 + h1*cos(a2),
		 x_qo2, 						 y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2),
		 <255 255 0>]

show
//----------------------очередь станка 2 выход - оконтовка------------------------------------

line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cos(a2),			 y_qo2 - l1*sin(a2), 
		 <0  0 0>]
line	[x_qo2 + l1*cos(a2),			 y_qo2 - l1*sin(a2), 
		 x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2), 
		 <0  0 0>]
line	[x_qo2 + l1*cos(a2) + h1*sin(a2),y_qo2 - l1*sin(a2) + h1*cos(a2), 
		 x_qo2 + h1*sin(a2), 			 y_qo2 + h1*cos(a2), 
		 <0  0 0>]
line	[x_qo2 + h1*sin(a2), 			 y_qo2 + h1*cos(a2), 
		 x_qo2, 						 y_qo2, 
		 <0  0 0>]
//----------------------линии очереди - выход станка 2------------------------------------------------		 
		 
line	[x_qo2 + 1*h1*cos(a2), y_qo2 - 1*h1*sin(a2), x_qo2 + h1*sin(a2) + 1*h1*cos(a2), y_qo2 + h1*cos(a2) - 1*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 2*h1*cos(a2), y_qo2 - 2*h1*sin(a2), x_qo2 + h1*sin(a2) + 2*h1*cos(a2), y_qo2 + h1*cos(a2) - 2*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 3*h1*cos(a2), y_qo2 - 3*h1*sin(a2), x_qo2 + h1*sin(a2) + 3*h1*cos(a2), y_qo2 + h1*cos(a2) - 3*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 4*h1*cos(a2), y_qo2 - 4*h1*sin(a2), x_qo2 + h1*sin(a2) + 4*h1*cos(a2), y_qo2 + h1*cos(a2) - 4*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 5*h1*cos(a2), y_qo2 - 5*h1*sin(a2), x_qo2 + h1*sin(a2) + 5*h1*cos(a2), y_qo2 + h1*cos(a2) - 5*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 6*h1*cos(a2), y_qo2 - 6*h1*sin(a2), x_qo2 + h1*sin(a2) + 6*h1*cos(a2), y_qo2 + h1*cos(a2) - 6*h1*sin(a2), <0 0 0>]
line	[x_qo2 + 7*h1*cos(a2), y_qo2 - 7*h1*sin(a2), x_qo2 + h1*sin(a2) + 7*h1*cos(a2), y_qo2 + h1*cos(a2) - 7*h1*sin(a2), <0 0 0>]
//----------------------число в очереди - вход станок 1------------------------------------------------		 
text    [ x_qo2 + 20, y_qo2, 90, 25, <127 127 127>, <100 255 0>, QEXITST2.длина_очереди ]

//----------------------очередь станка 2 - вход------------------------------------
triang	[x_qi2, 						y_qi2, 
		 x_qi2 + l1*cos(a2), 			y_qi2 - l1*sin(a2),
		 x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2),
		 <127 127 127>, <127 127 127>]

triang	[x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2), 
		 x_qi2 + h1*sin(a2), 			y_qi2 + h1*cos(a2),
		 x_qi2, 							y_qi2,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 2 - вход------------------------------------
show_if QSTANOK2.длина_очереди <= 8
triang	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.длина_очереди*cos(a2), 			y_qi2 - h1*QSTANOK2.длина_очереди*sin(a2),
		 x_qi2 + h1*QSTANOK2.длина_очереди*cos(a2) + h1*sin(a2),y_qi2 - h1*QSTANOK2.длина_очереди*sin(a2) + h1*cos(a2),
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + h1*QSTANOK2.длина_очереди*cos(a2) + h1*sin(a2),y_qi2 - h1*QSTANOK2.длина_очереди*sin(a2) + h1*cos(a2), 
		 x_qi2 + h1*sin(a2), 								    y_qi2 + h1*cos(a2),
		 x_qi2, 						 						y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.длина_очереди*cos(a2) + h1*sin(a2),y_qi2 - h1*QSTANOK2.длина_очереди*sin(a2) + h1*cos(a2),
		 <255 255 0>]
		 
show_if QSTANOK2.длина_очереди > 8
triang	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cos(a2), 			 y_qi2 - l1*sin(a2),
		 x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2),
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2), 
		 x_qi2 + h1*sin(a2), 			 y_qi2 + h1*cos(a2),
		 x_qi2, 						 y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2),
		 <255 255 0>]

show
//----------------------очередь станка 2 вход - оконтовка------------------------------------

line	[x_qi2, 							y_qi2, 
		 x_qi2 + l1*cos(a2),				y_qi2 - l1*sin(a2), 
		 <0  0 0>]
line	[x_qi2 + l1*cos(a2),				y_qi2 - l1*sin(a2), 
		 x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2), 
		 <0  0 0>]
line	[x_qi2 + l1*cos(a2) + h1*sin(a2),y_qi2 - l1*sin(a2) + h1*cos(a2), 
		 x_qi2 + h1*sin(a2), 			y_qi2 + h1*cos(a2), 
		 <0  0 0>]
line	[x_qi2+ h1*sin(a2), 				y_qi2 + h1*cos(a2), 
		 x_qi2, 							y_qi2, 
		 <0  0 0>]

//----------------------линии очереди - вход станка 2------------------------------------------------		 
		 
line	[x_qi2 + 1*h1*cos(a2), y_qi2 - 1*h1*sin(a2), x_qi2 + h1*sin(a2) + 1*h1*cos(a2), y_qi2 + h1*cos(a2) - 1*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 2*h1*cos(a2), y_qi2 - 2*h1*sin(a2), x_qi2 + h1*sin(a2) + 2*h1*cos(a2), y_qi2 + h1*cos(a2) - 2*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 3*h1*cos(a2), y_qi2 - 3*h1*sin(a2), x_qi2 + h1*sin(a2) + 3*h1*cos(a2), y_qi2 + h1*cos(a2) - 3*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 4*h1*cos(a2), y_qi2 - 4*h1*sin(a2), x_qi2 + h1*sin(a2) + 4*h1*cos(a2), y_qi2 + h1*cos(a2) - 4*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 5*h1*cos(a2), y_qi2 - 5*h1*sin(a2), x_qi2 + h1*sin(a2) + 5*h1*cos(a2), y_qi2 + h1*cos(a2) - 5*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 6*h1*cos(a2), y_qi2 - 6*h1*sin(a2), x_qi2 + h1*sin(a2) + 6*h1*cos(a2), y_qi2 + h1*cos(a2) - 6*h1*sin(a2), <0 0 0>]
line	[x_qi2 + 7*h1*cos(a2), y_qi2 - 7*h1*sin(a2), x_qi2 + h1*sin(a2) + 7*h1*cos(a2), y_qi2 + h1*cos(a2) - 7*h1*sin(a2), <0 0 0>]
//----------------------число в очереди - вход станок 2------------------------------------------------		 
text    [ x_qi2 - 10, y_qi2 - 25, 90, 25, <127 127 127>, <100 255 0>, QSTANOK2.длина_очереди ]




//----------------------Станок 11------------------------------------
triang	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 + 1.5*l_st11*sin(a_st11)- h_st11*cos(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 				
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11),
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 							
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 + 1.5*l_st11*sin(a_st11)- h_st11*cos(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>]

line	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11),
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 1.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 	
		 y_st11 + 1.5*l_st11*sin(a_st11) - h_st11*cos(a_st11), 
		 <0  0 0>]

//----------------------Станок 12------------------------------------
triang	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 + 0.5*l_st11*sin(a_st11)- h_st11*cos(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 				
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11),
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 							
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 + 0.5*l_st11*sin(a_st11)- h_st11*cos(a_st11),  
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>]

line	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11),
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11),
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 x_st11 - 0.5*l_st11*cos(a_st11) - h_st11*sin(a_st11), 	
		 y_st11 + 0.5*l_st11*sin(a_st11) - h_st11*cos(a_st11), 
		 <0  0 0>]
//----------------------Станок 13------------------------------------
triang	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 - 0.51*l_st11*sin(a_st11)- h_st11*cos(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 				
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11),
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11), 							
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 <210 210 210>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11), 			  		
		 y_st11 - 0.51*l_st11*sin(a_st11)- h_st11*cos(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11),
		 <210 210 210>]

line	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11),
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11),
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11), 
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + l_st11*cos(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) - l_st11*sin(a_st11) + h_st11*cos(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11),
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11) + h_st11*sin(a_st11), 
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11) + h_st11*cos(a_st11), 
		 x_st11 + 0.51*l_st11*cos(a_st11) - h_st11*sin(a_st11), 	
		 y_st11 - 0.51*l_st11*sin(a_st11) - h_st11*cos(a_st11), 
		 <0  0 0>]

//----------------------Станок 2------------------------------------
triang	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2), 			  		
		 y_st2 + 0.51*l_st2*sin(a_st2)- h_st2*cos(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2) + h_st2*cos(a_st2),
		 <210 210 210>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2) + h_st2*cos(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + h_st2*sin(a_st2), 				
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) + h_st2*cos(a_st2),
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2), 							
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2),
		 <210 210 210>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2), 			  		
		 y_st2 + 0.51*l_st2*sin(a_st2)- h_st2*cos(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2) + h_st2*cos(a_st2),
		 <210 210 210>]
		 
line	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2),
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2), 
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2),
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2), 
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2) + h_st2*cos(a_st2), 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + l_st2*cos(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) - l_st2*sin(a_st2) + h_st2*cos(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + h_st2*sin(a_st2),
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) + h_st2*cos(a_st2), 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2) + h_st2*sin(a_st2), 
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2) + h_st2*cos(a_st2), 
		 x_st2 - 0.51*l_st2*cos(a_st2) - h_st2*sin(a_st2), 	
		 y_st2 + 0.51*l_st2*sin(a_st2) - h_st2*cos(a_st2), 
		 <0  0 0>]

/*
//-------------------Очередь к первому станку---------------------------------------

rect	[x1, y1, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QSTANOK1.длина_очереди <= 10
rect	[x1 + hag*10 - QSTANOK1.длина_очереди*hag - 1, y1, QSTANOK1.длина_очереди*hag, h, <127 0 127>, <100 255 0>]
show_if QSTANOK1.длина_очереди > 10
rect	[x1, y1, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x1 + hag*10 - hag, y1 + 1, x1 + hag*10 - hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 2*hag, y1 + 1, x1 + hag*10 - 2*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 3*hag, y1 + 1, x1 + hag*10 - 3*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 4*hag, y1 + 1, x1 + hag*10 - 4*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 5*hag, y1 + 1, x1 + hag*10 - 5*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 6*hag, y1 + 1, x1 + hag*10 - 6*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 7*hag, y1 + 1, x1 + hag*10 - 7*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 8*hag, y1 + 1, x1 + hag*10 - 8*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 9*hag, y1 + 1, x1 + hag*10 - 9*hag, y1 + h - 1, <0  0 127>]

text    [ x1 - 20, y1 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK1.длина_очереди ]

*/








/*
//-------------------Очередь ко второму станку---------------------------------------

rect	[x2, y2, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QSTANOK2.длина_очереди <= 10
rect	[x2 + hag*10 - QSTANOK2.длина_очереди*hag - 1, y2, QSTANOK2.длина_очереди*hag, h, <127 0 127>, <100 255 0>]
show_if QSTANOK2.длина_очереди > 10
rect	[x2, y2, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x2 + hag*10 - hag, y2 + 1, x2 + hag*10 - hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 2*hag, y2 + 1, x2 + hag*10 - 2*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 3*hag, y2 + 1, x2 + hag*10 - 3*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 4*hag, y2 + 1, x2 + hag*10 - 4*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 5*hag, y2 + 1, x2 + hag*10 - 5*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 6*hag, y2 + 1, x2 + hag*10 - 6*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 7*hag, y2 + 1, x2 + hag*10 - 7*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 8*hag, y2 + 1, x2 + hag*10 - 8*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 9*hag, y2 + 1, x2 + hag*10 - 9*hag, y2 + h - 1, <0  0 127>]

text    [ x2- 20, y2 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK2.длина_очереди ]

//-------------------Очередь ко роботу на входном накопителе---------------------------------------

rect	[x3, y3, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QROBOT1.длина_очереди <= 10
rect	[x3 + hag*10 - QROBOT1.длина_очереди*hag - 1, y3, QROBOT1.длина_очереди*hag, h, <127 0 127>, <100 255 0>]
show_if QROBOT1.длина_очереди > 10
rect	[x3, y3, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x3 + hag*10 - hag, y3 + 1, x3 + hag*10 - hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 2*hag, y3 + 1, x3 + hag*10 - 2*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 3*hag, y3 + 1, x3 + hag*10 - 3*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 4*hag, y3 + 1, x3 + hag*10 - 4*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 5*hag, y3 + 1, x3 + hag*10 - 5*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 6*hag, y3 + 1, x3 + hag*10 - 6*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 7*hag, y3 + 1, x3 + hag*10 - 7*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 8*hag, y3 + 1, x3 + hag*10 - 8*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 9*hag, y3 + 1, x3 + hag*10 - 9*hag, y3 + h - 1, <0  0 127>]

text    [ x3- 20, y3 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT1.длина_очереди ]


//-------------------Очередь к роботу на перемещение ко второму---------------------------------------

rect	[x4, y4, hag*10, h2, <127 127 127>, <100 255 0> ]
show_if QROBOT2.длина_очереди <= 10
rect	[x4 + hag*10 - QROBOT2.длина_очереди*hag - 1, y4, QROBOT2.длина_очереди*hag, h2, <127 0 127>, <100 255 0>]
show_if QROBOT2.длина_очереди > 10
rect	[x4, y4, hag*10, h2, <127 0 127>, <100 255 0>]

show
line	[x4 + hag*10 - hag, y4 + 1, x4 + hag*10 - hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 2*hag, y4 + 1, x4 + hag*10 - 2*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 3*hag, y4 + 1, x4 + hag*10 - 3*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 4*hag, y4 + 1, x4 + hag*10 - 4*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 5*hag, y4 + 1, x4 + hag*10 - 5*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 6*hag, y4 + 1, x4 + hag*10 - 6*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 7*hag, y4 + 1, x4 + hag*10 - 7*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 8*hag, y4 + 1, x4 + hag*10 - 8*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 9*hag, y4 + 1, x4 + hag*10 - 9*hag, y4 + h2 - 1, <0  0 127>]

text    [ x4- 20, y4 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT2.длина_очереди ]


//-------------------Очередь к роботу на разгрузку второго, и перемещение в накопитель---------------------------------------

rect	[x5, y5, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QROBOT3.длина_очереди <= 10
rect	[x5 + hag*10 - QROBOT3.длина_очереди*hag - 1, y5, QROBOT3.длина_очереди*hag, h, <127 0 127>, <100 255 0>]
show_if QROBOT3.длина_очереди > 10
rect	[x5, y5, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x5 + hag*10 - hag, y5 + 1, x5 + hag*10 - hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 2*hag, y5 + 1, x5 + hag*10 - 2*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 3*hag, y5 + 1, x5 + hag*10 - 3*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 4*hag, y5 + 1, x5 + hag*10 - 4*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 5*hag, y5 + 1, x5 + hag*10 - 5*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 6*hag, y5 + 1, x5 + hag*10 - 6*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 7*hag, y5 + 1, x5 + hag*10 - 7*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 8*hag, y5 + 1, x5 + hag*10 - 8*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 9*hag, y5 + 1, x5 + hag*10 - 9*hag, y5 + h - 1, <0  0 127>]

text    [ x5- 20, y5 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT3.длина_очереди ]*/
/*
rect	[100, 100, 100, 20, <127 127 127>, <100 255 0> ]

Show_if QROBOT.длина_очереди = 1
ellipse [ 180, 100, 20, 20,  <127 0 127>, <100 255 0> ]
Show_if QROBOT.длина_очереди = 2
ellipse [ 180, 100, 20, 20,  <127 0 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 0 127>, <100 255 0> ]
Show_if QROBOT.длина_очереди = 3
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
Show_if QROBOT.длина_очереди = 4
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 120, 100, 20, 20,  <127 127 127>, <100 255 0> ]
Show_if QROBOT.длина_очереди > 5
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 120, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 100, 100, 20, 20,  <127 127 127>, <100 255 0> ]
*/

//------------------первая группа станков-----------------------------------
//active Операция_Поломка [x_st11, y_st11, gx_st11, gy_st11]
//active Операция_Починка [x_st11, y_st11, gx_st11, gy_st11]

/*
active Операция_Поломка [500, 300, 80, 20]
rect	[500, 300, 80, 20, <0 127 127>, <200 255 100> ]
text    [ 510, 300, 80, 20, <0 127 127>, <100 255 0>, 'сломать' ]

active Операция_Починка [500, 350, 80, 20]
rect	[500, 350, 80, 20, <255 255 127>, <200 255 100> ]
text    [ 510, 350, 80, 20, <255 255 127>, <100 255 0>, 'починить' ]
*/

/*Show_if ST11.Состояние = Свободен

rect	[x_st11, y_st11, gx_st11, gy_st11, <255 255 127>, <200 255 100> ]

Show_if ST11.Состояние = Занят

rect	[x_st11, y_st11, gx_st11, gy_st11, <255 127 127>, <200 255 100> ]

Show_if ST11.Состояние = Сломан

rect	[x_st11, y_st11, gx_st11, gy_st11, <0 127 127>, <0 255 100> ]
line	[x_st11 - 5, y_st11 - 5,x_st11 + gx_st11 + 5, y_st11 + gy_st11 + 5, <0  0 127>]
line	[x_st11 - 5, y_st11 + gy_st11 + 5,x_st11 + gx_st11 + 5, y_st11 - 5, <0  0 127>]
//---------------------------------------------------------------------------

Show_if ST12.Состояние = Свободен

rect	[x_st12, y_st12, gx_st12, gy_st12, <255 255 127>, <200 255 100> ]

Show_if ST12.Состояние = Занят

rect	[x_st12, y_st12, gx_st12, gy_st12, <255 127 127>, <200 255 100> ]

//---------------------------------------------------------------------------

Show_if ST13.Состояние = Свободен

rect	[x_st13, y_st13, gx_st13, gy_st13, <255 255 127>, <200 255 100> ]

Show_if ST13.Состояние = Занят

rect	[x_st13, y_st13, gx_st13, gy_st13, <255 127 127>, <200 255 100> ]
//------------------второй станок-----------------------------------

Show_if STANOK2.Состояние = Свободен

rect	[x_st2, y_st2, gx_st2, gy_st2, <255 255 127>, <200 255 100> ]

Show_if STANOK2.Состояние = Занят

rect	[x_st2, y_st2, gx_st2, gy_st2, <255 127 127>, <200 255 100> ]

Show
text    [ 600, 97, 90, 25, <127 127 127>, <100 255 0>, QEXIT.длина_очереди ]
*/
/*
text   [10,70 ,350 ,25 , <127 127 127>, <0 0 0>, 'Станок 1 в состоянии:' ]
text   [350,70 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_1.состояние]
text   [500,70 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_1.положение]
text   [10,85 ,350 ,25 , <127 127 127>, <0 0 0>, 'Станок 2 в состоянии:' ]
text   [350,85 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_2.состояние]
text   [500,85 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_2.положение]
text   [10,100 ,350 ,25 , <127 127 127>, <0 0 0>, 'Станок 3 в состоянии:' ]
text   [350,100 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_3.состояние]
text   [500,100 ,350 ,25 , <127 127 127>, <0 0 0>, Станок_3.положение]

text   [10,120 ,350 ,25 , <127 127 127>, <0 0 0>, 'Тележка_1 в состоянии:' ]
text   [350,120 ,350 ,25 , <127 127 127>, <0 0 0>, Тележка_1.состояние]
text   [500,120 ,350 ,25 , <127 127 127>, <0 0 0>, Тележка_1.положение]

text   [10,135 ,350 ,25 , <127 127 127>, <0 0 0>, 'Тележка_2 в состоянии:' ]
text   [350,135 ,350 ,25 , <127 127 127>, <0 0 0>, Тележка_2.состояние]
text   [500,135 ,350 ,25 , <127 127 127>, <0 0 0>, Тележка_2.положение]

text   [10,150 ,350 ,25 , <127 127 127>, <0 0 0>, 'Робот_1 в состоянии:' ]
text   [350,150 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_1.состояние]
text   [500,150 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_1.положение]

text   [10,165 ,350 ,25 , <127 127 127>, <0 0 0>, 'Робот_2 в состоянии:' ]
text   [350,165 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_2.состояние]
text   [500,165 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_2.положение]

text   [10,180 ,350 ,25 , <127 127 127>, <0 0 0>, 'Робот_3 в состоянии:' ]
text   [350,180 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_3.состояние]
text   [500,180 ,350 ,25 , <127 127 127>, <0 0 0>, Робот_3.положение]

text   [10,200 ,350 ,25 , <127 127 127>, <0 0 0>, 'Кол-во детале в 1-м накопителе:' ]
text   [350,200 ,350 ,25 , <127 127 127>, <0 0 0>, Накопитель_1.текущее_количество]
text   [10,215 ,350 ,25 , <127 127 127>, <0 0 0>, 'Кол-во детале в 2-м накопителе:' ]
text   [350,215 ,350 ,25 , <127 127 127>, <0 0 0>, Накопитель_2.текущее_количество]

text   [10,300 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 1:' ]
text   [250,300 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_1.положение]
text   [350,300 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_1.состояние]

text   [10,315 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 2:' ]
text   [250,315 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_2.положение]
text   [350,315 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_2.состояние]

text   [10,330 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 3:' ]
text   [250,330 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_3.положение]
text   [350,330 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_3.состояние]

text   [10,345 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 4:' ]
text   [250,345 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_4.положение]
text   [350,345 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_4.состояние]

text   [10,360 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 5:' ]
text   [250,360 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_5.положение]
text   [350,360 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_5.состояние]

text   [10,375 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 6:' ]
text   [250,375 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_6.положение]
text   [350,375 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_6.состояние]

text   [10,390 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 7:' ]
text   [250,390 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_7.положение]
text   [350,390 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_7.состояние]

text   [10,405 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 8:' ]
text   [250,405 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_8.положение]
text   [350,405 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_8.состояние]

text   [10,420 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 9:' ]
text   [250,420 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_9.положение]
text   [350,420 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_9.состояние]

text   [10,435 ,350 ,25 , <127 127 127>, <0 0 0>, 'Деталь 10:' ]
text   [250,435 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_10.положение]
text   [350,435 ,350 ,25 , <127 127 127>, <0 0 0>, Деталь_10.состояние]
*/

$End
