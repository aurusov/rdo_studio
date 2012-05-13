use strict;
open(source_studio,"rdo_studio_rus.qhp");
open(source_lang,"rdo_lang_rus.qhp");

if (-e "keyword-index.html") {
	unlink ("keyword-index.html");
}
open(target,">>keyword-index.html");

my @temp;
my @strings_studio=<source_studio>;
foreach my $line (@strings_studio)
{
	if ($line=~ /keyword name=/) {
		push @temp, $line;
	}
}

my @strings_lang=<source_lang>;
foreach my $line (@strings_lang)
{
	if ($line=~ /keyword name=/) {
		push @temp, $line;
	}
}
close (source_studio);
close (source_lang);

my @temp2 = sort { uc (pack 'U0C*', unpack 'C*', $a) cmp uc (pack 'U0C*', unpack 'C*', $b) } @temp;
my @temp3;
foreach my $line (@temp2)
{
	$line =~ s/\t{3}<keyword name=\"//;
	$line =~ s/\" ref=\"/|/;
	$line =~ s/\"\/>\n//;
	my @a = split(/\|/, $line);
	push @temp3, ([$a[0],$a[1]]);
}

sub callerUrl {
	my $path = $_[0];
	open(tempFile,$path);
	my @stringsTemp=<tempFile>;
	foreach my $line (@stringsTemp)
	{	
		if ($line =~ /<TITLE>/) {
			$line =~ s/<TITLE>//;
			$line =~ s/<\/TITLE>\n//;
			return $line;
		}
	}
	close(tempFile);
}

open(source_html_head,"keyword-index-head.tmp");
my @strings3=<source_html_head>;
foreach my $line (@strings3)
{
	print (target $line);
}
close (source_html_head);

my $it = 0;
my $ii = 1;
my $size = @temp3;
for (my $i = 0; $i <= $size-1; ++$i,++$ii,++$it) 
{
	if ($temp3[$i][0] eq $temp3[$i+1][0]) { #печать сгруппированных строк
		my $parrent = $ii;
		print (target "\t\tList[$it] = \"$ii\|0\|$temp3[$i][0]\|\";\n"); #печать строки-родителя
		--$i;
		do  {
			++$i;
			++$ii;
			++$it;
			my $parrentName = callerUrl ($temp3[$i][1]);
			print (target "\t\t\tList[$it] = \"$ii\|$parrent\|$parrentName\|help\/$temp3[$i][1]\";\n"); #печать строки-потомка
		} until ($temp3[$i][0] ne $temp3[$i+1][0])
	}
	else {
		if ( $temp3[$i][0] eq '\\' ) { print (target "\t\tList[$it] = \"$ii\|0\|\\\\\|help\/$temp3[$i][1]\";\n"); }
		else { print (target "\t\tList[$it] = \"$ii\|0\|$temp3[$i][0]\|help\/$temp3[$i][1]\";\n"); }
	}
}

open(source_html_body,"keyword-index-body.tmp");
my @strings4=<source_html_body>;
foreach my $line (@strings4)
{
	print (target $line);
}
close (source_html_body);
close (target);

