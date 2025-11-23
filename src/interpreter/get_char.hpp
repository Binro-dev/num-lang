#pragma once

#include <cstdio>

#if defined(_WIN32)
#include <conio.h>

inline int get_char() 
{
    return _getch();   
}

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

inline int get_char() 
{
    int tty = open("/dev/tty", O_RDONLY);
    if (tty == -1)
        return EOF;

    termios oldt, newt;
    tcgetattr(tty, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(tty, TCSANOW, &newt);

    unsigned char ch;
    int n = read(tty, &ch, 1); 

    tcsetattr(tty, TCSANOW, &oldt);
    close(tty);

    if (n < 1)
        return EOF;

    return ch;
}

#endif
