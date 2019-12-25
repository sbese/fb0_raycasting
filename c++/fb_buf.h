#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <cstring>
#include <cmath>
#include <algorithm> 


class fb_color;
class fb_buf;

class fb_buf
{
private:
    int fbfd, fbsize, fb;
    unsigned char *fbbuf, *fb_black_buf, *temp_buf;

public:
    fb_var_screeninfo vinfo;
    fb_buf(/* args */);
    ~fb_buf();
    void set_pixel(int x, int y, u_char r = 255, u_char g = 255, u_char b = 255);
    void set_pixel(int x, int y, fb_color);
    void draw_line(int x1, int y1, int x2, int y2, u_char r = 255, u_char g = 255, u_char b = 255);
    void draw_line(int x1, int y1, int x2, int y2, fb_color color);
    void draw_circle(int x0, int y0, int radius, u_char r = 255, u_char g = 255, u_char b = 255);
    void draw_circle(int x0, int y0, int radius, fb_color color);
    void clear_screen();
    void fill_rectangle(int x1,int y1, int x2,int y2, u_char r = 255, u_char g = 255, u_char b = 255);
    void fill_rectangle(int x1,int y1, int x2,int y2, fb_color color);
    void draw_rectangle(int x1,int y1, int x2,int y2, u_char r = 255, u_char g = 255, u_char b = 255);
    void draw_rectangle(int x1,int y1, int x2,int y2, fb_color color);
    void fill_circle(int x0, int y0, int radius, u_char r = 255, u_char g = 255, u_char b = 255);
    void fill_circle(int x0, int y0, int radius, fb_color color);
    void draw_horisontal_line(int x1, int x2, int y, u_char r = 255, u_char g = 255, u_char b = 255);
    void show();

};

class fb_color 
{
public:    
    u_char r,g,b;
    fb_color(u_char r = 255, u_char g = 255, u_char b = 255);
    ~fb_color() {};
    static fb_color fb_red  ;
    static fb_color fb_green;
    static fb_color fb_blue ;
    static fb_color fb_black;
};

