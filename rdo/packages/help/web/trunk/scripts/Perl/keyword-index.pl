# use open OUT => ':utf8';
# use utf8;
# use local;

# use POSIX;
# POSIX::setlocale(&POSIX::LC_ALL, 'ru_RU.utf8');
# use locale;

# use utf8;
# binmode ( source_studio, ':utf8' );
# binmode ( source_lang, ':utf8' );
# binmode ( STDOUT, ':utf8' );

 # use locale;
 # use POSIX qw(locale_h);
 # setlocale(LC_ALL, 'ru_RU') or die 'locale!';

# use utf8;
# binmode source_studio, 'encoding(utf8)';
# binmode source_lang, 'encoding(utf8)';


open(source_studio,"rdo_studio_rus.qhp");
open(source_lang,"rdo_lang_rus.qhp");

if (-e "keyword-index.html") {
	unlink ("keyword-index.html");
}
open(target,">>keyword-index.html");

@strings1=<source_studio>;
foreach $line (@strings1)
{
	if ($line=~ /keyword name=/) {
		push @temp, $line;
	}
}

@strings2=<source_lang>;
foreach $line (@strings2)
{
	if ($line=~ /keyword name=/) {
		push @temp, $line;
	}
}
close (source_studio);
close (source_lang);

use encoding 'utf8';

@temp2 = sort { uc ($a) cmp uc ($b) } @temp;

foreach $line (@temp2)
{
	$line =~ s/\t{3}<keyword name=\"//;
	$line =~ s/\" ref=\"/|/;
	$line =~ s/\"\/>\n//;
	@a = split(/\|/, $line);
	push @temp3, ([$a[0],$a[1]]);
}

sub callerUrl {
	$path = $_[0];
	open(tempFile,$path);
	@stringsTemp=<tempFile>;
	foreach $line (@stringsTemp)
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
@strings3=<source_html_head>;
foreach $line (@strings3)
{
	print (target $line);
}
close (source_html_head);

$it = 0;
$ii = 1;
$size = @temp3;
for (my $i = 0; $i <= $size-1; ++$i,++$ii,++$it) 
{
	if ($temp3[$i][0] eq $temp3[$i+1][0]) {
		$parrent = $ii;
		print (target "\t\tList[$it] = \"$ii\|0\|$temp3[$i][0]\|\";\n");
		--$i;
		do  {
			++$i;
			++$ii;
			++$it;
			my $parrentName = callerUrl ($temp3[$i][1]);
			print (target "\t\t\tList[$it] = \"$ii\|$parrent\|$parrentName\|help\/$temp3[$i][1]\";\n");
		} until ($temp3[$i][0] ne $temp3[$i+1][0])
	}
	else {
		if ( $temp3[$i][0] eq '\\' ) { print (target "\t\tList[$it] = \"$ii\|0\|\\\\\|help\/$temp3[$i][1]\";\n"); }
		else { print (target "\t\tList[$it] = \"$ii\|0\|$temp3[$i][0]\|help\/$temp3[$i][1]\";\n"); }
	}
}

open(source_html_body,"keyword-index-body.tmp");
@strings4=<source_html_body>;
foreach $line (@strings4)
{
	print (target $line);
}
close (source_html_body);
close (target);

