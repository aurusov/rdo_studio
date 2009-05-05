$Frame fram_1
$Back_picture = <255 255 255> 1080 680
Show

text    [10, 5,  50, 25, <127 127 127>, <100 255 0>, 'Время:' ]
text    [60, 5, 150, 25, <127 127 127>, <100 255 0>, Time_now ]

rect [100, 430, 40, 20, <255 255 0>, <0 0 0>]
text [150, 430, 150, 20, <255 255 255>, <0 0 0>, 'Движение заготовки' ]		 

rect [100, 460, 40, 20, <210 210 210>, <0 0 0>]
text [150, 460, 150, 20, <255 255 255>, <0 0 0>, 'Ресурс свободен' ]		

rect [100, 490, 40, 20, <0 255 0>, <0 0 0>]
text [150, 490, 150, 20, <255 255 255>, <0 0 0>, 'Ресурс занят' ]	

rect [100, 520, 40, 20, <255 0 0>, <0 0 0>]
text [150, 520, 150, 20, <255 255 255>, <0 0 0>, 'Ресурс сломан' ]


ellipse [x_mid - l_rob/2, y_mid - l_rob/2, l_rob, l_rob, <255 255 255>, <0 0 0>]
//----------------------Робот - поломка------------------------------------
show		 
active  Операция_Поломка_Робота1[x_mid - 2 - 40, y_mid - h_rob, 80, h_rob + 20 ]
rect	[x_mid - 2 - 40, y_mid - h_rob + 20, 80, h_rob, <0 0 0>]
//--------------------Робот----------------------------------------------------------
show_if ROBOT.Состояние = Свободен and ROBOT.Положение = state2
rect	[x_mid - 3, y_mid - h_rob, 6, h_rob + 2, <210 210 210>, <0 0 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 - 20, y_mid - h_rob - 20, <0 0 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 + 20, y_mid - h_rob - 20, <0 0 0> ]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <210 210 210>, <0 0 0>] 
 
show_if ROBOT.Состояние = Занят and ROBOT.Положение = state2
rect	[x_mid - 3, y_mid - h_rob, 6, h_rob + 2, <0 255 0>, <0 0 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 - 20, y_mid - h_rob - 20, <0 255 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 + 20, y_mid - h_rob - 20, <0 255 0> ]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <0 255 0>, <0 0 0>]  
ellipse [x_mid - r_rob/2, y_mid - h_rob - 25, 18, 18, <255 255 0>, <0 0 0>] 

show_if ROBOT.Состояние = Сломан and ROBOT.Положение = state2
rect	[x_mid - 3, y_mid - h_rob, 6, h_rob + 2, <255 0 0>, <0 0 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 - 20, y_mid - h_rob - 20, <255 0 0> ]
line	[x_mid, y_mid - h_rob, x_mid - 2 + 20, y_mid - h_rob - 20, <255 0 0> ]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <255 0 0>, <0 0 0>]  

//-----------------------Роббот 1--------------------------------
show_if ROBOT.Состояние = Свободен and ROBOT.Положение = state1
triang	[xr1, 						 yr1, 
		 xr1 + lr1*cosr1, 			 yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <210 210 210>, <0 0 0>]

triang	[xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1, 
		 xr1 + hr1*sinr1, 			 yr1 + hr1*cosr1,
		 xr1, 						 yr1,
		  <210 210 210>, <0 0 0>]
line	[xr1, 											  yr1, 
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <210 210 210>]
		 
line	[xr1 + lr1*cosr1, 			 		   yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + k*sin(0.785 - ar1), yr1 - lr1*sinr1 - k*cos(0.785 - ar1),
		 <0 0 0>]

line	[xr1 + lr1*cosr1 + hr1*sinr1,	yr1 - lr1*sinr1 + hr1*cosr1,
		 xr1 + lr1*cosr1 + hr1*sinr1 + k*sin(0.785 + ar1), yr1 - lr1*sinr1 + hr1*cosr1 + k*cos(0.785 + ar1),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <210 210 210>, <0 0 0>] 

show_if ROBOT.Состояние = Занят and ROBOT.Положение = state1
triang	[xr1, 						 yr1, 
		 xr1 + lr1*cosr1, 			 yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <0 255 0>, <0 0 0>]

triang	[xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1, 
		 xr1 + hr1*sinr1, 			 yr1 + hr1*cosr1,
		 xr1, 						 yr1,
		<0 255 0>, <0 0 0>]
line	[xr1, 											  yr1, 
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <0 255 0>]
		 
line	[xr1 + lr1*cosr1, 			 		   yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + k*sin(0.785 - ar1), yr1 - lr1*sinr1 - k*cos(0.785 - ar1),
		 <0 0 0>]

line	[xr1 + lr1*cosr1 + hr1*sinr1,	yr1 - lr1*sinr1 + hr1*cosr1,
		 xr1 + lr1*cosr1 + hr1*sinr1 + k*sin(0.785 + ar1), yr1 - lr1*sinr1 + hr1*cosr1 + k*cos(0.785 + ar1),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <0 255 0>, <0 0 0>]
ellipse [xr1 + lr1*cosr1 - 20, yr1 - lr1*sinr1 - 17, 18, 18, <255 255 0>, <0 0 0>] 

