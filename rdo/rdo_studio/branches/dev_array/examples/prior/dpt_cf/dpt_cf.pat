$Pattern �������_������������_������� : operation trace
$Relevant_resources
	_��������������: �������������� Keep Keep
$Time = ������������_������������( 20, 40 )
$Body
_��������������
	Choice from _��������������.���������_����������� = �������� and _��������������.����������_�_������� > 0
	Convert_begin
		����������_�_�������  set _��������������.����������_�_������� - 1
		���������_����������� set �����
	Convert_end
		���������_�����������  set ��������
		����������_����������� set _��������������.����������_����������� + 1
$End

$Pattern �������_�������_������� : irregular_event no_trace
$Relevant_resources
	_��������������: �������������� Keep
$Time = ��������_�������( 30 )
$Body
_��������������
	Convert_event
		����������_�_������� set _��������������.����������_�_������� + 1
$End