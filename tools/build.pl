#!/usr/bin/perl
#
# Name:		build.pl
# Description:	Generate the makefiles and configuration files required for
#		building the program
#

use strict;
use warnings;
use IO::File;

my %object_mappings = (
	"c" => "o",
	"cpp" => "o"
);

my $makefiles = [ ];

main();
exit(0);

sub main {
	my ($file) = @ARGV;
	$file = "recipe.cfg" unless ($file);

	return(-1) if parse_config($file);
	foreach my $config (@{ $makefiles }) {
		generate_makefile($config);
	}
}

sub parse_config {
	my ($file) = @_;

	$file =~ /(.*)(\/|\\)(.*?)/;
	my $dir = defined($1) ? $1 : "";
	my ($linenum, $thisline) = (1, 1);
	my $config = { 'file' => $file, 'dir' => $dir, 'root' => get_path_to_root($dir), 'sources' => [ ], 'libraries' => [ ], 'configs' => [ ], 'builds' => [ ] };

	(my $fd = new IO::File("$file", "r")) or (print "Error: Unable to open $file\n" and return(-1));
	while (defined(my $line = read_line($fd, \$linenum))) {
		if (!$line) {
			## Ignore blank lines
		}
		elsif ($line =~ /^\s*build\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			parse_config(make_filename($dir, $name));
			$name =~ /(.*)(\/|\\)(.*?)/;
			push(@{ $config->{'builds'} }, $1);
		}
		elsif ($line =~ /^\s*target\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			$config->{'target'} = $name;
		}
		elsif ($line =~ /^\s*libtarget\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			$config->{'libtarget'} = $name;
		}
		elsif ($line =~ /^\s*source\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			push(@{ $config->{'sources'} }, get_source_files($dir, $name));
		}
		elsif ($line =~ /^\s*library\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			push(@{ $config->{'libraries'} }, $name);
		}
		elsif ($line =~ /^\s*config\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			push(@{ $config->{'configs'} }, $name);
		}
		elsif ($line =~ /^\s*root\s+(.*?)\s*$/i) {
			my ($name) = ($1);
			$config->{'root'} = $name;
		}
=stop
		elsif ($line =~ /^\s*define\s+(library|binary|module)\s+(.*?)\s*$/i) {
			my ($type, $name) = (lc($1), $2);
			if (defined($makefile->{'defines'}->{ $name }) and $makefile->{'defines'}->{ $name }->{'type'} ne $type) {
				print "Error $file:$thisline: Redeclaration of type for \"$name\"\n";
			}
			else {
				unless (defined($makefile->{'defines'}->{ $name })) {
					$makefile->{'defines'}->{ $name } = {
						'name' => $name,
						'type' => $type,
						'sources' => [ ],
						'libs' => [ ]
					};
				}
				parse_define($fd, $dir, $file, \$linenum, $makefile->{'defines'}->{ $name });
			}
		}
=cut
		else {
			print "Error $file:$thisline: Invalid syntax at \"$line\"\n";
		}
		$thisline = $linenum;
	}
	close($fd);
	push(@{ $makefiles }, $config);
	return(0);
}