show_if ROBOT.Состояние = Сломан and ROBOT.Положение = state1
triang	[xr1, 						 yr1, 
		 xr1 + lr1*cosr1, 			 yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <255 0 0>, <0 0 0>]

triang	[xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1, 
		 xr1 + hr1*sinr1, 			 yr1 + hr1*cosr1,
		 xr1, 						 yr1,
		<255 0 0>, <0 0 0>]
line	[xr1, 											  yr1, 
		 xr1 + lr1*cosr1 + hr1*sinr1,yr1 - lr1*sinr1 + hr1*cosr1,
		 <255 0 0>]
		 
line	[xr1 + lr1*cosr1, 			 		   yr1 - lr1*sinr1,
		 xr1 + lr1*cosr1 + k*sin(0.785 - ar1), yr1 - lr1*sinr1 - k*cos(0.785 - ar1),
		 <0 0 0>]

line	[xr1 + lr1*cosr1 + hr1*sinr1,	yr1 - lr1*sinr1 + hr1*cosr1,
		 xr1 + lr1*cosr1 + hr1*sinr1 + k*sin(0.785 + ar1), yr1 - lr1*sinr1 + hr1*cosr1 + k*cos(0.785 + ar1),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <255 0 0>, <0 0 0>]


//-----------------------Роббот 2--------------------------------
show_if ROBOT.Состояние = Свободен and ROBOT.Положение = state3
triang	[xr2, 						 yr2, 
		 xr2 + lr2*cosr2, 			 yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <210 210 210>, <0 0 0>]

triang	[xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2, 
		 xr2 + hr2*sinr2, 			 yr2 + hr2*cosr2,
		 xr2, 						 yr2,
		  <210 210 210>, <0 0 0>]
line	[xr2, 											  yr2, 
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <210 210 210>]
		 
line	[xr2 + lr2*cosr2, 			 		   yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + k*sin(0.785 - ar2), yr2 - lr2*sinr2 - k*cos(0.785 - ar2),
		 <0 0 0>]

line	[xr2 + lr2*cosr2 + hr2*sinr2,	yr2 - lr2*sinr2 + hr2*cosr2,
		 xr2 + lr2*cosr2 + hr2*sinr2 + k*sin(0.785 + ar2), yr2 - lr2*sinr2 + hr2*cosr2 + k*cos(0.785 + ar2),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <210 210 210>, <0 0 0>] 

show_if ROBOT.Состояние = Занят and ROBOT.Положение = state3
triang	[xr2, 						 yr2, 
		 xr2 + lr2*cosr2, 			 yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <0 255 0>, <0 0 0>]

triang	[xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2, 
		 xr2 + hr2*sinr2, 			 yr2 + hr2*cosr2,
		 xr2, 						 yr2,
		<0 255 0>, <0 0 0>]
line	[xr2, 											  yr2, 
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <0 255 0>]
		 
line	[xr2 + lr2*cosr2, 			 		   yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + k*sin(0.785 - ar2), yr2 - lr2*sinr2 - k*cos(0.785 - ar2),
		 <0 0 0>]

line	[xr2 + lr2*cosr2 + hr2*sinr2,	yr2 - lr2*sinr2 + hr2*cosr2,
		 xr2 + lr2*cosr2 + hr2*sinr2 + k*sin(0.785 + ar2), yr2 - lr2*sinr2 + hr2*cosr2 + k*cos(0.785 + ar2),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <0 255 0>, <0 0 0>]
ellipse [xr2 + lr2*cosr2 + 4, yr2 - lr2*sinr2 - 12, 18, 18, <255 255 0>, <0 0 0>] 

show_if ROBOT.Состояние = Сломан and ROBOT.Положение = state3
triang	[xr2, 						 yr2, 
		 xr2 + lr2*cosr2, 			 yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <255 0 0>, <0 0 0>]

triang	[xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2, 
		 xr2 + hr2*sinr2, 			 yr2 + hr2*cosr2,
		 xr2, 						 yr2,
		<255 0 0>, <0 0 0>]
line	[xr2, 											  yr2, 
		 xr2 + lr2*cosr2 + hr2*sinr2,yr2 - lr2*sinr2 + hr2*cosr2,
		 <255 0 0>]
		 
line	[xr2 + lr2*cosr2, 			 		   yr2 - lr2*sinr2,
		 xr2 + lr2*cosr2 + k*sin(0.785 - ar2), yr2 - lr2*sinr2 - k*cos(0.785 - ar2),
		 <0 0 0>]

line	[xr2 + lr2*cosr2 + hr2*sinr2,	yr2 - lr2*sinr2 + hr2*cosr2,
		 xr2 + lr2*cosr2 + hr2*sinr2 + k*sin(0.785 + ar2), yr2 - lr2*sinr2 + hr2*cosr2 + k*cos(0.785 + ar2),
		 <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <255 0 0>, <0 0 0>]





