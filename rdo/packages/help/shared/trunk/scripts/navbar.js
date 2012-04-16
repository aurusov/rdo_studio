var str = '<TABLE class="navigation" cellpadding="0"><TR class="navigation">'
var tdi = '<TD class="navigation" align=left width="26">';
var tdt = '<TD class="navigation" align=middle valign=middle nowrap style="padding-left: 5px; padding-right: 5px">';
var len = document.anchors.length;
var imgb = '<IMG class="navigation" align=middle hspace="0" vspace="0" height=18 border src="../';
var imge = '.gif" width=26';
str += '</TR></TABLE>';
document.write(str);
str = '<TABLE class="navigation" cellpadding="0"><TR class="navigation"><TD class="navigation" align=left width="26">&nbsp;</TD></TR></TABLE><DIV class="footer">';
str += 'Справка по языку РДО';
str += '<BR>';
str += 'МГТУ им. Н.Э. Баумана, 2002-2012 гг.</DIV>';
document.write(str);