sub generate_makefile {
	my ($config) = @_;

	my $makefile = make_filename($config->{'dir'}, "Makefile");
	open(FILE, ">$makefile") or (print "Unable to open file for writing: \"$makefile\"\n" and return(-1));
	print FILE "#\n# Automatically generated by tools/build.pl\n#\n\n";
	print FILE ".SUFFIXES: .o .c .h .cpp .s .asm .rc\n";
	print FILE ".PHONY: builds clean\n\n";

	print FILE "ROOT = $config->{'root'}\n";
	if (defined($config->{'libtarget'})) {
		print FILE "LIB_TARGET = $config->{'root'}/lib/$config->{'libtarget'}\n";
	}
	elsif (defined($config->{'target'})) {
		print FILE "TARGET = $config->{'root'}/bin/$config->{'target'}\n";
	}
	print FILE "\n";

	## Output the list of object files
	if (scalar(@{ $config->{'sources'} })) {
		print FILE "OBJS =\t";
		print FILE join("\t\\\n\t", map { get_object_file($_) } @{ $config->{'sources'} });
		print FILE "\n\n";
	}

	## Output the list of libraries 
	if (scalar(@{ $config->{'libraries'} })) {
		print FILE "LIBS =\t";
		print FILE join("\t\\\n\t", map { "-l$_" } @{ $config->{'libraries'} });
		print FILE "\n\n";
	}

	## Output configs (raw makefile statements)
	print FILE "include \$(ROOT)/config.mk\n\n";
	if (scalar(@{ $config->{'configs'} })) {
		print FILE join("\n", @{ $config->{'configs'} });
		print FILE "\n\n";
	}

	## Output main rule
	print FILE "all: builds \$(TARGET) \$(LIB_TARGET)\n\n";

	## Output the rule to build all the things we depend on
	print FILE "builds:\n";
	foreach my $build (@{ $config->{'builds'} }) {
		print FILE "\t\$(MAKE) -C $build all\n";
	}
	print FILE "\n";

	## Output generic rules
	if (defined($config->{'libtarget'}) || defined($config->{'target'})) {
		print FILE "\$(TARGET): builds \$(OBJS)\n";
		print FILE "\t\$(LD) \$(LDFLAGS) -o \$(TARGET) \$(OBJS) \$(LIBS)\n\n";
		print FILE "\$(LIB_TARGET): builds \$(OBJS)\n";
		print FILE "\t\$(AR) \$(ARFLAGS) \$@ \$(OBJS)\n";
		print FILE "\t\$(RANLIB) \$(LIB_TARGET)\n\n";
		print FILE ".c.o:\n";
		print FILE "\t\$(CC) \$(CFLAGS) -c -o \$@ \$<\n\n";
		print FILE ".cpp.o:\n";
		print FILE "\t\$(CXX) \$(CFLAGS) \$(CXXFLAGS) -c -o \$@ \$<\n\n";
	}

	## Output the rule to delete the object files
	print FILE "clean:\n";
	foreach my $build (@{ $config->{'builds'} }) {
		print FILE "\t\$(MAKE) -C $build clean\n";
	}
	print FILE "\t\$(RM) \$(OBJS)\n" if (scalar(@{ $config->{'sources'} }));
	print FILE "\n";

	## Output the file depend rules
	foreach my $source (@{ $config->{'sources'} }) {
		print FILE `cd $config->{'dir'}; gcc -MM -I$config->{'root'}/include $source` . "\n";
	}

	close(FILE);
}

sub get_source_files {
	my ($base, @sources) = @_;

	# TODO we assume we only get dir sources for now and just add all .c files
	my @files = ();
	foreach my $source (@sources) {
		$source = make_filename($base, $source);
		if (-d $source) {
			opendir(DIR, $source) or (print "Unable to open directory $source\n" and exit(-1));
			while (my $file = readdir(DIR)) {
				if ($file =~ /\.c$/) {
					$file = "$source/$file";
					$file =~ s/^\Q$base\/\E//;
					$file =~ s/^\.\///;
					push(@files, $file);
				}
			}
			closedir(DIR);
		}
		elsif (-e $source) {
			$source =~ s/^\Q$base\/\E//;
			$source =~ s/^\.\///;
			push(@files, $source);
		}
	}
	return(sort(@files));
}

sub get_path_to_root {
	my ($path) = @_;

	my $root = "";
	my @parts = split("/", $path);
	foreach my $part (@parts) {
		$root .= "../";
	}
	$root =~ s/\/$//;
	return($root);
}

sub make_filename {
	my ($dir, $file) = @_;

	$file =~ s/^\.(\/|\\)//;
	return($file) unless ($dir);
	$dir =~ s/(\/|\\)$//;
	return("$dir/$file");
}

##
# Given a source file name, return the corresponding object file name based
# on a list of source/object file extension pairings.
#
sub get_object_file {
	my ($file) = @_;

	foreach my $ext (keys(%object_mappings)) {
		if ($file =~ /\.\Q$ext\E$/) {
			$file =~ s/\.\Q$ext\E$/\.$object_mappings{ $ext }/;
			return($file);
		}
	}
	return($file);
}

##
# Read a line of input from a file in a standardized way, remove comments,
# and keep track of the line number.
#
sub read_line {
	my ($fd, $lineref) = @_;

	my $line = <$fd>;
	return(undef) unless defined($line);
	$line =~ s/(^|[^\\])\#.*$/$1/;		## Strip comments
	$line =~ s/\s*$//;
	$line =~ s/\\\#/\#/;
	while ($line =~ /(\\\s*(|\r)\n)$/) {	## If the line ends with '\' then read in the next line
		$line =~ s/\Q$1\E//;		## Removed the '\' and linebreak
		$line .= <$fd>;
		$line =~ s/\s*\#.*$//;		## Strip comments
		$$lineref++;			## Keep track of linenumbers
	}
	$line =~ s/(|\r)\n//;			## Remove line break
	$$lineref++;				## Keep track of linenumbers
	return($line);
}


