use strict;

open(source_studio,"rdo_studio_rus.qhp");
open(source_lang,"rdo_lang_rus.qhp");

if (-e "index.html") {
	unlink ("index.html");
}
open(target,">>index.html");

my @temp;
my $flagO=0;
my $flagC=0;
my @strings_studio=<source_studio>;
foreach my $line (@strings_studio)
{
	if ($line=~ /<\/toc>/) { $flagC = 1; }
	if ( $flagO != $flagC ) {
		push @temp, $line;
	}
	if ($line=~ /<toc>/) { $flagO = 1; }
}


my $flagO=0;
my $flagC=0;
my @strings_lang=<source_lang>;
foreach my $line (@strings_lang)
{
	if ($line=~ /<\/toc>/) { $flagC = 1; }
	if ( $flagO != $flagC ) {
		push @temp, $line;
	}
	if ($line=~ /<toc>/) { $flagO = 1; }
}
close (source_studio);
close (source_lang);


my $it = 0;
my $ii = 1;
my $size = @temp;
my @parents;
push @parents, $it;
foreach my $line (@temp)
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

open(source_html_head,"index-head.tmp");
my @strings_html_head=<source_html_head>;
foreach my $line (@strings_html_head)
{
	print (target $line);
}
close (source_html_head);

print (target @temp);

open(source_html_body,"index-body.tmp");
my @strings_html_body=<source_html_body>;
foreach my $line (@strings_html_body)
{
	print (target $line);
}
close (source_html_body);