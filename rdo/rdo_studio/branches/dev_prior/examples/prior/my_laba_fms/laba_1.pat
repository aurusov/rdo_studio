$Pattern Погрузка_детали :  operation trace
$Relevant_resources
	накопитель_	: Накопители Keep NoChange
	деталь_		: Детали Keep Keep
	робот_		: Роботы Keep Keep
	тележка_  	: Тележки Keep Keep
$Time = Экспоненциальный( Время_погрузки )
$Body
	накопитель_
		Choice from накопитель_.номер = 1 and накопитель_.текущее_количество > 0
		first
		Convert_begin
			текущее_количество set накопитель_.текущее_количество - 1
	
	деталь_
		Choice from деталь_.положение = накопитель_1
		first
		Convert_begin
			положение set нигде
			состояние set транспортируется
		Convert_end
			положение set тележка_1_н

	робот_
		Choice from робот_.номер = 1 and робот_.положение = накопитель_1 and робот_.состояние = свободен
		first
		Convert_begin
			положение set нигде
			состояние set занят
		Convert_end
			положение set тележка_1_н		
			состояние set свободен
	тележка_
		Choice from тележка_.номер = 1 and тележка_.положение = тележка_1_н and тележка_.состояние = свободен
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set загружен
$End

$Pattern Погрузка_детали_на_вторую_тележку :  operation trace
$Parameters
	номер_накопителя   : such_as Накопители.положение
	номер_тележки      : such_as Накопители.положение
	номер_робота       : such_as Накопители.положение
$Relevant_resources
	накопитель_	: Накопители Keep NoChange
	деталь_		: Детали Keep Keep
	робот_		: Роботы Keep Keep
	тележка_  	: Тележки Keep Keep
$Time = Экспоненциальный( Время_погрузки )
$Body
	накопитель_
		Choice from накопитель_.положение = номер_накопителя and накопитель_.текущее_количество > 0
		first
		Convert_begin
			текущее_количество set накопитель_.текущее_количество - 1
	
	деталь_
		Choice from деталь_.положение = номер_накопителя
		first
		Convert_begin
			положение set нигде
			состояние set транспортируется
		Convert_end
			положение set номер_тележки

	робот_
		Choice from робот_.положение = номер_робота and робот_.состояние = свободен
		first
		Convert_begin
			положение set нигде
			состояние set занят
		Convert_end
			положение set номер_тележки
			состояние set свободен
	тележка_
		Choice from тележка_.положение = номер_тележки and тележка_.состояние = свободен
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set загружен
$End

$Pattern Доставка_детали :  operation trace
$Parameters
	номер_тележки : integer
	начальное_положение_устройства : such_as Накопители.положение
	конечное_положение_устройства  : such_as Накопители.положение
$Relevant_resources
	тележка_  : Тележки Keep Keep	
	деталь_	  : Детали NoChange Keep	
$Time = Экспоненциальный( Время_доставки )
$Body
	тележка_	  
		Choice from тележка_.положение = начальное_положение_устройства 
				and тележка_.состояние = загружен
		first
		Convert_begin
			состояние set перемещается
		Convert_end
			положение set конечное_положение_устройства 
			состояние set прибыл

	деталь_
		Choice from деталь_.положение = начальное_положение_устройства
		first
		Convert_end
			положение set конечное_положение_устройства 
$End

$Pattern Возврат_робота :  operation trace
$Relevant_resources
	робот_	  : Роботы Keep Keep	
$Time = Время_возврата
$Body
	робот_	  
		Choice from робот_.положение <> место_возврата(робот_.положение)
				and робот_.состояние = свободен
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set свободен
			положение set место_возврата(робот_.положение)
$End

$Pattern Возврат_тележки :  operation trace
$Parameters
	номер_тележки : integer
	начальное_положение_устройства : such_as Накопители.положение
	конечное_положение_устройства  : such_as Накопители.положение
$Relevant_resources
	тележка_  : Тележки Keep Keep		
$Time = Экспоненциальный( Время_доставки )
$Body
	тележка_	  
		Choice from тележка_.положение = начальное_положение_устройства 
				and тележка_.состояние = свободен
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set свободен
			положение set конечное_положение_устройства 
$End

