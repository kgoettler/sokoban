/*
 * Extending demo02.c, now have the ball push a "box" around
 * but define the ball and box as a struct
 */

#include <curses.h>
#include "sokoban.h"

const char * map = "##########\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "##########";

int main (void)
{
    /* Initialize variables */
    struct sokoban_t game;
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
    game.player.x = COLS / 2;
    game.player.y = LINES / 2;
    game.box.x = COLS / 2 + 1;
    game.box.y = LINES / 2;
    key = 0;

    /* Draw + wait for input */
    mvprintw(game.player.y, game.player.x, "o");
    mvprintw(game.box.y, game.box.x, "@");
    /* Process input */
    while ((key = getch()) != 'q')
    {
        /* Change game.player position */
        switch (key)
        {
            case ('h'):
            case (KEY_LEFT):
                move_left(&game.player);
                if (same_position(&game.player, &game.box) == 0)
                {
                    if (game.player.x != 0)
                        move_left(&game.box);
                    else
                        move_right(&game.player);
                }
                break;
            case ('l'):
            case (KEY_RIGHT):
                move_right(&game.player);
                if (same_position(&game.player, &game.box) == 0)
                {
                    if (game.player.x != COLS - 1)
                        move_right(&game.box);
                    else
                        move_left(&game.player);
                }
                break;
            case ('k'):
            case (KEY_UP):
                move_up(&game.player);
                if (same_position(&game.player, &game.box) == 0)
                {
                    if (game.player.y != 0)
                        move_up(&game.box);
                    else
                        move_down(&game.player);
                }
                break;
            case ('j'):
            case (KEY_DOWN):
                move_down(&game.player);
                if (same_position(&game.player, &game.box) == 0)
                {
                    if (game.player.y != LINES - 1)
                        move_down(&game.box);
                    else
                        move_up(&game.player);
                }
                break;
            case ('r'):
                game.player.x = COLS / 2;
                game.player.y = LINES / 2;
                game.box.x = game.player.x + 1;
                game.box.y = game.player.y;
            default:
                break;
        }
        /* Check x,y */
        check_boundary(&game.player);

        /* Redraw */
        clear();
        mvprintw(game.player.y, game.player.x, "o");
        mvprintw(game.box.y, game.box.x, "@");
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
