$Constant
	timeIntervalBetweenArrivals: real = 150
	processingTimeMean         : real = 60
	processingTimeStDev        : real = 10
	loadingMachineTimeMean     : real = 6
	loadingMachineTimeStDev    : real = 0.5
	loadingExitTimeMean        : real = 5
	loadingExitTimeStDev       : real = 0.5
$End

$Sequence nextInflow : real
$Type = exponential 123456789
$End

$Sequence timeProcessing: real
$Type = normal 123456789
$End

$Sequence loadingMachineTime: real
$Type = uniform 123456789
$End

$Sequence loadingExitTime: real
$Type = uniform 123456789
$End

