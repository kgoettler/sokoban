/*
 * Extending demo02.c, now have the ball push a "box" around
 * but define the ball and box as a struct
 */

#include <curses.h>

struct element {
    int x;
    int y;
};

void check_boundary(struct element * s);
void move_left(struct element * s);
void move_right(struct element * s);
void move_up(struct element * s);
void move_down(struct element * s);
int same_position(struct element * s1, struct element * s2);
void set_position(struct element * s, int x, int y);

int main (void)
{
    /* Initialize variables */
    struct element ball;
    struct element box;
    int key;

    /* Initialize ncurses session */
    initscr();
    /* Do not echo character input to screen */
    noecho();
    /* Hide cursor */
    curs_set(0);
    /* Accept function keys/arrow keys */
    keypad(stdscr, TRUE);
    
    /* Assign variables */
    ball.x = COLS / 2;
    ball.y = LINES / 2;
    box.x = COLS / 2 + 1;
    box.y = LINES / 2;
    key = 0;

    /* Draw + wait for input */
    mvprintw(ball.y, ball.x, "o");
    mvprintw(box.y, box.x, "@");
    /* Process input */
    while ((key = getch()) != 'q')
    {
        /* Change ball position */
        switch (key)
        {
            case ('h'):
            case (KEY_LEFT):
                move_left(&ball);
                if (same_position(&ball, &box) == 0)
                {
                    if (ball.x != 0)
                        move_left(&box);
                    else
                        move_right(&ball);
                }
                break;
            case ('l'):
            case (KEY_RIGHT):
                move_right(&ball);
                if (same_position(&ball, &box) == 0)
                {
                    if (ball.x != COLS - 1)
                        move_right(&box);
                    else
                        move_left(&ball);
                }
                break;
            case ('k'):
            case (KEY_UP):
                move_up(&ball);
                if (same_position(&ball, &box) == 0)
                {
                    if (ball.y != 0)
                        move_up(&box);
                    else
                        move_down(&ball);
                }
                break;
            case ('j'):
            case (KEY_DOWN):
                move_down(&ball);
                if (same_position(&ball, &box) == 0)
                {
                    if (ball.y != LINES - 1)
                        move_down(&box);
                    else
                        move_up(&ball);
                }
                break;
            case ('r'):
                ball.x = COLS / 2;
                ball.y = LINES / 2;
                box.x = ball.x + 1;
                box.y = ball.y;
            default:
                break;
        }
        /* Check x,y */
        check_boundary(&ball);

        /* Redraw */
        clear();
        mvprintw(ball.y, ball.x, "o");
        mvprintw(box.y, box.x, "@");
    }
    endwin();
    return 0;
}

void check_boundary(struct element * s)
{
    if (s->x < 0)
        s->x = 0;
    if (s->x >= COLS)
        s->x = COLS - 1;
    if (s->y < 0)
        s->y = 0;
    if (s->y >= LINES)
        s->y = LINES - 1;
    return;
}

int same_position(struct element * s1, struct element * s2)
{
    if ((s1->x == s2->x) && (s1->y == s2->y))
        return 0;
    return 1;
}

void move_left(struct element * s)
{
    s->x -= 1;
    return;
}

void move_right(struct element * s)
{
    s->x += 1;
    return;
}

void move_up(struct element * s)
{
    s->y -= 1;
    return;
}

void move_down(struct element * s)
{
    s->y += 1;
    return;
}

void set_position(struct element * s, int x, int y)
{
    s->x = x;
    s->y = y;
    return;
}
