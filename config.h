/* ttt configuration. Rebuild after editing: make clean install */

/* 256-color palette indexes */
static const int wedgecolor = 196; /* remaining-time wedge */
static const int rimcolor   = 250; /* dial outline and tick marks */
static const int textcolor  = 250; /* mm:ss readout */

/* horizontal stretch; raise above 1.0 if the dial looks tall and
 * narrow in your font, lower if it looks squashed */
static const double xscale = 1.0;

/* run when the timer reaches zero */
static const char *alarmcmd =
	"ttt-flash & pw-play /usr/share/sounds/freedesktop/stereo/alarm-clock-elapsed.oga >/dev/null 2>&1 &";
