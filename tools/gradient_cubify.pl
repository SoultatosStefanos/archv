#!/usr/bin/perl

# Creative Commons Legal Code
#
# CC0 1.0 Universal
#
#     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
#     LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
#     ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
#     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
#     REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
#     PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
#     THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
#     HEREUNDER.
#
# Statement of Purpose
#
# The laws of most jurisdictions throughout the world automatically confer
# exclusive Copyright and Related Rights (defined below) upon the creator
# and subsequent owner(s) (each and all, an "owner") of an original work of
# authorship and/or a database (each, a "Work").

# Certain owners wish to permanently relinquish those rights to a Work for
# the purpose of contributing to a commons of creative, cultural and
# scientific works ("Commons") that the public can reliably and without fear
# of later claims of infringement build upon, modify, incorporate in other
# works, reuse and redistribute as freely as possible in any form whatsoever
# and for any purposes, including without limitation commercial purposes.
# These owners may contribute to the Commons to promote the ideal of a free
# culture and the further production of creative, cultural and scientific
# works, or to gain reputation or greater distribution for their Work in
# part through the use and efforts of others.
#
# For these and/or other purposes and motivations, and without any
# expectation of additional consideration or compensation, the person
# associating CC0 with a Work (the "Affirmer"), to the extent that he or she
# is an owner of Copyright and Related Rights in the Work, voluntarily
# elects to apply CC0 to the Work and publicly distribute the Work under its
# terms, with knowledge of his or her Copyright and Related Rights in the
# Work and the meaning and intended legal effect of CC0 on those rights.
#
# 1. Copyright and Related Rights. A Work made available under CC0 may be
# protected by copyright and related or neighboring rights ("Copyright and
# Related Rights"). Copyright and Related Rights include, but are not
# limited to, the following:
#
#   i. the right to reproduce, adapt, distribute, perform, display,
#      communicate, and translate a Work;
#  ii. moral rights retained by the original author(s) and/or performer(s);
# iii. publicity and privacy rights pertaining to a person's image or
#      likeness depicted in a Work;
#  iv. rights protecting against unfair competition in regards to a Work,
#      subject to the limitations in paragraph 4(a), below;
#   v. rights protecting the extraction, dissemination, use and reuse of data
#      in a Work;
#  vi. database rights (such as those arising under Directive 96/9/EC of the
#      European Parliament and of the Council of 11 March 1996 on the legal
#      protection of databases, and under any national implementation
#      thereof, including any amended or successor version of such
#      directive); and
# vii. other similar, equivalent or corresponding rights throughout the
#      world based on applicable law or treaty, and any national
#      implementations thereof.
#
# 2. Waiver. To the greatest extent permitted by, but not in contravention
# of, applicable law, Affirmer hereby overtly, fully, permanently,
# irrevocably and unconditionally waives, abandons, and surrenders all of
# Affirmer's Copyright and Related Rights and associated claims and causes
# of action, whether now known or unknown (including existing as well as
# future claims and causes of action), in the Work (i) in all territories
# worldwide, (ii) for the maximum duration provided by applicable law or
# treaty (including future time extensions), (iii) in any current or future
# medium and for any number of copies, and (iv) for any purpose whatsoever,
# including without limitation commercial, advertising or promotional
# purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
# member of the public at large and to the detriment of Affirmer's heirs and
# successors, fully intending that such Waiver shall not be subject to
# revocation, rescission, cancellation, termination, or any other legal or
# equitable action to disrupt the quiet enjoyment of the Work by the public
# as contemplated by Affirmer's express Statement of Purpose.
#
# 3. Public License Fallback. Should any part of the Waiver for any reason
# be judged legally invalid or ineffective under applicable law, then the
# Waiver shall be preserved to the maximum extent permitted taking into
# account Affirmer's express Statement of Purpose. In addition, to the
# extent the Waiver is so judged Affirmer hereby grants to each affected
# person a royalty-free, non transferable, non sublicensable, non exclusive,
# irrevocable and unconditional license to exercise Affirmer's Copyright and
# Related Rights in the Work (i) in all territories worldwide, (ii) for the
# maximum duration provided by applicable law or treaty (including future
# time extensions), (iii) in any current or future medium and for any number
# of copies, and (iv) for any purpose whatsoever, including without
# limitation commercial, advertising or promotional purposes (the
# "License"). The License shall be deemed effective as of the date CC0 was
# applied by Affirmer to the Work. Should any part of the License for any
# reason be judged legally invalid or ineffective under applicable law, such
# partial invalidity or ineffectiveness shall not invalidate the remainder
# of the License, and in such case Affirmer hereby affirms that he or she
# will not (i) exercise any of his or her remaining Copyright and Related
# Rights in the Work or (ii) assert any associated claims and causes of
# action with respect to the Work, in either case contrary to Affirmer's
# express Statement of Purpose.
#
# 4. Limitations and Disclaimers.
#
#  a. No trademark or patent rights held by Affirmer are waived, abandoned,
#     surrendered, licensed or otherwise affected by this document.
#  b. Affirmer offers the Work as-is and makes no representations or
#     warranties of any kind concerning the Work, express, implied,
#     statutory or otherwise, including without limitation warranties of
#     title, merchantability, fitness for a particular purpose, non
#     infringement, or the absence of latent or other defects, accuracy, or
#     the present or absence of errors, whether or not discoverable, all to
#     the greatest extent permissible under applicable law.
#  c. Affirmer disclaims responsibility for clearing rights of other persons
#     that may apply to the Work or any use thereof, including without
#     limitation any person's Copyright and Related Rights in the Work.
#     Further, Affirmer disclaims responsibility for obtaining any necessary
#     consents, permissions or other rights required for any use of the
#     Work.
#  d. Affirmer understands and acknowledges that Creative Commons is not a
#     party to this document and has no duty or obligation with respect to
#     this CC0 or use of the Work.

