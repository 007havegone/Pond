#!/usr/bin/perl

$mapName = $ARGV[0];

@WallNames = ("Front", "Right", "Back", "Left");
@DirNames = ("North", "East", "South", "West");

open($in, "<", $mapName) or die "Can't open $mapName";

$header = <$in>;
unless($header =~ /(\d+) (\d+)/)
{
	die "Malformed Header: $_\n"; }

$width = $1;
$length = $2;
@map;

sub checkValid {
	my $x = shift;
	my $y = shift;
	if($x < 0 or $y < 0) {
		return 0;
	}
	elsif($x >= $width or $y >= $length) {
		return 0;
	}
	elsif($map[$y][$x] eq "#") {
		return 0;
	}
	else {
		return 1;
	}
}

sub mX {
	my $x = shift;
	my $dir = shift;
	if ($dir == 1) {return $x+1;}
	if ($dir == 3) {return $x-1;}
	return $x;
}

sub mY {
	my $y = shift;
	my $dir = shift;
	if ($dir == 2) {return $y+1;}
	if ($dir == 0) {return $y-1;}
	return $y;
}
while (<$in>) {
	push @map, [ split(undef, $_) ];
}

print "(define (domain hallway)\n";
print " (:requirements :probabilistic-effects\n";
print "                :negative-preconditions\n";
print "                :conditional-effects)\n";
print " (:predicates\n";

for $i (0 .. $width-1 ) {
	print "  (atX_$i)\n";
}

for $i (0 .. $length-1 ) {
	print "  (atY_$i)\n";
}

for $wall ( @WallNames ) {
	print "  ($wall)\n";
}

for $dir ( @DirNames ) {
	print "  ($dir)\n";
}

print ")\n";

@WallConfig;

#Group the locations by there wall layout...
for $config (0 .. 15) {
	push @WallConfig, [];
	for $x (0 .. $width-1) {
		for $y (0 .. $length-1) {
			if(checkValid($x, $y)) {
				if(checkValid($x, $y-1) == (($config>>0)&1) and 
				   checkValid($x+1, $y) == (($config>>1)&1) and
				   checkValid($x, $y+1) == (($config>>2)&1) and 
				   checkValid($x-1, $y) == (($config>>3)&1)) {
					push @{ $WallConfig[$config] }, {$x, $y};
				}
			}
		}
	}
}

#Turn Right...
print "\n\n";
print "(:action turn_right\n";
print " :effect (and\n";
for $dir (0 .. 3) {
	for $x (0 .. $width-1) {
		for $y (0 .. $length-1) {
			if(checkValid($x, $y)) {
				print "  (when (and ($DirNames[$dir]) (atX_$x) (atY_$y))\n";
				print "   (probabilistic\n";
				print "    .70 (and (not ($DirNames[$dir])) ($DirNames[($dir+1)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .10 (and (not ($DirNames[$dir])) ($DirNames[($dir+2)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .10 (and (not ($DirNames[$dir])) ($DirNames[($dir+3)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")))\n";
			}
		}
	}
}
print ")\n";
print " :observation(\n";
for $wall ( @WallNames ) {
	print "  (($wall) .9 .9)\n";
}
print "))\n";
#Turn Around...
print "\n\n";
print "(:action turn_around\n";
print " :effect (and\n";
for $dir (0 .. 3) {
	for $x (0 .. $width-1) {
		for $y (0 .. $length-1) {
			if(checkValid($x, $y)) {
				print "  (when (and ($DirNames[$dir]) (atX_$x) (atY_$y))\n";
				print "   (probabilistic\n";
				print "    .15 (and (not ($DirNames[$dir])) ($DirNames[($dir+1)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .65 (and (not ($DirNames[$dir])) ($DirNames[($dir+2)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .15 (and (not ($DirNames[$dir])) ($DirNames[($dir+3)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")))\n";
			}
		}
	}
}
print ")\n";
print " :observation(\n";
for $wall ( @WallNames ) {
	print "  (($wall) .9 .9)\n";
}
print "))\n";

#Turn Left...
print "\n\n";
print "(:action turn_left\n";
print " :effect (and\n";
for $dir (0 .. 3) {
	for $x (0 .. $width-1) {
		for $y (0 .. $length-1) {
			if(checkValid($x, $y)) {
				print "  (when (and ($DirNames[$dir]) (atX_$x) (atY_$y))\n";
				print "   (probabilistic\n";
				print "    .10 (and (not ($DirNames[$dir])) ($DirNames[($dir+1)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+1)%4), mY($y, ($dir+$wdir+1)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .10 (and (not ($DirNames[$dir])) ($DirNames[($dir+2)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+2)%4), mY($y, ($dir+$wdir+2)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")\n";
				print "    .70 (and (not ($DirNames[$dir])) ($DirNames[($dir+3)%4])\n";
				print "             ";
				for $wdir (0 .. 3) {
					if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
							not checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4))) {
						print "($WallNames[$wdir]) ";
					}
					if(checkValid(mX($x, ($dir+$wdir+3)%4), mY($y, ($dir+$wdir+3)%4)) and
							not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
						print "(not ($WallNames[$wdir])) ";
					}
				}
				print ")))\n";
			}
		}
	}
}
print ")\n";
print " :observation(\n";
for $wall ( @WallNames ) {
	print "  (($wall) .9 .9)\n";
}
print "))\n";