show
//------------------------стрелка 1---------------------------------------
line	[xs1, ys1, xs1 + ls1*cas1, ys1 - ls1*sas1, <0 0 0>]
triang	[xs1 + (ls1 + bs1)*cas1, ys1 - (ls1 + bs1)*sas1,
		 xs1 + ls1*cas1 + 0.5*cs1*sas1, ys1 - ls1*sas1 + 0.5*cs1*cas1,
		 xs1 + ls1*cas1 - 0.5*cs1*sas1, ys1 - ls1*sas1 - 0.5*cs1*cas1,
		 blue, <0 0 0>]

line	[xs2, ys2, xs2 + ls1*cas2, ys2 - ls1*sas2, <0 0 0>]
triang	[xs2 + (ls1 + bs1)*cas2, ys2 - (ls1 + bs1)*sas2,
		 xs2 + ls1*cas2 + 0.5*cs1*sas2, ys2 - ls1*sas2 + 0.5*cs1*cas2,
		 xs2 + ls1*cas2 - 0.5*cs1*sas2, ys2 - ls1*sas2 - 0.5*cs1*cas2,
		 blue, <0 0 0>]

line	[xs3, ys3, xs3 + ls1*cas3, ys3 - ls1*sas3, <0 0 0>]
triang	[xs3 + (ls1 + bs1)*cas3, ys3 - (ls1 + bs1)*sas3,
		 xs3 + ls1*cas3 + 0.5*cs1*sas3, ys3 - ls1*sas3 + 0.5*cs1*cas3,
		 xs3 + ls1*cas3 - 0.5*cs1*sas3, ys3 - ls1*sas3 - 0.5*cs1*cas3,
		 blue, <0 0 0>]

line	[xs4, ys4, xs4 + ls1*cas4, ys4 - ls1*sas4, <0 0 0>]
triang	[xs4 + (ls1 + bs1)*cas4, ys4 - (ls1 + bs1)*sas4,
		 xs4 + ls1*cas4 + 0.5*cs1*sas4, ys4 - ls1*sas4 + 0.5*cs1*cas4,
		 xs4 + ls1*cas4 - 0.5*cs1*sas4, ys4 - ls1*sas4 - 0.5*cs1*cas4,
		 blue, <0 0 0>]

line	[xs5, ys5, xs5 + ls1*cas5, ys5 - ls1*sas5, <0 0 0>]
triang	[xs5 + (ls1 + bs1)*cas5, ys5 - (ls1 + bs1)*sas5,
		 xs5 + ls1*cas5 + 0.5*cs1*sas5, ys5 - ls1*sas5 + 0.5*cs1*cas5,
		 xs5 + ls1*cas5 - 0.5*cs1*sas5, ys5 - ls1*sas5 - 0.5*cs1*cas5,
		 blue, <0 0 0>]

show		 
line	[xs6, ys6, xs6 + ls6*cas6, ys6 - ls6*sas6, <0 0 0>]
triang	[xs6 + (ls6 + bs1)*cas6, ys6 - (ls6 + bs1)*sas6,
		 xs6 + ls6*cas6 + 0.5*cs1*sas6, ys6 - ls6*sas6 + 0.5*cs1*cas6,
		 xs6 + ls6*cas6 - 0.5*cs1*sas6, ys6 - ls6*sas6 - 0.5*cs1*cas6,
		 blue, <0 0 0>]

line	[xs7, ys7, xs7 + ls7*cas7, ys7 - ls7*sas7, <0 0 0>]
triang	[xs7 + (ls7 + bs1)*cas7, ys7 - (ls7 + bs1)*sas7,
		 xs7 + ls7*cas7 + 0.5*cs1*sas7, ys7 - ls7*sas7 + 0.5*cs1*cas7,
		 xs7 + ls7*cas7 - 0.5*cs1*sas7, ys7 - ls7*sas7 - 0.5*cs1*cas7,
		 blue, <0 0 0>]

line	[xs8, ys8, xs8 + ls6*cas8, ys8 - ls6*sas8, <0 0 0>]
triang	[xs8 + (ls6 + bs1)*cas8, ys8 - (ls6 + bs1)*sas8,
		 xs8 + ls6*cas8 + 0.5*cs1*sas8, ys8 - ls6*sas8 + 0.5*cs1*cas8,
		 xs8 + ls6*cas8 - 0.5*cs1*sas8, ys8 - ls6*sas8 - 0.5*cs1*cas8,
		 blue, <0 0 0>]

