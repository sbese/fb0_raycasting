#include "fb_buf.h"

fb_buf::fb_buf(/* args */)
{
    int i;
    /* Открываем видеопамять */
    if ((fbfd = open("/dev/fb0", O_RDWR)) < 0) 
    {
        printf("unable to open fb0");
        exit(1);
    }
 
    /* Получаем изменяемые параметры изображения */
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) 
    {
        printf("Bad vscreeninfo ioctl\n");
        exit(2);
    }
 
    /* Размер кадрового буфера =
    (разрешение по X * разрешение по Y * байты на пиксель) */
    fbsize = vinfo.xres_virtual*vinfo.yres_virtual*(vinfo.bits_per_pixel/8);
 
    /* Отображаем видеопамять */
    if ((fbbuf = (unsigned char *) mmap(0, fbsize, PROT_READ|PROT_WRITE,
        MAP_SHARED, fbfd, 0)) == (void *) -1)
    {
            printf("bad mmap");
            exit(3);
    }

    fb_black_buf = new u_char[fbsize];
    temp_buf = new u_char[fbsize];
    memset(fb_black_buf,0,fbsize);

    clear_screen();
}

fb_buf::~fb_buf()
{
    munmap(fbbuf, fbsize);
    close(fbfd);
    delete fb_black_buf;
    delete temp_buf;
}

void fb_buf::set_pixel(int x, int y, u_char r, u_char g, u_char b)
{
    if(vinfo.yres_virtual > y && vinfo.xres_virtual > x)
    {
        *(temp_buf + ((y*vinfo.yres_virtual + x) * (vinfo.bits_per_pixel/8))) = b;
        *(temp_buf + ((y*vinfo.yres_virtual + x) * (vinfo.bits_per_pixel/8))+1) = g;
        *(temp_buf + ((y*vinfo.yres_virtual + x) * (vinfo.bits_per_pixel/8))+2) = r;
    }
}

void fb_buf::set_pixel(int x, int y, fb_color color)
{
    set_pixel(x,y,color.r,color.g,color.b);
}


void fb_buf::draw_line(int x1, int y1, int x2, int y2, u_char r, u_char g, u_char b) 
{
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    set_pixel(x2, y2, r, g, b);
    while(x1 != x2 || y1 != y2) 
   {
        set_pixel(x1, y1, r, g, b);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY) 
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
    }

}

void fb_buf::draw_line(int x1, int y1, int x2, int y2, fb_color color)
{
    draw_line(x1,y1,x2,y2,color.r,color.g,color.b);
} 


void fb_buf::draw_circle(int x0, int y0, int radius, u_char r, u_char g, u_char b) 
{
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while(y >= 0) 
    {
		set_pixel(x0 + x, y0 + y, r, g, b);
		set_pixel(x0 + x, y0 - y, r, g, b);
		set_pixel(x0 - x, y0 + y, r, g, b);
		set_pixel(x0 - x, y0 - y, r, g, b);

		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) 
        {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) 
        {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void fb_buf::draw_circle(int x0, int y0, int radius, fb_color color) 
{
    draw_circle(x0,y0,radius,color.r,color.g,color.b);
}

void fb_buf::clear_screen()
{
    memcpy(temp_buf,fb_black_buf,fbsize);
}

void fb_buf::show()
{
    memcpy(fbbuf,temp_buf,fbsize);
}
void fb_buf::fill_rectangle(int x1,int y1, int x2,int y2, u_char r, u_char g, u_char b)
{
    int px1,px2,py1,py2;

    px1 = std::min(x1,x2);
    px2 = std::max(x1,x2);

    py1 = std::min(y1,y2);
    py2 = std::max(y1,y2);

    if (px1<0) px1 = 0;
    if (px2<0) px2 = 0;
    if (py1<0) py1 = 0;
    if (py2<0) py2 = 0;

    if (px1>vinfo.xres) px1 = vinfo.xres;
    if (px2>vinfo.xres) px2 = vinfo.xres;
    if (py1>vinfo.yres) py1 = vinfo.yres;
    if (py2>vinfo.yres) py2 = vinfo.yres;

    for(int i=py1; i<=py2; ++i)
    {
        draw_line(px1,i,px2,i,r,g,b);
    }
}

void fb_buf::fill_rectangle(int x1,int y1, int x2,int y2, fb_color color) 
{
    fill_rectangle(x1,x1,x2,y2,color.r,color.g,color.b);
}

void fb_buf::draw_rectangle(int x1,int y1, int x2,int y2, u_char r, u_char g, u_char b)
{
    int px1,px2,py1,py2;

    px1 = std::min(x1,x2);
    px2 = std::max(x1,x2);

    py1 = std::min(y1,y2);
    py2 = std::max(y1,y2);

    if (px1<0) px1 = 0;
    if (px2<0) px2 = 0;
    if (py1<0) py1 = 0;
    if (py2<0) py2 = 0;

    if (px1>vinfo.xres) px1 = vinfo.xres;
    if (px2>vinfo.xres) px2 = vinfo.xres;
    if (py1>vinfo.yres) py1 = vinfo.yres;
    if (py2>vinfo.yres) py2 = vinfo.yres;

    draw_horisontal_line(px1,px1,py2,r,g,b);
    draw_horisontal_line(px1,px2,py1,r,g,b);
    draw_horisontal_line(px2,px1,py2,r,g,b);
    draw_horisontal_line(px2,px2,py1,r,g,b);
}

void fb_buf::draw_rectangle(int x1,int y1, int x2,int y2, fb_color color) 
{
    draw_rectangle(x1,x1,x2,y2,color.r,color.g,color.b);
}

void fb_buf::fill_circle(int x0, int y0, int radius, u_char r, u_char g, u_char b) 
{
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while(y >= 0) 
    {

        draw_horisontal_line(x0 + x, x0 - x, y0 + y, r, g, b);
        draw_horisontal_line(x0 + x, x0 - x, y0 - y, r, g, b);


		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) 
        {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) 
        {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void fb_buf::fill_circle(int x0, int y0, int radius, fb_color color) 
{
    fill_circle(x0,y0,radius,color.r,color.g,color.b);
}

void fb_buf::draw_horisontal_line(int x1, int x2, int y, u_char r, u_char g, u_char b)
{

    if (x1<0) x1 = 0;
    if (x2<0) x2 = 0;
    if (y <0) y  = 0;

    if (x1>vinfo.xres) x1 = vinfo.xres;
    if (x2>vinfo.xres) x2 = vinfo.xres;
    if (y >vinfo.yres) y  = vinfo.yres;

    if (x1 > x2)
    {
        std::swap(x1, x2);
    }

    int line_size = std::abs(x1-x2);
    if (line_size==0)
    {
        return;
    }
    u_char pixel[4] = {b,g,r,0};
    u_int32_t pix = ((int)(pixel[0])<<24) + ((int)(pixel[1])<<16) + ((int)(pixel[2])<<8) + pixel[3];//b<<24+g<<16+r<<8;
    u_char* ucp_line = new u_char[line_size*4];

    for(int i = 0; i < line_size; ++i)
    {
        memcpy((ucp_line+sizeof(int)*i), &pix, sizeof(int32_t));
    }

    memcpy(this->temp_buf+(y*vinfo.yres_virtual*4+x1*4)-1, ucp_line, line_size*4);
}