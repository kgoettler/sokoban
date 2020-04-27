/*
 * Simple ncurses app to move a ball around the screen with the keyboard
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
    
    // Add ball
    int x = 0, y = 0;
    mvprintw(y, x, "o");

    // Process input
    int key = 0;
    while ((key = getch()) != 'q')
    {
        if (key == KEY_LEFT)
            x -= 1;
        else if (key == KEY_RIGHT)
            x += 1;
        else if (key == KEY_UP)
            y -= 1;
        else if (key == KEY_DOWN)
            y += 1;
        
        // Check x,y
        if (x < 0)
            x = 0;
        if (x >= COLS)
            x = COLS - 1;
        if (y < 0)
            y = 0;
        if (y >= LINES)
            y = LINES - 1;

        // Redraw
        clear();
        mvprintw(y, x, "o");
    }
    endwin();
    return 0;
}
