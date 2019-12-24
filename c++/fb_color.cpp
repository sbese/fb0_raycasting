#include "fb_buf.h"

fb_color::fb_color(u_char r, u_char g, u_char b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

fb_color fb_color::fb_red   = fb_color(255, 0, 0);
fb_color fb_color::fb_green = fb_color(0, 255, 0);
fb_color fb_color::fb_blue  = fb_color(0, 0, 255);
fb_color fb_color::fb_black = fb_color(0, 0, 0);