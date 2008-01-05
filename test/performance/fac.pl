
sub fac {
	my ($n) = @_;
	if ($n == 0) {
		return(1);
	} else {
		return($n * fac($n - 1));
	}
}

fac(100);
fac(100);
fac(100);
fac(100);
fac(100);
fac(100);
fac(100);
fac(100);
fac(100);
fac(100);

