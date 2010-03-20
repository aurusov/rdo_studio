$Frame frame
$Back_picture = <127 127 127> 800 600
Show
	rect    [130, 20, 120, 90, <200 100 0>, <0 0 0> ]
	rect    [130, 120, 120, 90, <200 100 0>, <0 0 0> ]
Show_if machine_tool_1.status = busy
	ellipse [180, 55, 20, 20, <255 0 255>, <0 0 0> ]
Show_if machine_tool_2.status = busy
	ellipse [180, 155, 20, 20, <255 0 255>, <0 0 0> ]
Show_if part_1.status = free
	ellipse [10,  10, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_2.status = free
	ellipse [10,  40, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_3.status = free
	ellipse [10,  70, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_4.status = free
	ellipse [10, 100, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_5.status = free
	ellipse [10, 130, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_6.status = free
	ellipse [10, 160, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_7.status = free
	ellipse [10, 190, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_8.status = free
	ellipse [10, 220, 20, 20, <0 0 255>, <0 0 0> ]
Show_if part_1.status = ready
	ellipse [310,  10, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_2.status = ready
	ellipse [310,  40, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_3.status = ready
	ellipse [310,  70, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_4.status = ready
	ellipse [310, 100, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_5.status = ready
	ellipse [310, 130, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_6.status = ready
	ellipse [310, 160, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_7.status = ready
	ellipse [310, 190, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_8.status = ready
	ellipse [310, 220, 20, 20, <0 255 0>, <0 0 0> ]
Show_if part_1.status = broken
	ellipse [350,  10, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_2.status = broken
	ellipse [350,  40, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_3.status = broken
	ellipse [350,  70, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_4.status = broken
	ellipse [350, 100, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_5.status = broken
	ellipse [350, 130, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_6.status = broken
	ellipse [350, 160, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_7.status = broken
	ellipse [350, 190, 20, 20, <255 0 0>, <0 0 0> ]
Show_if part_8.status = broken
	ellipse [350, 220, 20, 20, <255 0 0>, <0 0 0> ]
$End
