/*
 * @url: https://people.eecs.berkeley.edu/~bh/pdf/v2ch14.pdf
 * @lang: c89
 * @taglist: graphics
 *
 * A machine using a polar, relative based coordinate system, with optional
 * wrap-around.
 *
 * Screen boundaries are set so that (-100, -100) .. (100, 100) is fully visible
 * on the screen, with correct aspect ratio.
 *
 * Headings in degrees are measured clockwise from the positive Y up axis, which
 * differs from mathematical convention of measuring angles from the positive X
 * axis, counter-clockwise.
 *
 * Seymour Papert introduced the turtle in the Logo language in the late 1960s
 */

struct Turtle;

/*
 * Motion
 * ------
 */

/* Moves the turtle forward, in the direction that it's facing by the specified
 * distance. */
void turtle_forward(struct Turtle *turtle, double distance);
#define turtle_fd turtle_forward

/* Moves the turtle backward, i.e. exactly opposite to the direction it's facing
 * by the specified distance. */
void turtle_back(struct Turtle *turtle, double distance);
#define turtle_bk turtle_back

/* Turn the turtle counterclockwise by the specified angle, measured in degrees.
 * (1/360 of a circle) */
void turtle_left(struct Turtle *turtle, double degrees);
#define turtle_lt turtle_left

/* Turn the turtle clockwise by the specified angle measured in degrees. (1/360
 * of a circle) */
void turtle_right(struct Turtle *turtle, double degrees);
#define turtle_rt turtle_right

/* Moves the turtle to an absolute screen position. */
void turtle_setxy(struct Turtle *turtle, double x, double y);

/* Moves the turtle horizontally from its old position to a new absolute
 * horizontal coordinate. */
void turtle_setx(struct Turtle *turtle, double x);

/* Moves the turtle vertically from its old position to a new absolute
 * horizontal coordinate. */
void turtle_sety(struct Turtle *turtle, double y);

/* Turn the turtle to a new absolute heading. */
void turtle_setheading(struct Turtle *turtle, double degrees);
#define turtle_seth turtle_setheading

/* Moves the turtle to the center of the screen, equivalent of setxy(0, 0) */
void turtle_home(struct Turtle *turtle);

/*
 * Motion Queries
 * --------------
 */

struct TurtleXY {
    double x, y;
};

/* Outputs the turtle's current position as x,y coordinates. */
struct TurtleXY turtle_pos(struct Turtle const *turtle);

/* Outputs the turtle's heading in degrees. */
double turtle_heading(struct Turtle const *turtle);

/* Outputs the heading the turtle should be facing so that it would point from
 * its current position to the position given as argument. */

double turtle_towards(struct Turtle const *turtle, double pos);

/* Outputs the x,y scrunch factors. @see: turtle_setscrunch */
struct TurtleXY turtle_scrunch(struct Turtle const *turtle);

/*
 * Turtle & Display control
 * ------------------------
 */

/* Makes the turtle visible. */
void turtle_show(struct Turtle *turtle);

/* Makes the turtle invisible. */
void turtle_hide(struct Turtle *turtle);

/* Erases all lines that the turtle has drawn on the graphics window the turtle
 * state is not changed. */
void turtle_clean(struct Turtle const *turtle);

/* Synonym of { home ; clean }. */
void turtle_clearscreen(struct Turtle *turtle);

/* Tells the turtle to enter wrap mode, from now on, if the turtle is asked to
 * move past the boundary of the graphics window, it will wrap around and
 * reappear at the opposite edge. This is the default mode. @see{turtle_window}
 * @see{turtle_fence} */
void turtle_wrap(struct Turtle *turtle);

/* Tells the turtle to enter window mode: from now on, if the turtle is asked to
 * move past the boundary of the graphics window, it will move offscreen.
 * @see{turtle_wrap} @see{turtle_fence} */
void turtle_window(struct Turtle *turtle);

/* Tells the turtle to enter fence mode: from now on, it will move as far as it
 * can and then stop at the edge with an out of bounds error. @see{turtle_wrap}
 * @see{turtle_window} */
void turtle_fence(struct Turtle *turtle);

/* Prints the text starting at the turtle's position. */
void turtle_label(struct Turtle *turtle, char const *text_first, size_t text_n);

/* Adjusts the aspect ratio and scaling of the graphics display. After this
 * command is used, all further turtle motion will be adjusted by multiplying
 * the horizontal and vertical extent of the motion by the two numbers given as
 * inputs. For example after the instruction turtle_setcrunch(turtle, 2, 1),
 * motion at a 45 degrees will move twice as far horizontally as vertically. */
void turtle_setscrunch(struct Turtle *turtle, double xscale, double yscale);

/*
 * Turtle and Window Queries
 * -------------------------
 */

/* Outputs true if the turtle is shown (visible) or false if the turtle is
 * hidden. @see{turtle_show} @see{turtle_hide} */
int turtle_shownp(struct Turtle const *turtle);

/*
 * Pen and Background control
 * --------------------------
 *
 * The turtle carries a pen that can draw. At any time the pen can be up (in
 * which case moving the turtle does not change what's on the screen) or down
 * (in which case the turtle leaves a trace)
 */

void turtle_pendown(struct Turtle *turtle);
#define turtle_pd turtle_pendown

void turtle_penup(struct Turtle *turtle);
#define turtle_pu turtle_penup

enum TurtleColor {
    TurtleColor_Black = 0,
    TurtleColor_System1,
    TurtleColor_System2,
    TurtleColor_System3,
    TurtleColor_System4,
    TurtleColor_System5,
    TurtleColor_System6,
    TurtleColor_White = 7,
    TurtleColor_User0,
    TurtleColor_UserLast = 65536,
};

/* Sets the pen color to the given color. System colors may not be consistent
 * between machines. */
void turtle_setpencolor(struct Turtle *turtle, enum TurtleColor color);

struct TurtleRGBA {
    char rgba[4];
};

/* Sets the actual color to the given RGBA tuple. */
void turtle_setpalette(struct Turtle *turtle,
                       enum TurtleColor color,
                       struct TurtleRGBA rgba);

/*
 * Pen Queries
 * -----------
 */

/* Outputs true if the pen is down, false if it's up. */
int turtle_pendownp(struct Turtle const *turtle);

/* Outputs the current pen color. */
enum TurtleColor turtle_pencolor(struct Turtle const *turtle);
