$Constant
	ширина: real = 40
	высота: real = 40
	pi    : real = 3.14
$End

$Function ѕриведение_угла: real
$Type = algorithmic
$Parameters
	текущий_угол: real
$Body
	Calculate_if текущий_угол < 0   ѕриведение_угла = текущий_угол + 360
	Calculate_if текущий_угол > 360 ѕриведение_угла = текущий_угол - 360
	Calculate_if 1 = 1              ѕриведение_угла = текущий_угол
$End

$Function ¬_какую_сторону_крутить: integer
$Type = algorithmic
$Parameters
	текущий_поворот  : real
	требуемый_поворот: real
$Body
	Calculate_if требуемый_поворот = текущий_поворот ¬_какую_сторону_крутить =  0
	Calculate_if требуемый_поворот > текущий_поворот and требуемый_поворот - текущий_поворот >= 180 ¬_какую_сторону_крутить = -1
	Calculate_if требуемый_поворот > текущий_поворот and требуемый_поворот - текущий_поворот <  180 ¬_какую_сторону_крутить =  1
	Calculate_if требуемый_поворот < текущий_поворот and требуемый_поворот - текущий_поворот >= 180 ¬_какую_сторону_крутить =  1
	Calculate_if требуемый_поворот < текущий_поворот and требуемый_поворот - текущий_поворот <  180 ¬_какую_сторону_крутить = -1
$End

$Function ≈ще_поворачивать: such_as –оботы.действие
$Type = algorithmic
$Parameters
	текущий_поворот  : real
	требуемый_поворот: real
$Body
	Calculate_if текущий_поворот = требуемый_поворот ≈ще_поворачивать = нет
	Calculate_if 1 = 1                               ≈ще_поворачивать = поворот
$End

$Function ≈ще_двигатьс€: such_as –оботы.действие
$Type = algorithmic
$Parameters
	текущий_x  : real
	текущий_y  : real
	требуемый_x: real
	требуемый_y: real
$Body
	Calculate_if abs(текущий_x - требуемый_x) < 2 and abs(текущий_y - требуемый_y) < 2 ≈ще_двигатьс€ = нет
	Calculate_if 1 = 1                                                                 ≈ще_двигатьс€ = движение
$End

$Function ѕроверить_приращение: real
$Type = algorithmic
$Parameters
	текущий: real
	куда   : real
	дельта : real
$Body
	Calculate_if abs(куда + дельта - текущий) < 1 ѕроверить_приращение = 0
	Calculate_if 1 = 1                            ѕроверить_приращение = дельта
$End

$Function Ќаправление_движени€: real
$Type = algorithmic
$Parameters
	текущий_x  : real
	требуемый_x: real
$Body
	Calculate_if текущий_x >= требуемый_x Ќаправление_движени€ = 1
	Calculate_if 1 = 1                    Ќаправление_движени€ = 1
$End

$Function ”гол_между: real
$Type = algorithmic
$Parameters
	текущий_x  : real
	текущий_y  : real
	требуемый_x: real
	требуемый_y: real
$Body
	Calculate_if abs(текущий_y - требуемый_y) < 0.0001 and текущий_x <= требуемый_x ”гол_между = pi / 2
	Calculate_if abs(текущий_y - требуемый_y) < 0.0001 and текущий_x  > требуемый_x ”гол_между = pi * 3 / 2
	Calculate_if 1 = 1                                                              ”гол_между = pi / 2 - (требуемый_x - текущий_x)/(требуемый_y - текущий_y)
$End
