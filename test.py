#import sys, struct
#from PIL import Image
#import numpy as np
#with open("./00FF00_png.bmp", "rb") as img:
#	f = img.read()
#	b = bytearray(f)

#print b
#for i in xrange(0,len(b)-3,4):
#	sys.stdout.write(str(hex((b[i+3] << 24) | b[i+2] << 16 | b[i+1] << 8 | (b[i]))) + ", ")

a = open("00FF00_png.bmp", "rb").read()  
print a
#output = []
#for a in p:
#	print p
#for i in range(572*392):
#	


#print f.read()

#print (struct.unpack('i', fin.read(4)))