/*
show_if	ROBOT.Положение = state1 
line	[xs6, ys6, xs6 + ls6*cas6, ys6 - ls6*sas6, <0 255 0>]
triang	[xs6 + (ls6 + bs1)*cas6, ys6 - (ls6 + bs1)*sas6,
		 xs6 + ls6*cas6 + 0.5*cs1*sas6, ys6 - ls6*sas6 + 0.5*cs1*cas6,
		 xs6 + ls6*cas6 - 0.5*cs1*sas6, ys6 - ls6*sas6 - 0.5*cs1*cas6,
		 <0 255 0>, <0 255 0>]
show_if	ROBOT.Положение = state2
line	[xs7, ys7, xs7 + ls7*cas7, ys7 - ls7*sas7, <0 255 0>]
triang	[xs7 + (ls7 + bs1)*cas7, ys7 - (ls7 + bs1)*sas7,
		 xs7 + ls7*cas7 + 0.5*cs1*sas7, ys7 - ls7*sas7 + 0.5*cs1*cas7,
		 xs7 + ls7*cas7 - 0.5*cs1*sas7, ys7 - ls7*sas7 - 0.5*cs1*cas7,
		 <0 255 0>, <0 255 0>]
show_if	ROBOT.Положение = state3
line	[xs8, ys8, xs8 + ls6*cas8, ys8 - ls6*sas8, <0 255 0>]
triang	[xs8 + (ls6 + bs1)*cas8, ys8 - (ls6 + bs1)*sas8,
		 xs8 + ls6*cas8 + 0.5*cs1*sas8, ys8 - ls6*sas8 + 0.5*cs1*cas8,
		 xs8 + ls6*cas8 - 0.5*cs1*sas8, ys8 - ls6*sas8 - 0.5*cs1*cas8,
		 <0 255 0>, <0 255 0>]		 

*/


show
//----------------------очередь накопитель - вход------------------------------------
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cosai, 			y_qi - l1*sinai,
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <127 127 127>, <0 0 127>]

triang	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + h1*sinai, 			y_qi + h1*cosai,
		 x_qi, 						 	y_qi,
		  <127 127 127>, <127 127 127>]
		  

//----------------------очередь накопитель - по длине очереди------------------------------------
show_if QIN.длина_очереди <= 8
triang	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.длина_очереди*cosai, 			   y_qi - h1*QIN.длина_очереди*sinai,
		 x_qi + h1*QIN.длина_очереди*cosai + h1*sinai,y_qi - h1*QIN.длина_очереди*sinai + h1*cosai,
		 <255 255 0>, <0 0 0>]

triang	[x_qi + h1*QIN.длина_очереди*cosai + h1*sinai,y_qi - h1*QIN.длина_очереди*sinai + h1*cosai, 
		 x_qi + h1*sinai, 								   y_qi + h1*cosai,
		 x_qi, 						 						   y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.длина_очереди*cosai + h1*sinai,y_qi - h1*QIN.длина_очереди*sinai + h1*cosai,
		 <255 255 0>]
		 
show_if QIN.длина_очереди > 8
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cosai, 			y_qi - l1*sinai,
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <255 255 0>, <0 0 0>]

triang	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + l1*sinai, 			y_qi + h1*cosai,
		 x_qi, 						 	y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <255 255 0>]

show
//----------------------очередь накопитель - оконтовка------------------------------------

line	[x_qi, 						 	y_qi, 
		 x_qi + l1*cosai,			 	y_qi - l1*sinai, 
		 <0  0 0>]
line	[x_qi + l1*cosai,			 	y_qi - l1*sinai, 
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 <0  0 0>]
line	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + h1*sinai, 			y_qi + h1*cosai, 
		 <0  0 0>]
line	[x_qi+ h1*sinai, 				y_qi + h1*cosai, 
		 x_qi, 					     	y_qi, 
		 <0  0 0>]


//----------------------линии очереди накопитель - вход------------------------------------------------		 
		 
line	[x_qi + 1*h1*cosai, y_qi - 1*h1*sinai, x_qi + h1*sinai + 1*h1*cosai, y_qi + h1*cosai - 1*h1*sinai, <0 0 0>]
line	[x_qi + 2*h1*cosai, y_qi - 2*h1*sinai, x_qi + h1*sinai + 2*h1*cosai, y_qi + h1*cosai - 2*h1*sinai, <0 0 0>]
line	[x_qi + 3*h1*cosai, y_qi - 3*h1*sinai, x_qi + h1*sinai + 3*h1*cosai, y_qi + h1*cosai - 3*h1*sinai, <0 0 0>]
line	[x_qi + 4*h1*cosai, y_qi - 4*h1*sinai, x_qi + h1*sinai + 4*h1*cosai, y_qi + h1*cosai - 4*h1*sinai, <0 0 0>]
line	[x_qi + 5*h1*cosai, y_qi - 5*h1*sinai, x_qi + h1*sinai + 5*h1*cosai, y_qi + h1*cosai - 5*h1*sinai, <0 0 0>]
line	[x_qi + 6*h1*cosai, y_qi - 6*h1*sinai, x_qi + h1*sinai + 6*h1*cosai, y_qi + h1*cosai - 6*h1*sinai, <0 0 0>]
line	[x_qi + 7*h1*cosai, y_qi - 7*h1*sinai, x_qi + h1*sinai + 7*h1*cosai, y_qi + h1*cosai - 7*h1*sinai, <0 0 0>]

//----------------------число в очереди накопитель - вход------------------------------------------------		 
text    [ x_qi - 10, y_qi, 90, 25, <127 127 127>, <100 255 0>, QIN.длина_очереди ]

//----------------------очередь накопитель - выход------------------------------------
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cosao, 			y_qo - l1*sinao,
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <127 127 127>, <127 127 127>]

triang	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + h1*sinao, 			y_qo + h1*cosao,
		 x_qo, 						 	y_qo,
		  <127 127 127>, <127 127 127>]