$Pattern Установка_на_станке :  operation trace
$Parameters
	номер_станка : such_as Накопители.положение
	положение_устройства : such_as Накопители.положение
$Relevant_resources	
	станок_	  :	Станки Keep Keep
	тележка_  : Тележки Keep Keep		
	робот_	  : Роботы Keep Keep	
	деталь_	  : Детали NoChange Keep
$Time = Экспоненциальный( Время_установки_на_станок )
$Body
	станок_	  
		Choice from станок_.положение = номер_станка and станок_.состояние = свободен
		first
		Convert_begin
			состояние set загружается
		Convert_end
			состояние set готов_к_обработке

	тележка_	  
		Choice from тележка_.положение = положение_устройства 
				and тележка_.состояние = прибыл
		first
		Convert_begin
			состояние set ожидает
		Convert_end
			состояние set свободен

	робот_	  
		Choice from робот_.положение = положение_устройства 
				and робот_.состояние = свободен
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set свободен
			положение set станок_.положение	   

	деталь_
		Choice from деталь_.положение = положение_устройства 
		first
		Convert_end
			положение set номер_станка 
$End

$Pattern Обработка_на_станке :  operation trace
$Parameters
	номер_станка : such_as Накопители.положение
$Relevant_resources
	станок_	  :	Станки Keep Keep
	деталь_	  : Детали Keep Keep
$Time = станок_.время_работы
$Body
	станок_	  
		Choice from станок_.положение = номер_станка and станок_.состояние = готов_к_обработке
		first
		Convert_begin
			состояние set работает
		Convert_end
			состояние set закончил_обработку
	
	деталь_
		Choice from деталь_.положение = номер_станка
		first	
		Convert_begin	
			состояние set обрабатывается
		Convert_end
			состояние set обработка_закончена
$End

$Pattern Разгрузка_станков :  operation trace
$Parameters
	номер_станка       : such_as Накопители.положение
	номер_накопителя   : such_as Накопители.положение
$Relevant_resources
	станок_	     : Станки     Keep Keep
	робот_	     : Роботы     Keep Keep
	накопитель_  : Накопители NoChange Keep	
	деталь_	     : Детали     Keep Keep
$Time = Экспоненциальный( Время_разгрузки_станка )
$Body
	станок_	  
		Choice from станок_.положение = номер_станка and станок_.состояние = закончил_обработку
		first
		Convert_begin
			состояние set разгружается
		Convert_end
			состояние set свободен
	
	робот_	  
		Choice from робот_.состояние = свободен and робот_станок(робот_.номер, станок_.номер) = 1
		first
		Convert_begin
			состояние set занят
		Convert_end
			состояние set свободен
			положение set накопитель_.положение	   
	
	накопитель_	  
		Choice from накопитель_.положение = номер_накопителя and накопитель_.текущее_количество < накопитель_.максимальное_количество
		first
		Convert_end
			текущее_количество set накопитель_.текущее_количество + 1
	
	деталь_
		Choice from деталь_.положение = номер_станка
		first	
		Convert_begin	
			состояние set транспортируется
		Convert_end
			положение set накопитель_.положение
$End

$Pattern Окончание_обработки :  operation trace
$Parameters
	номер_станка : such_as Накопители.положение
$Relevant_resources
	станок_	  	: Станки Keep Keep
	деталь_	  	: Детали Keep Keep
	накопитель_ : Накопитель_2 NoChange Keep
$Time = Экспоненциальный( Время_разгрузки_станка )
$Body
	станок_	  
		Choice from станок_.положение = номер_станка and станок_.состояние = закончил_обработку
		first
		Convert_begin
			состояние set разгружается
		Convert_end
			состояние set свободен

	деталь_
		Choice from деталь_.положение = номер_станка
		first	
		Convert_begin	
			состояние set транспортируется
		Convert_end
			положение set накопитель_2
			состояние set хранится


	накопитель_
		Choice NoCheck
		first	
		Convert_end
			текущее_количество set накопитель_.текущее_количество + 1
$End

$Pattern Работа_таймера : irregular_event trace
$Relevant_resources
	накопитель	  :	Накопитель_1 Keep
$Time = 0.5
$Body
	накопитель
		Convert_event
			положение set накопитель.положение
$End
