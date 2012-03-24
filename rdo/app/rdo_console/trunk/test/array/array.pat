$Pattern образец_1 : operation trace
$Parameters
	параметр_образца_1 : integer
	параметр_образца_2 : array<integer>
$Relevant_resources
	рел_ресурс_1 : тип_ресурса_1 Keep Keep
$Time = константа_1 	
$Body
	рел_ресурс_1
		Choice from рел_ресурс_1.параметр_1 == параметр_образца_1 and рел_ресурс_1.параметр_3 == свободен
		first
		Convert_begin
			параметр_3 set занят;
		Convert_end
			параметр_1 set параметр_образца_1 + 1;
			параметр_2 set параметр_образца_2;
			параметр_3 set свободен;
$End