use strict;
use warnings;

# GradientSkybox.pl

# This is a program to render cube maps with gradient fades.
#  Images are spherically distorted, so that the result appears as a
#  sky "sphere" without visible edges

use Math::Trig qw(deg2rad cartesian_to_spherical pi);

# increase SIZE to take more samples (powers of 2, minus 1)
use constant SIZE => 255;
use constant H_SIZE => SIZE/2;

# DOWNSAMPLE is a division factor to merge multiple pixels
use constant DOWNSAMPLE => 1;

# globals
my %gradient;
my @points;

# Helper function: write a face to disk as a .ppm image
sub dump_face {
  my ($name, $image) = @_;

  my $output_size = (SIZE + 1) / DOWNSAMPLE;

  open my $fp, '>:raw', $name . '.ppm';
  print $fp "P6 $output_size $output_size 255\n";
  for my $y (0 .. $output_size - 1) {
    for my $x (0 .. $output_size - 1) {
      # supersample
      for my $c (0 .. 2) {
        my $color = 0;
        for my $j (0 .. DOWNSAMPLE - 1) {
          for my $i (0 .. DOWNSAMPLE - 1) {
            $color += $image->[$y * DOWNSAMPLE + $j][$x * DOWNSAMPLE + $i][$c] || 0;
          }
        }
        print $fp chr($color / (DOWNSAMPLE ** 2) + 0.5);
      }
    }
  }
}

# given an angle, return a color trio for the angle
sub interpolate {
  my ($angle) = @_;

  # determine the two user-specified points for
  #  the interval, by advancing until our angle lies between
  my $idx = 0;
  while ($idx + 1 < $#points &&
    $points[$idx + 1] < $angle) {
    $idx ++;
  }
  my ($start, $end) = ($points[$idx], $points[$idx + 1]);

  # perform lerp
  my $scale = ($angle - $start) / ($end - $start);
  #print STDERR "Angle = $angle, start = $start, end = $end, scale = $scale\n";

  my @color;
  foreach my $c (0 .. 2) {
    $color[$c] = $gradient{$start}[$c] + ($gradient{$end}[$c] - $gradient{$start}[$c]) * $scale;
  }

  return \@color;
}

