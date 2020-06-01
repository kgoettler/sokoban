#ifndef SOKOBAN_H
#define SOKOBAN_H

enum {
    OPEN,
    WALL,
    BOX,
    GOAL,
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

/* Prototypes */
int read_map(sokoban_t * game, char * map);
void draw_map(sokoban_t * game);
void resolve_move(sokoban_t * game, int move);
int check_pos(sokoban_t * game, int pos);
int terminate(sokoban_t * game, int code);
int setup (void);
int play(sokoban_t * game);

#endif
