/*
 * Extending demo01.c, now have the ball push a "box" around
 */

#include <curses.h>

int main (void)
{
    // Initialize ncurses session
    initscr();
    // Do not echo character input to screen
    noecho();
    // Hide cursor
    curs_set(0);
    // Accept function keys/arrow keys
    keypad(stdscr, TRUE);
    
    // Add ball and box
    int xball = COLS / 2, yball = LINES / 2;
    int xbox = COLS / 2 + 1, ybox = LINES / 2;
    mvprintw(yball, xball, "o");
    mvprintw(ybox, xbox, "@");

    // Process input
    int key = 0;
    while ((key = getch()) != 'q')
    {
        // Change ball position
        //
        switch (key)
        {
            case ('h'):
            case (KEY_LEFT):
                xball -= 1;
                if (xball == xbox && yball == ybox)
                {
                    if (xball != 1)
                        xbox -= 1;
                    else
                        xball += 1;
                }
                break;
            case ('l'):
            case (KEY_RIGHT):
                xball += 1;
                if (xball == xbox && yball == ybox)
                {
                    if (xball != COLS - 1)
                        xbox += 1;
                    else
                        xball -= 1;
                }
                break;
            case ('k'):
            case (KEY_UP):
                yball -= 1;
                if (xball == xbox && yball == ybox)
                {
                    if (yball != 1)
                        ybox -= 1;
                    else
                        yball += 1;
                }
                break;
            case ('j'):
            case (KEY_DOWN):
                yball += 1;
                if (xball == xbox && yball == ybox)
                {
                    if (yball != LINES - 1)
                        ybox += 1;
                    else
                        yball -= 1;
                }
                break;
            case ('r'):
                xball = COLS / 2;
                yball = LINES / 2;
                xbox = xball + 1;
                ybox = yball;
            default:
                break;
        }
        // Check x,y
        if (xball < 0)
            xball = 0;
        if (xball >= COLS)
            xball = COLS - 1;
        if (yball < 0)
            yball = 0;
        if (yball >= LINES)
            yball = LINES - 1;

        // Redraw
        clear();
        mvprintw(yball, xball, "o");
        mvprintw(ybox, xbox, "@");
    }
    endwin();
    return 0;
}
