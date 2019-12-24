// #include "fb_buf.h"
#include <unistd.h>
#include "camera.h"
#include <iostream>


struct point
{
    int x,y;
};

std::vector<point> path;

int main(int argc, char const *argv[])
{
    fb_buf fb_test;
    fb_color fbc;
    Wall w1(400,0,400,300,fb_test);
    Wall w2(500,300,500,600,fb_test);
    Wall w3(600,0,600,300,fb_test);
    Wall ww1(0,0,0,600,fb_test);
    Wall ww2(0,0,800,0,fb_test);
    Wall ww3(800,600,0,600,fb_test);
    Wall ww4(800,600,800,0,fb_test);
    Camera cam(510,200,20,M_PI/2,0,fb_test);
    // Ray test_ray(300,0,0,fb_test);
    // cros_point_data cp;

    // test_ray.draw_line(test_ray.x + (test_ray.dir_x*100),test_ray.y + (test_ray.dir_y*100));
    for(int i = 0; i<6000; ++i)
    {
        fb_test.clear_screen();
        w1.draw_wall();
        w2.draw_wall();
        w3.draw_wall();
        // cam.move_to(300,i);
        cam.rotate(M_PI/200/3);
        fb_test.fill_circle(cam.x,cam.y,10,fb_color::fb_green);
        cam.ray_cast(std::vector<Wall>{w1,w2,w3,ww1,ww2,ww3,ww4});
        fb_test.show();
        usleep(16000);
    }
}




























// {
//     fb_buf fb_test;
//     fb_color fbc;

//     // fb_test.draw_horisontal_line(100,500,300);
//     // for (int i = 10; i<256;i++)
//     // fb_test.set_pixel(i,i,fb_color::fb_red);
//     // fb_test.draw_line(500,0,0,500);
//     // fb_test.draw_line(100,200,300,400,fb_color::fb_green);
//     // fb_test.draw_line(0,50,200,300,0,100,200);
//     // fb_test.draw_circle(400,400,100);
//     // fb_test.draw_circle(400,400,50,200,100,50);
//     // fb_test.draw_circle(400,400,150,fb_color::fb_blue);
//     // fb_test.fill_rectangle(100,100,200,200);
//     // fb_test.fill_rectangle(200,200,300,300,100,200,150);
//     // fb_test.fill_rectangle(300,300,400,200,fb_color::fb_green);
//     // fb_test.draw_rectangle(100,100,300,300);
//     // fb_test.draw_rectangle(200,200,300,300,200,200,50);
//     // fb_test.draw_rectangle(300,300,400,200,fb_color::fb_blue);
//     // fb_test.fill_circle(400,100,255,fb_color::fb_blue);

    // while (1)
    // {
    //     for(int i =1;i<255;i++)
    //     {
            
    //         for(int j=i;j>0;j--)
    //         {
    //             fb_test.fill_circle(200,200,j,255-j,255-j,255-j);
    //         }
    //         fb_test.show();
    //         usleep(8000);
    //     }
    //     fb_test.clear_screen();
    // }
    
    // int r = 100.0;
    // int pi = 3;
    // int x1,x2,y1,y2;
    // for(int i=1; i<2*r*pi; i++)
    // {
    //     fb_test.clear_screen();
    //     x1 = i+100;
    //     y1 = 300;
    //     x2 = (int) (-cos((float)i/(float)r)*r+x1);
    //     y2 = (int) (-sin((float)i/(float)r)*r+y1);
    //     path.push_back(point{x2,y2});
    //     fb_test.draw_circle(x1,y1,r);
    //     fb_test.draw_line(x1,y1,x2,y2);
    //     for(int j=0;j<path.size();++j)
    //     {
    //         if(j!=0)
    //         {
    //             fb_test.draw_line(path[j-1].x,path[j-1].y,path[j].x,path[j].y);
    //         }
    //     }
    //     fb_test.show();
    //     usleep(32000);
    // }
    // for(int i = 0; i<256; i++)
    // {
    //     int j = 100;
    //     fb_test.clear_screen();
    //     fb_test.fill_rectangle(100+i,100+i,200+i+j,200+i+j,256-i,i,256-i);
    //     fb_test.show();
    //     usleep(16000);
    // }
    // std::cin;
    // return 0;
// }