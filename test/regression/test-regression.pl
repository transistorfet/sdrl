#
# Script Name:	test-regression.pl
# Description:	Automatic testing program for the sdrl interpreter
#

use strict;
use warnings;

my $sdrlcmd = "../../bin/sdrl";

my $max_width = 30;

my $tests_passed = 0;
my $tests_failed = 0;

main();
exit(0);

sub main {
	opendir(DIR, ".") or die "Failed to open the current directory for reading\n";
	my @files = readdir(DIR);
	closedir(DIR);

	foreach my $file (@files) {
		if ($file =~ /\.sdrl$/i) {
			test_file($file);
		}
	}

	print "\nPassed: $tests_passed\tFailed: $tests_failed\n";
}

sub test_file {
	my ($file) = @_;

	my $failed = 0;
	my @expect = load_expected_output($file);
	print_test_name($file);
	open(CMD, "$sdrlcmd $file |") or die "Unable to open the command $sdrlcmd $file\n";
	while (my $line = <CMD>) {
		my $test = shift(@expect);
		unless ($line =~ /$test/) {
			$failed = 1;
			last;
		}
	}
	close(CMD);

	if ($failed || scalar(@expect)) {
		print "FAILED\n";
		$tests_failed++;
	}
	else {
		print "passed\n";
		$tests_passed++;
	}
}

sub load_expected_output {
	my ($file) = @_;

	my $record = 0;
	my @lines = ();
	open(FILE, $file) or die "Failed to open $file for reading\n";
	while (my $line = <FILE>) {
		last if ($line =~ /END EXPECTED OUTPUT/);
		$line =~ s/^#( |)//;
		push(@lines, $line) if ($record);
		$record = 1 if ($line =~ /EXPECTED OUTPUT/);
	}
	close(FILE);
	return(@lines);
}

sub print_test_name {
	my ($file) = @_;

	my $msg = "Test $file ";
	print $msg;
	for (length($msg)..$max_width) {
		print ".";
	}
	print " ";
}


