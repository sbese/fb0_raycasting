#include "fb_buf.h"
#include <vector>

class Ray;
class Wall;
class Camera;

struct cros_point_data
{
    bool is_point;
    double x,y;
};


class Ray
{
private:
    fb_buf *buf;
public:
    double dir_x,dir_y,angle;
    int x,y;
    Ray(int _x, int _y, double _angle, fb_buf &_buf);
    ~Ray();
    void set_dir(double _x, double _y);
    void rotate(double _angle);
    void set_cords(int _x, int _y);
    void draw_line(int _x, int _y);
    cros_point_data cros_point(Wall &wall);
};

class Wall
{
private:
    fb_buf *buf;
public:
    int ax,ay,bx,by;
    Wall(int _ax, int _ay, int _bx, int _by, fb_buf &_buf);
    ~Wall(){};
    void draw_wall();
};

class Camera
{
private:
    double view_angle, rotation_angle;
    fb_buf *buf;
    std::vector <Ray> rays;

public:
    int count_of_rays, x, y;
    Camera(int _x, int _y, int _count_of_rays, 
           double _view_angle, double _rotation_angle, fb_buf &_buf);
    ~Camera();
    void move(int dist);
    void move_to(int _x, int _y);
    void rotate(double _angle);
    double get_distance (double x1, double  y1, double  x2, double  y2);
    void ray_cast(std::vector<Wall> walls);
};