//----------------------очередь накопитель - по длине очереди------------------------------------
show_if QOUT.длина_очереди <= 8
triang	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.длина_очереди*cosao, 			   y_qo - h1*QOUT.длина_очереди*sinao,
		 x_qo + h1*QOUT.длина_очереди*cosao + h1*sinao,y_qo - h1*QOUT.длина_очереди*sinao + h1*cosao,
		 <255 255 0>, <0 0 0>]

triang	[x_qo + h1*QOUT.длина_очереди*cosao + h1*sinao,y_qo - h1*QOUT.длина_очереди*sinao + h1*cosao, 
		 x_qo + h1*sinao, 								   y_qo + h1*cosao,
		 x_qo, 						 						   y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.длина_очереди*cosao + h1*sinao,y_qo - h1*QOUT.длина_очереди*sinao + h1*cosao,
		 <255 255 0>]
		 
show_if QOUT.длина_очереди > 8
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cosao, 			y_qo - l1*sinao,
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <255 255 0>, <0 0 0>]

triang	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + l1*sinao, 			y_qo + h1*cosao,
		 x_qo, 						 	y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <255 255 0>]

show
//----------------------очередь накопитель - оконтовка------------------------------------
line	[x_qo, 						 	y_qo, 
		 x_qo + l1*cosao,			 	y_qo - l1*sinao, 
		 <0  0 0>]
line	[x_qo + l1*cosao,			 	y_qo - l1*sinao, 
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 <0  0 0>]
line	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + h1*sinao, 			y_qo + h1*cosao, 
		 <0  0 0>]
line	[x_qo+ h1*sinao, 				y_qo + h1*cosao, 
		 x_qo, 					     	y_qo, 
		 <0  0 0>]

//----------------------линии очереди накопитель - выход------------------------------------------------		 
		 
line	[x_qo + 1*h1*cosao, y_qo - 1*h1*sinao, x_qo + h1*sinao + 1*h1*cosao, y_qo + h1*cosao - 1*h1*sinao, <0 0 0>]
line	[x_qo + 2*h1*cosao, y_qo - 2*h1*sinao, x_qo + h1*sinao + 2*h1*cosao, y_qo + h1*cosao - 2*h1*sinao, <0 0 0>]
line	[x_qo + 3*h1*cosao, y_qo - 3*h1*sinao, x_qo + h1*sinao + 3*h1*cosao, y_qo + h1*cosao - 3*h1*sinao, <0 0 0>]
line	[x_qo + 4*h1*cosao, y_qo - 4*h1*sinao, x_qo + h1*sinao + 4*h1*cosao, y_qo + h1*cosao - 4*h1*sinao, <0 0 0>]
line	[x_qo + 5*h1*cosao, y_qo - 5*h1*sinao, x_qo + h1*sinao + 5*h1*cosao, y_qo + h1*cosao - 5*h1*sinao, <0 0 0>]
line	[x_qo + 6*h1*cosao, y_qo - 6*h1*sinao, x_qo + h1*sinao + 6*h1*cosao, y_qo + h1*cosao - 6*h1*sinao, <0 0 0>]
line	[x_qo + 7*h1*cosao, y_qo - 7*h1*sinao, x_qo + h1*sinao + 7*h1*cosao, y_qo + h1*cosao - 7*h1*sinao, <0 0 0>]
//----------------------число в очереди накопитель - выход------------------------------------------------		 
text    [ x_qo , y_qi, 90, 25, <127 127 127>, <100 255 0>, QOUT.длина_очереди ]




//----------------------очередь станка 1 - выход ------------------------------------
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cosa1, 			y_qo1 - l1*sina1,
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <127 127 127>, <127 127 127>]

triang	[x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 			y_qo1 + h1*cosa1,
		 x_qo1, 							y_qo1,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 1 - выход------------------------------------
show_if QEXITST1.длина_очереди <= 8
triang	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.длина_очереди*cosa1, 			   y_qo1 - h1*QEXITST1.длина_очереди*sina1,
		 x_qo1 + h1*QEXITST1.длина_очереди*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.длина_очереди*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + h1*QEXITST1.длина_очереди*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.длина_очереди*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 								   y_qo1 + h1*cosa1,
		 x_qo1, 						 						   y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.длина_очереди*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.длина_очереди*sina1 + h1*cosa1,
		 <255 255 0>]
		 
show_if QEXITST1.длина_очереди > 8
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cosa1, 			y_qo1 - l1*sina1,
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 			y_qo1 + h1*cosa1,
		 x_qo1, 						 	y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <255 255 0>]

show
//----------------------очередь станка1 выход - оконтовка------------------------------------


line	[x_qo1, 								y_qo1, 
		 x_qo1 + l1*cosa1,					y_qo1 - l1*sina1, 
		 <0  0 0>]
line	[x_qo1 + l1*cosa1,					y_qo1 - l1*sina1, 
		 x_qo1 + l1*cosa1 + h1*sina1, 	y_qo1 - l1*sina1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qo1 + l1*cosa1 + h1*sina1, 	y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 				y_qo1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qo1 + h1*sina1, 				y_qo1 + h1*cosa1, 
		 x_qo1, 								y_qo1, 
		 <0  0 0>]
//----------------------линии очереди - выход станка 1------------------------------------------------		 
		 
