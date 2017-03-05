#include <X11/Xlib.h>
#include <bits/stdc++.h>
#include "/home/dima/C++/debug.h"
using namespace std;

Display *display;
Window window;
int screen;

void draw() {
    GC gc = DefaultGC(display, screen);
//    XFillRectangle(display, window, gc, 20, 20, 10, 10);

    int x = 200;
    int y = 200;
//    XColor red, brown, blue, yellow, green;
//    assert(XAllocNamedColor(display, screen_colormap, "red", &red, &red));
//    XSetForeground(display, gc, red.pixel);
    XDrawPoint(display, window, gc, x, y);
}

int main(void) {
    display = XOpenDisplay(nullptr);
    assert(display != nullptr);
    screen = DefaultScreen(display);

    int displayWidth = DisplayWidth(display, screen);
    int displayHeight = DisplayHeight(display, screen);
    int width = displayWidth / 3;
    int height = displayHeight / 3;
    int x = (displayWidth - width) / 2;
    int y = (displayHeight - height) / 2;

    window = XCreateSimpleWindow(display, RootWindow(display, screen), x, y, width, height, 1, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);

    while (true) {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == Expose) {
            draw();
        }
    }

    XCloseDisplay(display);
    return 0;
}