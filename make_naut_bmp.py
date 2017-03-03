#! /usr/local/bin/python
import os
import sys
import subprocess
import struct


chunk_size = 8192
bpp = 4

def main():
    if len(sys.argv) != 4:
        print "usage: <image file> <image width> <image height>"

    raw_file_name = sys.argv[1]
    parts = raw_file_name.split(".")
    file_name = parts[0]
    extension = parts[1]
    width = sys.argv[2]
    height = sys.argv[3]
    rgba_file_name = file_name + ".rgba"
    naut_file_name = file_name + ".h"
    header_header = "#ifndef " + file_name + "_H\n#define " + file_name + "_H\n\n"
    header_trailer = "#endif /* " + file_name + "_H */"

    hashdef_image_width = "#define " + file_name + "width " + sys.argv[2] + "\n"
    hashdef_image_height = "#define " + file_name + "height " + sys.argv[3] + "\n"

    array_begin = "#define " + file_name + "_array ""{"
    array_end = "}"

    pixels_per_line = 4

    if os.access(naut_file_name, os.F_OK):
        os.remove(naut_file_name)
# oops this could have been a string
    im_args = ["convert", raw_file_name, "-resize", width + "x" + height + "^", "-gravity", "center", "-extent", width + "x" + height, "RGBA:" + rgba_file_name]
    subprocess.call(im_args)

    with open(naut_file_name, 'a') as out:
        out.write(header_header)
        out.write("\n")
        out.write(hashdef_image_width)
        out.write(hashdef_image_height)
        out.write("\n")
        out.write(array_begin)
        with open(rgba_file_name, 'r') as f:
            file_chunk = f.read(chunk_size)
            while file_chunk:
                shifted = ""
                for i in xrange(0, len(file_chunk), bpp):
                    if file_chunk[i + bpp - 1]:
                        shifted += hex(struct.unpack('i', file_chunk[i:i+bpp])[0]  & 0x00FFFFFF) + ( ", "  if (i / bpp) % pixels_per_line else "," )
                    if ((i / bpp) % pixels_per_line) == 0:
                        shifted +=("\\\n")
                out.write(shifted)
                file_chunk = f.read(chunk_size)
        out.write(array_end)
        out.write("\n")
        out.write(header_trailer)

if __name__ == "__main__":
    main()
