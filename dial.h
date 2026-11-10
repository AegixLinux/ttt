/* dial.h: pure geometry for the ttt dial, shared by ttt.c and tests */
#ifndef DIAL_H
#define DIAL_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Fraction of a full turn, counterclockwise from 12 o'clock, in [0, 1).
 * (dx, dy) is a point relative to the dial center, screen y grows down.
 * Matches the physical Time Timer: the wedge extends counterclockwise. */
static double
angfrac(double dx, double dy)
{
	double a = atan2(-dx, -dy);

	if (a < 0)
		a += 2 * M_PI;
	return a / (2 * M_PI);
}

/* 1 if the point lies inside the remaining-time wedge.
 * frac is remaining time as a fraction of the full dial (0 to 1). */
static int
in_wedge(double dx, double dy, double frac)
{
	if (frac <= 0)
		return 0;
	if (frac >= 1)
		return 1;
	return angfrac(dx, dy) <= frac;
}

#endif
