$Pattern timer_pat : irregular_event trace
$Relevant_resources
	rel_sys			: system            keep
$Time = 0.1
$Body
rel_sys
	Convert_event
$End

$Pattern opr_begin: rule trace
$Relevant_resources
	rel_sys			: system            keep
	rel_machine_tool: machine_tool_type keep
	rel_part        : part_type         keep
$Body
rel_sys
	Choice from true
	Convert_rule opr_count set opr_count + 1
rel_machine_tool
	Choice from
		status = free
	Convert_rule
		opr_count set rel_sys.opr_count
		status    set busy
rel_part
	Choice from
		status = free
	Convert_rule
		opr_count  set rel_sys.opr_count
		status     set busy
		begin_time set time_now
$End

$Pattern opr_end: rule trace
$Relevant_resources
	rel_sys			: system            keep
	rel_part        : part_type         keep
	rel_machine_tool: machine_tool_type keep
$Body
rel_sys
	Choice from true
	Convert_rule part_done_count set part_done_count + 1
rel_part
	Choice from
		status = busy
	and begin_time + opr_time <= time_now
	Convert_rule
		opr_count set 0
		status    set ready
rel_machine_tool
	Choice from
		status = busy
	and opr_count = rel_part.opr_count
	Convert_rule
		opr_count set 0
		status    set free
$End

$Pattern breakage_pat : keyboard
$Relevant_resources
	rel_sys			: system            keep NoChange
	rel_part        : part_type         keep NoChange
	rel_machine_tool: machine_tool_type keep NoChange
$Time = 0
$Body
rel_sys
	Choice from true
	Convert_begin part_broken_count set part_broken_count + 1
rel_part
	Choice from
		status = busy
	Convert_begin
		opr_count set 0
		status    set broken
rel_machine_tool
	Choice from
		status = busy
	and opr_count = rel_part.opr_count
	Convert_begin
		opr_count set 0
		status    set broken
$End

$Pattern repair_pat : keyboard
$Relevant_resources
	rel_machine_tool: machine_tool_type Keep NoChange
$Time = 0
$Body
rel_machine_tool
	Choice from
		status = broken
	Convert_begin
		opr_count set 0
		status    set free
$End
