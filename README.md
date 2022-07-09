# image-seam-carver
An image seam carver to resize PPM images without cutting and losing essential parts. 

execute 
```
make exec
```
to make the execution file.

use
```
bin/exec input-image [options] -o output-image
```
to seam carve the image.

Options to carve the image:\
-h n : carve the image horizontally for n times.\
-v n : carve the image vertically for n times.

example
```
bin/exec input.ppm -h 50 -v 30 -o output.ppm
```
Seam carve the input image 50 times horizontally and 30 times vertically and store the carved image as output.ppm.

example image:
original\
<img width="764" alt="image" src="https://user-images.githubusercontent.com/54991825/178125003-e08218a2-80ba-4b91-aadb-2b655529fa34.png">

image seam carved 200 times horizontally and 50 times vertically\
<img width="692" alt="image" src="https://user-images.githubusercontent.com/54991825/178125076-5e5e5221-eb32-499c-97ee-02cbc494fd39.png">
