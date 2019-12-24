import fcntl
import os
import struct
import mmap
import random
import time
f = os.open('/dev/fb0',os.O_RDWR)
# f = open('/dev/fb0','wrb')
a = struct.unpack("8I12I16I4I",fcntl.ioctl(f,0x4600," "*((8+12+16+4)*4)))
print(a)
framebuffer = mmap.mmap(f,2048*2048*4,flags=mmap.MAP_SHARED, prot=mmap.PROT_READ|mmap.PROT_WRITE)
print(len(framebuffer))
print(time.time())
fb='\xff'*len(framebuffer)
print(time.time())
framebuffer[0]=0
print(time.time())
os.close(f)