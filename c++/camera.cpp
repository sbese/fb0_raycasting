#include "camera.h"

Ray::Ray(int _x, int _y, double _angle, fb_buf &_buf)
{
    x = _x;
    y = _y;
    angle = _angle;
    dir_x = cos(angle);
    dir_y = sin(angle);
    buf = &_buf;

}
Ray::~Ray(){}

void Ray::set_dir(double _x, double _y)
{
    dir_x = _x;
    dir_y = _y;
}

void Ray::rotate(double _angle)
{
    angle=(angle + _angle);
    dir_x = cos(angle);
    dir_y = sin(angle);
}

void Ray::set_cords(int _x, int _y)
{
    x = _x;
    y = _y;
}
void Ray::draw_line(int _x, int _y)
{
    buf->draw_line(x,y,_x,_y);
}
cros_point_data Ray::cros_point(Wall &wall)
{
    double x1,x2,x3,x4;
    double y1,y2,y3,y4;
    double den,t,u;
    double pt_x, pt_y;

    x1 = wall.ax;
    y1 = wall.ay;
    x2 = wall.bx;
    y2 = wall.by;

    x3 = x;
    y3 = y;
    x4 = x + dir_x;
    y4 = y + dir_y;

    den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (den == 0)
    {
        return cros_point_data {false, NULL, NULL};
    }

    
    t =  ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;    

    if (t > 0 and t < 1 and u > 0)
    { 
        pt_x = x1 + t * (x2 - x1);
        pt_y = y1 + t * (y2 - y1);
        return cros_point_data {true, pt_x, pt_y};
    }
    else
    {
        return cros_point_data {false, NULL, NULL};
    }
}

Wall::Wall(int _ax, int _ay, int _bx, int _by, fb_buf &_buf)
{
    ax = _ax;
    ay = _ay;
    bx = _bx;
    by = _by;
    buf = &_buf;
}

void Wall::draw_wall()
{
    buf->draw_line(ax,ay,bx,by);
}


Camera::Camera(int _x, int _y, int _count_of_rays, double _view_angle, double _rotation_angle, fb_buf &_buf)
{
    x = _x;
    y = _y;
    count_of_rays = _count_of_rays;
    view_angle = _view_angle;
    rotation_angle = _rotation_angle;
    buf = &_buf;
    for(int i=0; i<count_of_rays; i++)
    {
        rays.push_back(Ray (x,
                            y,
                            rotation_angle-(view_angle/2)+(view_angle/(count_of_rays-1)*i),
                            _buf));        
    }

}

Camera::~Camera(){};

void Camera::move(int dist)
{
    x += cos(rotation_angle)*dist;
    y += sin(rotation_angle)*dist;
    for(int i = 0; i<rays.size(); ++i)
    {
        rays[i].set_cords(x,y);
    }
}

void Camera::move_to(int _x, int _y)
{
    x = _x;
    y = _y;
    for(int i = 0; i<rays.size(); ++i)
    {
        rays[i].set_cords(x,y);
    }
}

void Camera::rotate(double _angle)
{
    rotation_angle += _angle;
    for(int i=0; i<count_of_rays; i++)
    {
        rays[i].angle = rotation_angle-(view_angle/2)+(view_angle/(count_of_rays-1)*i);
        rays[i].dir_x = cos(rays[i].angle);
        rays[i].dir_y = sin(rays[i].angle); 
    }

}

double Camera::get_distance (double x1, double  y1, double  x2, double  y2)
{
    return pow((pow((x1-x2),2) + pow((y1-y2),2)),(1/2));
}

void Camera::ray_cast(std::vector<Wall> walls)
{
    double d,dx,dy,nd;
    Wall w=walls[0];
    cros_point_data cords;
    for(int i = 0; i<count_of_rays; ++i)
    {
        d = INFINITY;
        dx = 0; dy = 0;
        w = walls[0];
        for(int j=0; j<walls.size(); ++j)
        {
            cords = rays[i].cros_point(walls[j]);
            if(cords.is_point)
            {
                nd = get_distance(rays[i].x,rays[i].y,cords.x,cords.y);
                if(nd<d)
                {
                    d = nd;
                    w = walls[j];
                    dx = cords.x;
                    dy = cords.y;
                }
            }
        }
        if(d!= INFINITY)
        {
            rays[i].draw_line((int)dx,(int)dy);
        }
    }
}
