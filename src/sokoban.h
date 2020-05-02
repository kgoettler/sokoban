struct element {
    int x;
    int y;
};

typedef struct sokoban_t {
    struct element player;
    struct element box;
    int height;
    int width;
    int * map;
} sokoban_t;

void check_boundary(struct element * s);
void move_left(struct element * s);
void move_right(struct element * s);
void move_up(struct element * s);
void move_down(struct element * s);
int same_position(struct element * s1, struct element * s2);
void set_position(struct element * s, int x, int y);
