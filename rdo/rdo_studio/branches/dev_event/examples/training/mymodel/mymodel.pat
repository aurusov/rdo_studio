$Pattern patTrueState: rule trace
$Relevant_resources
	relRes: resType Keep
$Body
relRes
	Choice from relRes.state == 0
	Convert_rule
		state = 1;
$End

$Pattern patExchange: rule trace
$Relevant_resources
	relRes1: resType Keep
	relRes2: resType Keep
$Body
relRes1
	Choice from relRes1.state == 1
	Convert_rule
		tmpParam = relRes1.param;
		param    = relRes2.param;
		state    = 2;
relRes2
	Choice from relRes2.state == 1 and relRes2.id != relRes1.id
	Convert_rule
		param    = relRes1.tmpParam;
		state    = 2;
$End