#move forward...
print "\n\n";
print "(:action move_forward\n";
print " :effect (and\n";
for $dir (0 .. 3) {
	for $x (0 .. $width-1) {
		for $y (0 .. $length-1) {
			if(checkValid($x, $y)) {
				print "  (when (and ($DirNames[$dir]) (atX_$x) (atY_$y))\n";
				print "   (probabilistic\n";
				$nX = mX($x, $dir);
				$nY = mY($y, $dir);
				if(checkValid($nX, $nY)) {
					print "     .80 (and \n";
					unless($nX == $x) {
						print "           (not (atX_$x)) (atX_$nX)\n";
					}
					unless($nY == $y) {
						print "           (not (atY_$y)) (atY_$nY)\n";
					}
					print "              ";
					for $wdir (0 .. 3) {
						if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
								not checkValid(mX($nX, ($dir+$wdir)%4), mY($nY, ($dir+$wdir)%4))) {
							print "($WallNames[$wdir]) ";
						}
						if(checkValid(mX($nX, ($dir+$wdir)%4), mY($nY, ($dir+$wdir)%4)) and
								not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
							print "(not ($WallNames[$wdir])) ";
						}
					}
					print ")\n";
				}
				$nX = mX($x, ($dir+1)%4);
				$nY = mY($y, ($dir+1)%4);
				if(checkValid($nX, $nY)) {
					print "     .05 (and \n";
					#print "           (not ($WallNames[$dir])) (".$WallNames[($dir+1)%4].")\n";
					unless($nX == $x) {
						print "           (not (atX_$x)) (atX_$nX)\n";
					}
					unless($nY == $y) {
						print "           (not (atY_$y)) (atY_$nY)\n";
					}
					print "           (not($DirNames[$wdir])) ($DirNames[($wdir+1)%4])\n";
					print "              ";
					for $wdir (0 .. 3) {
						if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
								not checkValid(mX($nX, ($dir+$wdir+1)%4), mY($nY, ($dir+$wdir+1)%4))) {
							print "($WallNames[$wdir]) ";
						}
						if(checkValid(mX($nX, ($dir+$wdir+1)%4), mY($nY, ($dir+$wdir+1)%4)) and
								not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
							print "(not ($WallNames[$wdir])) ";
						}
					}
					print ")\n";
				}
				$nX = mX($x, ($dir+2)%4);
				$nY = mY($y, ($dir+2)%4);
				if(checkValid($nX, $nY)) {
					print "     .05 (and \n";
					#print "           (not ($WallNames[$dir])) (".$WallNames[($dir+2)%4].")\n";
					unless($nX == $x) {
						print "           (not (atX_$x)) (atX_$nX)\n";
					}
					unless($nY == $y) {
						print "           (not (atY_$y)) (atY_$nY)\n";
					}
					print "           (not($DirNames[$wdir])) ($DirNames[($wdir+2)%4])\n";
					print "              ";
					for $wdir (0 .. 3) {
						if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
								not checkValid(mX($nX, ($dir+$wdir+2)%4), mY($nY, ($dir+$wdir+2)%4))) {
							print "($WallNames[$wdir]) ";
						}
						if(checkValid(mX($nX, ($dir+$wdir+2)%4), mY($nY, ($dir+$wdir+2)%4)) and
								not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
							print "(not ($WallNames[$wdir])) ";
						}
					}
					print ")\n";
				}
				$nX = mX($x, ($dir+3)%4);
				$nY = mY($y, ($dir+3)%4);
				if(checkValid($nX, $nY)) {
					print "     .05 (and \n";
					#print "           (not ($DirNames[$dir])) (".$DirNames[($dir+3)%4].")\n";
					unless($nX == $x) {
						print "           (not (atX_$x)) (atX_$nX)\n";
					}
					unless($nY == $y) {
						print "           (not (atY_$y)) (atY_$nY)\n";
					}
					print "           (not($DirNames[$wdir])) ($DirNames[($wdir+3)%4])\n";
					print "              ";
					for $wdir (0 .. 3) {
						if(checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4)) and
								not checkValid(mX($nX, ($dir+$wdir+3)%4), mY($nY, ($dir+$wdir+3)%4))) {
							print "($WallNames[$wdir]) ";
						}
						if(checkValid(mX($nX, ($dir+$wdir+3)%4), mY($nY, ($dir+$wdir+3)%4)) and
								not checkValid(mX($x, ($dir+$wdir)%4), mY($y, ($dir+$wdir)%4))) {
							print "(not ($WallNames[$wdir])) ";
						}
					}
					print ")\n";
				}
				print "    ))\n";
			}
		}
	}
}
print ")\n";
print " :observation(\n";
for $wall ( @WallNames ) {
	print "  (($wall) .9 .9)\n";
}
print ")))\n";
$count = 0;
for $x (0 .. $width-1) {
	for $y (0 .. $width-1) {
		if(checkValid($x, $y)) {$count++;}
	}
}

print "(define (problem hallway_problem)\n";
print " (:domain hallway)\n";
print " (:init\n";
print "  (probabilistic\n";
$prob = 1/($count*4);
for $x (0 .. $width-1){
	for $y (0 .. $length-1) {
		if(checkValid($x, $y)) {
			for $dir ( @DirNames ) {
				print "   $prob (and (atX_$x) (atY_$y) ($dir))\n";
			}
		}
	}
}
print "  )\n";
print " )\n";
for $x (0 .. $width-1){
	for $y (0 .. $length-1) {
		if($map[$y][$x] eq "g") {
			print " (:goal (and (atX_$x) (atY_$y)))\n";
		}
	}
}
print ")\n";
