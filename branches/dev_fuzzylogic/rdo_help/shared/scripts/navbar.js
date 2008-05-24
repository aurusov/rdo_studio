var str = '<TABLE class="navigation" cellpadding="0"><TR class="navigation">'
var tdi = '<TD class="navigation" align=left width="26">';
var tdt = '<TD class="navigation" align=middle valign=middle nowrap style="padding-left: 5px; padding-right: 5px">';
var len = document.anchors.length;
var ips = 'prev_section';
var ins = 'next_section';
var ist = 'section_top';
var ipt = 'prev_topic';
var int = 'next_topic';
var imgb = '<IMG class="navigation" align=middle hspace="0" vspace="0" height=18 border src="../';
var imge = '.gif" width=26';
for (var i = 0; i < len - 1; i++) {
	var an = document.anchors[i];
	var rf = an.name.split(";");
	if ( rf[0] == ips || rf[0] == ins || rf[0] == ist || rf[0] ==ipt || rf[0] == int ) {
		str += tdi;
		if ( rf[1] != "" && rf[2] != "" )
			str += '<A href="' + rf[1] + '">' + imgb + rf[0] + imge + ' alt="' + rf[2] + '"></A></TD>';
		else
			str += imgb + rf[0] + '_d' + imge + '></TD>';
	} else if ( rf[0] == "section" ) {
		str += tdt;
		if ( rf[1] != "" )
			str += '<FONT class="navigation">' + rf[1] + '</FONT></TD>';
	}
}
str += '</TR></TABLE>';
document.write(str);
var an = document.anchors[len-1];
var rf = an.name.split(";");
str = '<TABLE class="navigation" cellpadding="0"><TR class="navigation"><TD class="navigation" align=left width="26">&nbsp;</TD></TR></TABLE><DIV class="footer">';
if ( rf[0] == 'title' ) {
	str += rf[1];
	str += '.<BR>';
}
str += 'Ã√“” ËÏ. Õ.›. ¡‡ÛÏ‡Ì‡, 2002-2008 „„.</DIV>';
document.write(str);