line	[x_qo1 + 1*h1*cosa1, y_qo1 - 1*h1*sina1, x_qo1 + h1*sina1 + 1*h1*cosa1, y_qo1 + h1*cosa1 - 1*h1*sina1, <0 0 0>]
line	[x_qo1 + 2*h1*cosa1, y_qo1 - 2*h1*sina1, x_qo1 + h1*sina1 + 2*h1*cosa1, y_qo1 + h1*cosa1 - 2*h1*sina1, <0 0 0>]
line	[x_qo1 + 3*h1*cosa1, y_qo1 - 3*h1*sina1, x_qo1 + h1*sina1 + 3*h1*cosa1, y_qo1 + h1*cosa1 - 3*h1*sina1, <0 0 0>]
line	[x_qo1 + 4*h1*cosa1, y_qo1 - 4*h1*sina1, x_qo1 + h1*sina1 + 4*h1*cosa1, y_qo1 + h1*cosa1 - 4*h1*sina1, <0 0 0>]
line	[x_qo1 + 5*h1*cosa1, y_qo1 - 5*h1*sina1, x_qo1 + h1*sina1 + 5*h1*cosa1, y_qo1 + h1*cosa1 - 5*h1*sina1, <0 0 0>]
line	[x_qo1 + 6*h1*cosa1, y_qo1 - 6*h1*sina1, x_qo1 + h1*sina1 + 6*h1*cosa1, y_qo1 + h1*cosa1 - 6*h1*sina1, <0 0 0>]
line	[x_qo1 + 7*h1*cosa1, y_qo1 - 7*h1*sina1, x_qo1 + h1*sina1 + 7*h1*cosa1, y_qo1 + h1*cosa1 - 7*h1*sina1, <0 0 0>]
//----------------------число в очереди - выход станок 1------------------------------------------------		 
text    [ x_qo1 + 10, y_qo1 - 30, 90, 25, <127 127 127>, <100 255 0>, QEXITST1.длина_очереди ]






//----------------------очередь станка 1 - вход------------------------------------
triang	[x_qi1, 						y_qi1, 
		 x_qi1 + l1*cosa1, 			y_qi1 - l1*sina1,
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <127 127 127>, <127 127 127>]

triang	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			y_qi1 + h1*cosa1,
		 x_qi1, 							y_qi1,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 1 - вход------------------------------------
show_if QSTANOK1.длина_очереди <= 8
triang	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.длина_очереди*cosa1, 			y_qi1 - h1*QSTANOK1.длина_очереди*sina1,
		 x_qi1 + h1*QSTANOK1.длина_очереди*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.длина_очереди*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + h1*QSTANOK1.длина_очереди*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.длина_очереди*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 								    y_qi1 + h1*cosa1,
		 x_qi1, 						 						y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.длина_очереди*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.длина_очереди*sina1 + h1*cosa1,
		 <255 255 0>]
		 
show_if QSTANOK1.длина_очереди > 8
triang	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cosa1, 			 y_qi1 - l1*sina1,
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			 y_qi1 + h1*cosa1,
		 x_qi1, 						 y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <255 255 0>]

show
//----------------------очередь станка1 вход - оконтовка------------------------------------

line	[x_qi1, 							y_qi1, 
		 x_qi1 + l1*cosa1,				y_qi1 - l1*sina1, 
		 <0  0 0>]
line	[x_qi1 + l1*cosa1,				y_qi1 - l1*sina1, 
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			y_qi1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qi1+ h1*sina1, 				y_qi1 + h1*cosa1, 
		 x_qi1, 							y_qi1, 
		 <0  0 0>]

//----------------------линии очереди - вход станка 1------------------------------------------------		 
		 
line	[x_qi1 + 1*h1*cosa1, y_qi1 - 1*h1*sina1, x_qi1 + h1*sina1 + 1*h1*cosa1, y_qi1 + h1*cosa1 - 1*h1*sina1, <0 0 0>]
line	[x_qi1 + 2*h1*cosa1, y_qi1 - 2*h1*sina1, x_qi1 + h1*sina1 + 2*h1*cosa1, y_qi1 + h1*cosa1 - 2*h1*sina1, <0 0 0>]
line	[x_qi1 + 3*h1*cosa1, y_qi1 - 3*h1*sina1, x_qi1 + h1*sina1 + 3*h1*cosa1, y_qi1 + h1*cosa1 - 3*h1*sina1, <0 0 0>]
line	[x_qi1 + 4*h1*cosa1, y_qi1 - 4*h1*sina1, x_qi1 + h1*sina1 + 4*h1*cosa1, y_qi1 + h1*cosa1 - 4*h1*sina1, <0 0 0>]
line	[x_qi1 + 5*h1*cosa1, y_qi1 - 5*h1*sina1, x_qi1 + h1*sina1 + 5*h1*cosa1, y_qi1 + h1*cosa1 - 5*h1*sina1, <0 0 0>]
line	[x_qi1 + 6*h1*cosa1, y_qi1 - 6*h1*sina1, x_qi1 + h1*sina1 + 6*h1*cosa1, y_qi1 + h1*cosa1 - 6*h1*sina1, <0 0 0>]
line	[x_qi1 + 7*h1*cosa1, y_qi1 - 7*h1*sina1, x_qi1 + h1*sina1 + 7*h1*cosa1, y_qi1 + h1*cosa1 - 7*h1*sina1, <0 0 0>]
//----------------------число в очереди - вход станок 1------------------------------------------------		 
text    [ x_qi1 - 19, y_qi1 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK1.длина_очереди ]

