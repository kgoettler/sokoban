/*
 * Extending demo02.c, now have the ball push a "box" around
 * but define the ball and box as a struct
 */

#include <curses.h>
#include <stdlib.h>
#include "sokoban.h"


enum {
    OPEN,
    WALL,
    BOX,
    PLAYER
};

char * map = "##########\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "#  PB    #\n"
                   "#   B    #\n"
                   "#        #\n"
                   "#        #\n"
                   "#        #\n"
                   "##########\n";

int read_map(sokoban_t * game, char * map)
{
    char *s;
    int line_width, width, height, pos;
    
    /* Get map dimensions */
    line_width = 0;
    width = 0;
    height = 0;
    for (s = map; *s != '\0' ; s++) {
        line_width++; 
        printf("%c", *s);
        if (*s == '\n')
        {
            height++;
            if (line_width > width)
            {
                width = line_width;
            }
            line_width = 0;
        }
    }
    game->width = width;
    game->height = height;
    game->map = malloc(sizeof(int) * height * width);
    for (s = map, pos = 0; *s != '\0'; s++, pos++) {
        switch (*s)
        {
            case ' ':
                game->map[pos] = OPEN;
                break;
            case '#':
                game->map[pos] = WALL;
                break;
            case 'P':
                game->map[pos] = PLAYER;
                break;
            case 'B':
                game->map[pos] = BOX;
                break;
            case '\n':
                game->map[pos] = OPEN;
                break;
            default:
                fprintf(stderr, "invalid character in map: \"%c\"\n", *s);
                return 1;
        }
    }
    for (pos = 0; pos < game->width * game->height; pos++)
    {
        printf("%d", game->map[pos]);
        if ((pos+1) % game->width == 0)
            printf("\n");
    }
    printf("\n");
    return 0;
}

void draw_map(sokoban_t * game)
{
    int left, top, x, y, i;
    /* Determine where to start drawing */
    left = (COLS - game->width) / 2;
    top = (LINES - game->height) / 2;
    for (i = 0; i < game->width * game->height; i++)
    {
        x = left + (i % game->width);
        y = top + i / game->width;
        printf("%d %d\n", x, y);
        switch (game->map[i])
        {
            case OPEN:
                /*mvaddstr(y, x, " ");*/
                break;
            case WALL:
                mvaddstr(y, x, "#");
                break;
            case BOX:
                mvaddstr(y, x, "x");
                break;
            case PLAYER:
                mvaddstr(y, x, "o");
                break; 
        }
    }
    return;
}

int main (void)
{
    /* Initialize variables */
    sokoban_t game;
    int key, res;
    
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

    /* Read map */
    res = read_map(&game, map);
    draw_map(&game);

    /* Draw + wait for input */
    mvaddstr(game.player.y, game.player.x, "o");
    mvaddstr(game.box.y, game.box.x, "@");
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
        draw_map(&game);
    }
    /* Free up variables */
    free(game.map);
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
