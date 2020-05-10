/*
 * Extending demo02.c, now have the ball push a "box" around
 * but define the ball and box as a struct
 */

#include <curses.h>
#include <stdlib.h>

enum {
    OPEN,
    WALL,
    BOX,
    PLAYER
};

enum {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
};

typedef struct sokoban_t {
    int pos;
    int height;
    int width;
    int size;
    int * map;
} sokoban_t;

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

/*
char * map = "      #####\n"
             "      #   #\n"
             "      #   #\n"
             "      #   #\n"
             "      #   #\n"
             "#######   #\n"
             "#         #\n"
             "#  P  B   #\n"
             "#     B   #\n"
             "#         #\n"
             "###########\n";
*/

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
    game->size = width * height;
    game->map = malloc(sizeof(int) * game->size);
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
                game->pos = pos;
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

void resolve_move(sokoban_t * game, int move)
{
    int dxy, next_pos, next_elem;
    switch (move)
    {
        case MOVE_LEFT:
            dxy = -1;
            break;
        case MOVE_RIGHT:
            dxy = 1;
            break;
        case MOVE_UP:
            dxy = -game->width;
            break;
        case MOVE_DOWN:
            dxy = game->width;
            break;
    }
    /* Check that next position exists */
    next_pos = game->pos + dxy;
    if (check_pos(game, next_pos))
        terminate(game, 1);
    next_elem = game->map[next_pos];
    
    /* Check for wall */
    if (next_elem != WALL)
    {
        if (next_elem == BOX)
        {
            if (check_pos(game, next_pos + dxy)) 
                terminate(game, 1);
            if (game->map[next_pos+dxy] == OPEN)
            {
                game->map[next_pos+dxy] = BOX;
                game->map[next_pos] = PLAYER;
                game->map[game->pos] = OPEN;
                game->pos = next_pos;
            }
        }
        else
        {
            if (check_pos(game, next_pos)) 
                terminate(game, 1);
            game->map[next_pos] = PLAYER;
            game->map[game->pos] = OPEN;
            game->pos = next_pos;
        }
    }
    return;
}

int check_pos(sokoban_t * game, int pos)
{
    if (pos < 0 || pos > game->size)
        return 1;
    return 0;
}

int terminate(sokoban_t * game, int code)
{
    free(game->map);
    endwin();
    if (code == 1)
    { 
        printf("Fatal error. " 
               "Attempted to access memory beyond bounds of map.\n"
               "(Is your map not fully enclosed by walls?)\n");
        exit(code);
    }
    return 0;
}

int setup (void)
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    return 0;
}

int play(sokoban_t * game)
{
    int key;
    draw_map(game);
    /* Process input */
    while ((key = getch()) != 'q')
    {
        /* Change game.player position */
        switch (key)
        {
            case ('h'):
            case (KEY_LEFT):
                resolve_move(game, MOVE_LEFT);
                break;
            case ('l'):
            case (KEY_RIGHT):
                resolve_move(game, MOVE_RIGHT);
                break;
            case ('k'):
            case (KEY_UP):
                resolve_move(game, MOVE_UP);
                break;
            case ('j'):
            case (KEY_DOWN):
                resolve_move(game, MOVE_DOWN);
                break;
            case ('r'):
            default:
                break;
        }
        /* Redraw */
        clear();
        draw_map(game);
    }
    return 0;
}

int main (void)
{
    sokoban_t game;
    /* Initialize ncurses session */
    setup();
    /* Read + draw map */
    read_map(&game, map);
    /* Loop */
    play(&game);
    /* Free up variables */
    terminate(&game, 0);
    return 0;
}