//----------------------очередь станка 2 - выход ------------------------------------
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2, 			 y_qo2 - l1*sina2,
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <127 127 127>, <127 127 127>]

triang	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2,
		 x_qo2, 						 y_qo2,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 2 - выход------------------------------------
show_if QEXITST2.длина_очереди <= 8
triang	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.длина_очереди*cosa2, 			y_qo2 - h1*QEXITST2.длина_очереди*sina2,
		 x_qo2 + h1*QEXITST2.длина_очереди*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.длина_очереди*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + h1*QEXITST2.длина_очереди*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.длина_очереди*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 								    y_qo2 + h1*cosa2,
		 x_qo2, 						 						y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.длина_очереди*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.длина_очереди*sina2 + h1*cosa2,
		 <255 255 0>]
		 
show_if QEXITST2.длина_очереди > 8
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2, 			 y_qo2 - l1*sina2,
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2,
		 x_qo2, 						 y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <255 255 0>]

show
//----------------------очередь станка 2 выход - оконтовка------------------------------------

line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2,			 y_qo2 - l1*sina2, 
		 <0  0 0>]
line	[x_qo2 + l1*cosa2,			 y_qo2 - l1*sina2, 
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2, 
		 x_qo2, 						 y_qo2, 
		 <0  0 0>]
//----------------------линии очереди - выход станка 2------------------------------------------------		 
		 
line	[x_qo2 + 1*h1*cosa2, y_qo2 - 1*h1*sina2, x_qo2 + h1*sina2 + 1*h1*cosa2, y_qo2 + h1*cosa2 - 1*h1*sina2, <0 0 0>]
line	[x_qo2 + 2*h1*cosa2, y_qo2 - 2*h1*sina2, x_qo2 + h1*sina2 + 2*h1*cosa2, y_qo2 + h1*cosa2 - 2*h1*sina2, <0 0 0>]
line	[x_qo2 + 3*h1*cosa2, y_qo2 - 3*h1*sina2, x_qo2 + h1*sina2 + 3*h1*cosa2, y_qo2 + h1*cosa2 - 3*h1*sina2, <0 0 0>]
line	[x_qo2 + 4*h1*cosa2, y_qo2 - 4*h1*sina2, x_qo2 + h1*sina2 + 4*h1*cosa2, y_qo2 + h1*cosa2 - 4*h1*sina2, <0 0 0>]
line	[x_qo2 + 5*h1*cosa2, y_qo2 - 5*h1*sina2, x_qo2 + h1*sina2 + 5*h1*cosa2, y_qo2 + h1*cosa2 - 5*h1*sina2, <0 0 0>]
line	[x_qo2 + 6*h1*cosa2, y_qo2 - 6*h1*sina2, x_qo2 + h1*sina2 + 6*h1*cosa2, y_qo2 + h1*cosa2 - 6*h1*sina2, <0 0 0>]
line	[x_qo2 + 7*h1*cosa2, y_qo2 - 7*h1*sina2, x_qo2 + h1*sina2 + 7*h1*cosa2, y_qo2 + h1*cosa2 - 7*h1*sina2, <0 0 0>]
//----------------------число в очереди - вход станок 1------------------------------------------------		 
text    [ x_qo2 + 20, y_qo2, 90, 25, <127 127 127>, <100 255 0>, QEXITST2.длина_очереди ]

//----------------------очередь станка 2 - вход------------------------------------
triang	[x_qi2, 						y_qi2, 
		 x_qi2 + l1*cosa2, 			y_qi2 - l1*sina2,
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <127 127 127>, <127 127 127>]

triang	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			y_qi2 + h1*cosa2,
		 x_qi2, 							y_qi2,
		  <127 127 127>, <127 127 127>]

//----------------------очередь станка 2 - вход------------------------------------
show_if QSTANOK2.длина_очереди <= 8
triang	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.длина_очереди*cosa2, 			y_qi2 - h1*QSTANOK2.длина_очереди*sina2,
		 x_qi2 + h1*QSTANOK2.длина_очереди*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.длина_очереди*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + h1*QSTANOK2.длина_очереди*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.длина_очереди*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 								    y_qi2 + h1*cosa2,
		 x_qi2, 						 						y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.длина_очереди*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.длина_очереди*sina2 + h1*cosa2,
		 <255 255 0>]
		 
show_if QSTANOK2.длина_очереди > 8
triang	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cosa2, 			 y_qi2 - l1*sina2,
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			 y_qi2 + h1*cosa2,
		 x_qi2, 						 y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <255 255 0>]

show
//----------------------очередь станка 2 вход - оконтовка------------------------------------

line	[x_qi2, 							y_qi2, 
		 x_qi2 + l1*cosa2,				y_qi2 - l1*sina2, 
		 <0  0 0>]
