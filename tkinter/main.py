from tkinter import Tk, Canvas
import math
from PIL import Image
im = Image.open("P:\\fb0\\tkinter\\texture.png")


root = Tk()
wind_width = 500
wind_height = 500
texture = []
for x in range(64):
    if x % 4 in [0, 1]:
        texture.append('#000000')
    else:
        texture.append('#ffffff')


class Ray():
    def __init__(self, x, y, angle, canvas):
        self.x = x
        self.y = y
        self.angle = angle
        self.dir_x = math.cos(angle)
        self.dir_y = math.sin(angle)
        self.canvas = canvas
        self.line = self.canvas.create_line(0, 0, 0, 0)

    def set_dir(self, x, y):
        self.dir_x = x
        self.dir_y = y

    def set_cords(self, x, y):
        self.x = x
        self.y = y

    def draw_line(self, x, y):
        self.canvas.coords(self.line, self.x, self.y, x, y)

    def cros_point(self, wall):
        x1 = wall.ax
        y1 = wall.ay
        x2 = wall.bx
        y2 = wall.by

        x3 = self.x
        y3 = self.y
        x4 = self.x + self.dir_x
        y4 = self.y + self.dir_y

        den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
        if (den == 0):
            # c.coords(self.line,0,0,0,0)
            return (False, None, None)

        t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den
        u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den

        if (t > 0 and t < 1 and u > 0):
            pt_x = x1 + t * (x2 - x1)
            pt_y = y1 + t * (y2 - y1)
            return(True, pt_x, pt_y)
        else:
            # c.coords(self.line,0,0,0,0)
            return (False, None, None)


class Wall():
    def __init__(self, ax, ay, bx, by, canvas):
        self.ax = ax
        self.ay = ay
        self.bx = bx
        self.by = by
        self.canvas = canvas
        self.line = self.canvas.create_line(ax, ay, bx, by)
    
    def get_pixel_column(self, x, y):
        return int(math.sqrt((self.ax-x)**2+(self.ay-y)**2)) % 64
        


class Camera():
    def __init__(self, x, y, count_of_rays,
                 view_angle, rotation_angle, canvas):
        self.count_of_rays = count_of_rays
        self.view_angle = view_angle
        self.rotation_angle = rotation_angle
        self.canvas = canvas
        self.x = x
        self.y = y
        self.rays = [Ray(
            x,
            y,
            self.rotation_angle-self.view_angle/2 +
            (self.view_angle/(self.count_of_rays-1)*i),
            self.canvas)
            for i in range(self.count_of_rays)]

    def move_to(self, x, y):
        self.x = x
        self.y = y
        for ray in self.rays:
            ray.set_cords(x, y)

    def move(self, dist):
        self.x += math.cos(self.rotation_angle)*dist
        self.y += math.sin(self.rotation_angle)*dist
        for ray in self.rays:
            ray.x = self.x
            ray.y = self.y

    def rotate(self, angle):
        self.rotation_angle += angle

        for i, ray in enumerate(self.rays):
            ray.angle = (self.rotation_angle-self.view_angle/2) + (
                self.view_angle/(self.count_of_rays-1)*i)
            ray.dir_x = math.cos(ray.angle)
            ray.dir_y = math.sin(ray.angle)

    def get_distance(self, x1, y1, x2, y2):
        return ((x1-x2)**2+(y1-y2)**2)**(1/2)

    def ray_cast(self, wals):
        for ray in self.rays:

            d = math.inf
            dx = 0
            dy = 0
            w = wals[0]
            for wall in walls:
                cords = ray.cros_point(wall)
                if cords[0]:
                    nd = self.get_distance(ray.x, ray.y, cords[1], cords[2])
                    if nd < d:
                        d = nd
                        w = wall
                        dx = cords[1]
                        dy = cords[2]
            ray.draw_line(dx, dy)
            yield {"dist": d, "x": dx, "y": dy, "wall": w}


