open(source_studio,"rdo_studio_rus.qhp");
open(source_lang,"rdo_lang_rus.qhp");

if (-e "help-index.html") {
	unlink ("help-index.html");
}
open(target,">>help-index.html");


$flagO=0;
$flagC=0;
@strings1=<source_studio>;
foreach $line (@strings1)
{
	if ($line=~ /<\/toc>/) { $flagC = 1; }
	if ( $flagO != $flagC ) {
		push @temp, $line;
	}
	if ($line=~ /<toc>/) { $flagO = 1; }
}


$flagO=0;
$flagC=0;
@strings2=<source_lang>;
foreach $line (@strings2)
{
	if ($line=~ /<\/toc>/) { $flagC = 1; }
	if ( $flagO != $flagC ) {
		push @temp, $line;
	}
	if ($line=~ /<toc>/) { $flagO = 1; }
}
close (source_studio);
close (source_lang);


$it = 0;
$ii = 1;
$size = @temp;
push @parents, $it;
foreach $line (@temp)
{
	if ( $line =~ /\"\/>/ ) {
		$line =~ s/<section title="/Tree[$it] = \"$ii\|$parents[-1]\|/;
		$line =~ s/\" ref=\"/\|help\//;
		$line =~ s/\"\/>/\";/;
		
		++$it;
		++$ii;
	}
	else {if ( $line =~ /\">/ ) {
		$line =~ s/<section title="/Tree[$it] = \"$ii\|$parents[-1]\|/;
		$line =~ s/\" ref=\"/\|help\//;
		$line =~ s/\">/\";/;
		
		push @parents, $ii;
		++$it;
		++$ii;
	}
	else  {
		$line =~ s/\t+<\/section>\n//;
		pop @parents;
	}}

}

open(source_html_head,"help-index-head.tmp");
@strings3=<source_html_head>;
foreach $line (@strings3)
{
	print (target $line);
}
close (source_html_head);

print (target @temp);

open(source_html_body,"help-index-body.tmp");
@strings4=<source_html_body>;
foreach $line (@strings4)
{
	print (target $line);
}
close (source_html_body);