#
# Script Name:	test-performance.pl
# Description:	Automatic testing program for comparing language performances
#

use strict;
use warnings;

my $cmds = {
	"sdrl" => "../../bin/sdrl",
	"lua" => "lua",
	"pl" => "perl",
	"lsp" => "clisp",
	"scm" => "guile -s"
};

main();
exit(0);

sub main {
	my ($test) = @ARGV;

	opendir(DIR, ".") or die "Failed to open the current directory for reading\n";
	my @files = readdir(DIR);
	closedir(DIR);

	print_header();
	foreach my $file (@files) {
		if ($file =~ /^\Q$test\E\./i) {
			test_file($file);
		}
	}
}

sub test_file {
	my ($file) = @_;

	$file =~ /\.(.*?)$/;
	my $ext = $1;
	my $cmd = $cmds->{ $ext };
	return unless defined($cmd);

	my $info = { };
	open(CMD, "time -p $cmd $file 2>&1 |") or die "Unable to open the command $cmd\n";
	while (my $line = <CMD>) {
		if ($line =~ /^(real|user|sys)\s+(.*)(|(|\r)\n)$/) {
			$info->{ $1 } = $2;
		}
	}
	close(CMD);
	print_test_info($file, $info);
}

sub print_header {
	print format_width("Test", 30);
	print format_width("real", 15);
	print format_width("user", 15);
	print format_width("sys", 15);
	print "\n";
}

sub print_test_info {
	my ($file, $info) = @_;

	print format_width($file, 30);
	print format_width($info->{'real'}, 15);
	print format_width($info->{'user'}, 15);
	print format_width($info->{'sys'}, 15);
	print "\n";
}

sub format_width {
	my ($str, $max, $ch) = @_;

	$ch = ' ' unless defined($ch);
	my $fill;
	for (length($str)..$max) {
		$fill .= $ch;
	}
	return($str . $fill);
}


