

import atexit
import test
import time
W = 2048
H = 2048
S = 4
class fb_draw():
    def __init__(self,w,h,s):
        self.W = w
        self.H = h
        self.S = s
        self.f = open('/dev/fb0','wb')
        self.frame = bytearray([0 for x in range((self.W*self.H*self.S)+(800*600*4))])
        self.buffer = []

    def setPixel(self,x,y):
        self.buffer.append([x,y])

    def draw_line(self,x1=0, y1=0, x2=0, y2=0):
            
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

    def draw_circle(self,x, y, r):
        disp_x = x
        disp_y = y
        x = 0
        y = r
        delta = (1-2*r)
        error = 0
        while y >= 0:
            self.setPixel(disp_x + x, disp_y + y)
            self.setPixel(disp_x + x, disp_y - y)
            self.setPixel(disp_x - x, disp_y + y)
            self.setPixel(disp_x - x, disp_y - y)
            
            error = 2 * (delta + y) - 1
            if ((delta < 0) and (error <=0)):
                x+=1
                delta = delta + (2*x+1)
                continue
            error = 2 * (delta - x) - 1
            if ((delta > 0) and (error > 0)):
                y -= 1
                delta = delta + (1 - 2 * y)
                continue
            x += 1
            delta = delta + (2 * (x - y))
            y -= 1
    
    def draw(self):
        for cords in self.buffer:
            x,y=cords
            if (x<self.W) and (y<self.H) and (x>0 and 0<y):
                self.frame[(x*self.W*self.S)+(y*self.S) + 0]= b'\xff'
                self.frame[(x*self.W*self.S)+(y*self.S) + 1]= b'\xff'
                self.frame[(x*self.W*self.S)+(y*self.S) + 2]= b'\xff'
        self.f.write(self.frame)

    def __del__(self):
        self.f.close()


d = fb_draw(W,H,S)
for i in range(10):
    d.draw_circle(100,100,30)
    d.draw_circle(100,100,40)
    d.draw_circle(100,100,50)
    d.draw_circle(100,100,60)
    d.draw_circle(100,100,70)
    d.draw_circle(100,100,80)
    d.draw_line(-5000,-5000,5000,5000)
    d.draw()
    time.sleep(1)
d.__del__()