line	[x_qi2 + l1*cosa2,				y_qi2 - l1*sina2, 
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			y_qi2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qi2+ h1*sina2, 				y_qi2 + h1*cosa2, 
		 x_qi2, 							y_qi2, 
		 <0  0 0>]

//----------------------линии очереди - вход станка 2------------------------------------------------		 
		 
line	[x_qi2 + 1*h1*cosa2, y_qi2 - 1*h1*sina2, x_qi2 + h1*sina2 + 1*h1*cosa2, y_qi2 + h1*cosa2 - 1*h1*sina2, <0 0 0>]
line	[x_qi2 + 2*h1*cosa2, y_qi2 - 2*h1*sina2, x_qi2 + h1*sina2 + 2*h1*cosa2, y_qi2 + h1*cosa2 - 2*h1*sina2, <0 0 0>]
line	[x_qi2 + 3*h1*cosa2, y_qi2 - 3*h1*sina2, x_qi2 + h1*sina2 + 3*h1*cosa2, y_qi2 + h1*cosa2 - 3*h1*sina2, <0 0 0>]
line	[x_qi2 + 4*h1*cosa2, y_qi2 - 4*h1*sina2, x_qi2 + h1*sina2 + 4*h1*cosa2, y_qi2 + h1*cosa2 - 4*h1*sina2, <0 0 0>]
line	[x_qi2 + 5*h1*cosa2, y_qi2 - 5*h1*sina2, x_qi2 + h1*sina2 + 5*h1*cosa2, y_qi2 + h1*cosa2 - 5*h1*sina2, <0 0 0>]
line	[x_qi2 + 6*h1*cosa2, y_qi2 - 6*h1*sina2, x_qi2 + h1*sina2 + 6*h1*cosa2, y_qi2 + h1*cosa2 - 6*h1*sina2, <0 0 0>]
line	[x_qi2 + 7*h1*cosa2, y_qi2 - 7*h1*sina2, x_qi2 + h1*sina2 + 7*h1*cosa2, y_qi2 + h1*cosa2 - 7*h1*sina2, <0 0 0>]
//----------------------число в очереди - вход станок 2------------------------------------------------		 
text    [ x_qi2 - 10, y_qi2 - 25, 90, 25, <127 127 127>, <100 255 0>, QSTANOK2.длина_очереди ]


//----------------------Станок 11 - поломка------------------------------------
show		 
active  Операция_Поломка11 [x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]

//----------------------Станок 11 - свободен------------------------------------
Show_if ST11.Состояние = Свободен
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------Станок 11 - занят------------------------------------
Show_if ST11.Состояние = Занят		 
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]
		 
ellipse [x_st11 - 0.5*h_st11*sinast1 - l_st11*cosast1, y_st11 - 0.5*h_st11*cosast1 + l_st11*sinast1, 18, 18, 
		 <255 255 0>, <0 0 0>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
		 
//----------------------Станок 11 - сломан------------------------------------
Show_if ST11.Состояние = Сломан		 
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]

//----------------------Станок 12 - поломка------------------------------------
show		 
active  Операция_Поломка12  [x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]

//------------------------Станок 12 - свободен------------------------------
Show_if ST12.Состояние = Свободен

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//------------------------Станок 12 - занят------------------------------
Show_if ST12.Состояние = Занят

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]
		 
ellipse [x_st11 - 0.5*h_st11*sinast1, y_st11 - 0.5*h_st11*cosast1, 18, 18, <255 255 0>, <0 0 0>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]

//------------------------Станок 12 - сломан------------------------------
Show_if ST12.Состояние = Сломан

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------Станок 13 - поломка------------------------------------
show		 
active  Операция_Поломка13[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]


//----------------------Станок 13 - свободен------------------------------------
Show_if ST13.Состояние = Свободен
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------Станок 13 - занят------------------------------------
Show_if ST13.Состояние = Занят
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]

ellipse [x_st11 - 0.5*h_st11*sinast1 + l_st11*cosast1, y_st11 - 0.5*h_st11*cosast1 - l_st11*sinast1, 18, 18, 
		 <255 255 0>, <0 0 0>]


line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------Станок 13 - сломан------------------------------------
Show_if ST13.Состояние = Сломан
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
		 
//----------------------Станок 2 - поломка------------------------------------
show		 
active  Операция_Поломка2[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
						  y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2,
						  -h_st2*cosast2 - l_st2*sinast2,
						  l_st2*cosast2 - h_st2*sinast2]


//----------------------Станок 2 - свободен------------------------------------
Show_if STANOK2.Состояние = Свободен
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <210 210 210>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <210 210 210>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <210 210 210>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]
//----------------------Станок 2 - занят------------------------------------
Show_if STANOK2.Состояние = Занят
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <0 255 0>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <0 255 0>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <0 255 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]

ellipse [x_st2 - 0.5*h_st11*sinast1 + 18, y_st11 - 0.5*h_st11*cosast1 - 2, 18, 18, 
		 <255 255 0>, <0 0 0>]


//----------------------Станок 2 - сломан------------------------------------
Show_if STANOK2.Состояние = Сломан
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <255 0 0>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <255 0 0>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <255 0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]

$End