class Player:
    def __init__(self, render):
        self.render = render
        self.camera = render.camera
        self.render_it()

    def render_it(self):
        rr = list(self.camera.ray_cast(walls))
        self.render.render_3d(rr)
        self.render.render_buf(rr)

    def test(self, event):
        x, y = event.x, event.y
        self.camera.move_to(x, y)
        self.render_it()

    def rotate_left(self, event):
        self.camera.rotate(-math.pi/16)
        self.render_it()

    def rotate_right(self, event):
        self.camera.rotate(math.pi/16)
        self.render_it()

    def move_forvard(self, event):
        self.camera.move(10)
        self.render_it()

    def move_back(self, event):
        self.camera.move(-10)
        self.render_it()


class Render:
    def __init__(self, x, y, width, height, canvas, camera):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.camera = camera
        self.canvas = canvas
        self.rectangles = []
        self.rast_do_steny = 55

    def render_ez(self, intersection_points):
        for rec in self.rectangles:
            self.canvas.delete(rec)
        self.rectangles = []
        crossed_walls=[{"wall": intersection_points[0]["wall"],
                        "x1":intersection_points[0]["x"],
                        "y1":intersection_points[0]["y"],
                        "dist1":intersection_points[0]["dist"],
                        "rays_count": 1}]
        for i, point in enumerate(intersection_points[1:]):
            crossed_walls[-1]["rays_count"] += 1
            if not point["wall"] is crossed_walls[-1]["wall"]:
                crossed_walls[-1]["x2"] = intersection_points[i]["x"]
                crossed_walls[-1]["y2"] = intersection_points[i]["y"]
                crossed_walls[-1]["dist2"] = intersection_points[i]["dist"]
                crossed_walls.append(
                    {"wall": point["wall"],
                     "x1":point["x"],
                     "y1":point["y"],
                     "dist1":point["dist"],
                     "rays_count": 1})
        crossed_walls[-1]["x2"] = intersection_points[-1]["x"]
        crossed_walls[-1]["y2"] = intersection_points[-1]["y"]
        crossed_walls[-1]["dist2"] = intersection_points[-1]["dist"]
        COR = 0
        for crossed_wall in crossed_walls:
            y12_offset = (self.height/2)/crossed_wall["dist1"]*self.rast_do_steny
            y34_offset = (self.height/2)/crossed_wall["dist2"]*self.rast_do_steny
            y1 = self.height/2 + y12_offset
            y2 = self.height/2 - y12_offset
            y3 = self.height/2 + y34_offset
            y4 = self.height/2 - y34_offset
            ray_height = self.height/(len(self.camera.rays)-1)
            x1 = COR*ray_height + self.width
            COR += crossed_wall["rays_count"]
            x2 = COR*ray_height + self.width
            self.rectangles.append(
                self.canvas.create_line(
                    x1, y1, 
                    x2, y3, 
                    x2, y4, 
                    x1, y2, 
                    x1, y1))
            

    def render_buf(self, intersection_points):
        crossed_walls=[{"wall": intersection_points[0]["wall"],
                        "x1":intersection_points[0]["x"],
                        "y1":intersection_points[0]["y"],
                        "dist1":intersection_points[0]["dist"],
                        "rays_count": 1}]
        for i, point in enumerate(intersection_points[1:]):
            crossed_walls[-1]["rays_count"] += 1
            if not point["wall"] is crossed_walls[-1]["wall"]:
                crossed_walls[-1]["x2"] = intersection_points[i]["x"]
                crossed_walls[-1]["y2"] = intersection_points[i]["y"]
                crossed_walls[-1]["dist2"] = intersection_points[i]["dist"]
                crossed_walls.append(
                    {"wall": point["wall"],
                    "x1":point["x"],
                    "y1":point["y"],
                    "dist1":point["dist"],
                    "rays_count": 1})
        crossed_walls[-1]["x2"] = intersection_points[-1]["x"]
        crossed_walls[-1]["y2"] = intersection_points[-1]["y"]
        crossed_walls[-1]["dist2"] = intersection_points[-1]["dist"]
        COR = 0
        b = Buf()
        for crossed_wall in crossed_walls:
            y12_offset = (self.height/2)/crossed_wall["dist1"]*self.rast_do_steny
            y34_offset = (self.height/2)/crossed_wall["dist2"]*self.rast_do_steny
            y1 = self.height/2 + y12_offset
            y2 = self.height/2 - y12_offset
            y3 = self.height/2 + y34_offset
            y4 = self.height/2 - y34_offset
            ray_height = self.height/(len(self.camera.rays)-1)
            x1 = COR*ray_height
            COR += crossed_wall["rays_count"]
            x2 = COR*ray_height
            b.draw_line(x1, y1, x2, y3)
            b.draw_line(x2, y3, x2, y4)
            b.draw_line(x2, y4, x1, y2)
            b.draw_line(x1, y2, x1, y1)
            print(x1,x2,y12_offset,y34_offset,ray_height)
        b.save()

    def render_3d(self, intersection_points):
        for rec in self.rectangles:
            self.canvas.delete(rec)
        self.rectangles = []
        rectangle_width = int(self.width/len(intersection_points))
        for i, point in enumerate(intersection_points):
            distance = point["dist"] * math.cos(
                self.camera.rotation_angle-self.camera.rays[i].angle)
            coord = (self.height/2)/distance*self.rast_do_steny
            x1 = i*rectangle_width + self.height
            x2 = x1+rectangle_width
            y1 = self.height/2 - coord
            y2 = self.height/2 + coord
            y12 = abs(y1-y2)/64
            column = point["wall"].get_pixel_column(point["x"], point["y"])
            for j in range(64):
                color ='#%02x%02x%02x' % im.getpixel((column, j))[:3]
                self.rectangles.append(
                    self.canvas.create_rectangle(
                        x1,
                        y1+(j*y12),
                        x2,
                        y1+((1+j)*y12),
                        outline = color,
                        fill = color))

