from PIL import Image
im = Image.open("P:\\fb0\\tkinter\\texture.png")

a = [ [list(im.getpixel((j, i))[:3]) for j in range(64) ] for i in range(64)]
f = open('arr.txt', "w")
f.write(str(a))
f.close()