##############################################################################
# The end cap angles are assumed to be 0 and 180
#  Cutoff for spherical images onto cubes is between 60 and 120 for
#  solid-colored caps
# please define colors as hex RRGGBB
die "Usage: $0 colorStart [color2:angle2] [color3:angle3] [...] colorEnd" unless @ARGV > 1;

for my $i (0 .. $#ARGV) {
  my ($angle, $color);
  if ($i == 0) {
    $angle = 0;
    $color = $ARGV[$i];
  } elsif ($i == $#ARGV) {
    $angle = pi;
    $color = $ARGV[$i];
  } else {
    ($color, $angle) = split /:/, $ARGV[$i];
    $angle = deg2rad($angle);
  }

  for my $c (0 .. 2) {
    $color .= '00';
    $gradient{$angle}[$c] = hex(substr($color, 0, 2));
    $color = substr($color, 2);
  }
}
@points = sort { $a <=> $b } keys %gradient;

print "Defined gradient stops:\n";
foreach my $idx (0 .. $#points) {
  printf("\t%02d: %f (%x %x %x)\n", $idx, $points[$idx], $gradient{$points[$idx]}[0], $gradient{$points[$idx]}[1], $gradient{$points[$idx]}[2]);
}

my @image;

# there are three images
#  roof first
for my $y (0 .. int(H_SIZE)) {
  my $t = ($y - H_SIZE) / H_SIZE;
  for my $x ($y .. int(H_SIZE)) {
    my $s = ($x - H_SIZE) / H_SIZE;
    # trace a ray from 0, 0, 0 through the unit sphere onto the edge of the box,
    #  and determine which angle it passed through unit sphere
    my (undef, undef, $phi) = cartesian_to_spherical($s, $t, 1);
    my $color = interpolate($phi);

    $image[$y][$x] = $color;
    $image[$y][SIZE - $x] = $color;
    $image[SIZE - $y][$x] = $color;
    $image[SIZE - $y][SIZE - $x] = $color;

    if ($x != $y) {
      $image[$x][$y] = $color;
      $image[$x][SIZE - $y] = $color;
      $image[SIZE - $x][$y] = $color;
      $image[SIZE - $x][SIZE - $y] = $color;
    }
  }
}
dump_face('UP', \@image);

# now in front
for my $z (0 .. SIZE) {
  # z scans from top to bottom though
  my $t = (H_SIZE - $z) / H_SIZE;
  for my $x (0 .. int(H_SIZE)) {
    my $s = ($x - H_SIZE) / H_SIZE;

    #  would usually have to do 4 takes but all 4 sides are identical with this gradient tool
    my (undef, undef, $phi) = cartesian_to_spherical($s, 1, $t);
    my $color = interpolate($phi);

    $image[$z][$x] = $color;
    $image[$z][SIZE - $x] = $color;
  }
}
dump_face('FRONT', \@image);

# finally, looking down
for my $y (0 .. int(H_SIZE)) {
  my $t = ($y - H_SIZE) / H_SIZE;
  for my $x ($y .. int(H_SIZE)) {
    my $s = ($x - H_SIZE) / H_SIZE;

    my (undef, undef, $phi) = cartesian_to_spherical($s, $t, -1);
    my $color = interpolate($phi);

    $image[$y][$x] = $color;
    $image[$y][SIZE - $x] = $color;
    $image[SIZE - $y][$x] = $color;
    $image[SIZE - $y][SIZE - $x] = $color;

    if ($x != $y) {
      $image[$x][$y] = $color;
      $image[$x][SIZE - $y] = $color;
      $image[SIZE - $x][$y] = $color;
      $image[SIZE - $x][SIZE - $y] = $color;
    }
  }
}
dump_face('DOWN', \@image);