class Buf:
    def __init__(self):
        self.buf = bytearray(b"\xFF"*500*500*3)
    
    def setPixel(self,x,y):
        x = int(x)
        y = int(y)
        if 0 > y*500*3+x*3+2 or y*500*3+x*3+2 > len(self.buf):
            return
        try:
            self.buf[y*500*3+x*3] = 0
            self.buf[y*500*3+x*3+1] = 0
            self.buf[y*500*3+x*3+2] = 0
        except Exception as e:
            print(e,y*500*3+x*3+2, len(self.buf))

    def save(self):
        im = Image.frombytes(mode="RGB", size=(500, 500), data=bytes(self.buf))
        im.save("P:\\fb0\\tkinter\\piltest.jpg", "JPEG")
    
    def draw_line(self, x1=0, y1=0, x2=0, y2=0):
            dx = x2 - x1
            dy = y2 - y1
            sign_x = 1 if dx>0 else -1 if dx<0 else 0
            sign_y = 1 if dy>0 else -1 if dy<0 else 0
            if dx < 0: dx = -dx
            if dy < 0: dy = -dy
            if dx > dy:
                pdx, pdy = sign_x, 0
                es, el = dy, dx
            else:
                pdx, pdy = 0, sign_y
                es, el = dx, dy
            x, y = x1, y1
            error, t = el/2, 0        
            self.setPixel(x, y)
            while t < el:
                error -= es
                if error < 0:
                    error += el
                    x += sign_x
                    y += sign_y
                else:
                    x += pdx
                    y += pdy
                t += 1
                self.setPixel(x, y)


if __name__ == "__main__":
    c = Canvas(root, width=2*wind_width, height=wind_width, bg='white')
    c.pack()
    walls = [Wall(0, 0, wind_width, 0, c),
             Wall(0, 0, 0, wind_height, c),
             Wall(wind_width, wind_width, wind_width, 0, c),
             Wall(wind_width, wind_height, 0, wind_height, c),
             Wall(0, 200, 200, 200, c),
             Wall(0, 100, 200, 100, c)]

    walls.append(Wall(wind_width/2, 0, wind_width/2, wind_width/2, c))
    # walls.extend([Wall(random.randint(0, wind_width),
    #                    random.randint(0, wind_height),
    #                    random.randint(0, wind_width),
    #                    random.randint(0, wind_height), c)
    #               for x in range(5)])

    camera = Camera(100, 145, 100, math.pi/3, math.pi/2, c)
    render = Render(wind_width, 0, wind_width, wind_height, c, camera)
    player = Player(render)
    # c.bind('<Motion>', player.test)
    # c.create_rectangle(1000, 0, 2000, 1000)
    root.bind('<a>', player.rotate_left)
    root.bind('<d>', player.rotate_right)
    root.bind('<w>', player.move_forvard)
    root.bind('<s>', player.move_back)
    root.mainloop()
