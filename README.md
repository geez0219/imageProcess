# imageProcess
This is a self-made image processing library for C++.
 
* basic function: crop, resize, color space transform
* advance function: contrast equalization, homographic transform, Non-local Mean denoising. 
* image coding support: Shannon-Fano, Huffman, Adaptive Huffman

# Using Example
## read image / output image
### read image from file 

```
#include “PIC.h”
Pic a(fileNameIn, sizey, sizex, sizez);
```

### output image as .raw
```
#include “PIC.h”
PIC a(fileNameIn, sizey, sizex, sizez) ;
a.output(fileNameOut)
```

### output image using Shannon-Fano/Huffman/Adaptive Huffman coding
```
#include “PIC.h”
Pic a(fileNameIn, sizey, sizex, sizez);
string treeFileName = "tree.csv";
a.encodeFano(fileNameOut, treeFileName); // using Shanno-Fano coding
a.encodeHuff(fileNameOut, treeFileName); // using Huffman coding
a.encodeAdaptHuff(fileNameOut); // using Adaptive Huffman
```
* Note: <br />
Shannon-Fano and Huffman coding will generate the trees file that are required for decoding 

###decode file back to image using Shannon-Fano/Huffman/Adaptive Huffman coding
```
#include “PIC.h”
Pic a;
a = Pic::decodeFano(fileNameIn, treeFileName, SizeY, SizeX, SizeZ); // using Shanno-Fano coding
a = Pic::decodeHuff(fileNameIn, treeFileName, SizeY, SizeX, SizeZ); // using Huffman coding
a = Pic::decodeAdaptHuff(fileNameIn, SizeY, SizeX, SizeZ); // using Adaptive Huffman
```
* Note: <br />
Shannon-Fano and Huffman decoding need the tree files that were generated during encoding

### file to file adaptive Huffman encoding and decoding
```
#include “AdaptHuffNode.h”
AdaptHuffNode::encodeFile(filenameIn, filenameOut);
AdaptHuffNode::decodeFile(filenameIn, filenameOut);
```

## basic image manipulation
### crop image
```
#include “PIC.h”
// way#1: return cropped copy
Pic a(fileNameIn, sizey, sizex, sizez); 
int yStart=0, xStart=0, yLength=200, xLength=200;
Pic b = a.crop(yStart, xStart, yLength, xLength);
b.output(fileNameOut);
// way#2 take dst as input argument, return nothing
Pic c;
a.crop(c, yStart, xStart, yLength, xLength);
c.output(fileNameOut);
```
### resize
```
#include "PIC.h"
Pic a(fileNameIn, sizey, sizex, sizez);
int newY = 200, newX = 200;
a.resize(newY, newX);
a.output(fileNameOut);
```
### change colorspace
```
#include "PIC.h"
Pic a(fileNameIn, sizey, sizex, sizez);
a.RGBToCMY(); // a.RGBToHSL()
```
## advanve image function
### constrast_equalize
```
#include "PIC.h"
Pic a(fileNameIn, sizey, sizex, sizez);
a.histEqualize();
a.output(fileNameOut);
```
<img src="https://user-images.githubusercontent.com/27904418/55457348-82d98d00-559e-11e9-83b6-8e29e0b142ab.jpg" height="200">
<img src="https://user-images.githubusercontent.com/27904418/55457360-8c62f500-559e-11e9-9b80-27f8d50083ff.jpg" height="200">


### homographic transform
```
#include “PIC.h”
Pic a(fileNameIn, sizey, sizex, sizez);
Point_2D corner1(0, 0); // new top-left corner 
Point_2D corner2(300, 100); // new bottom-left corner
Point_2D corner3(400, 500); // new bottom-right corner
Point_2D corner4(0, 400); // new top-right corner

//change the original four corner to new 4 corner
Pic b = a.homoTransform(corner1,corner2,corner3,corner4); 
b.output(fileNameOut);
```
<img src="https://user-images.githubusercontent.com/27904418/55456632-6fc5bd80-559c-11e9-924a-37b837ddd0a9.jpg" width="200" height="200">
<img src="https://user-images.githubusercontent.com/27904418/55456647-7e13d980-559c-11e9-82bb-2cdcdf9e3f4b.jpg" width="200" height="200">


### NLM denoise
```
#include "PIC.h"
#include "FILTER.h"
Pic a(fileNameIn, sizey, sizex, sizez);
Filter kernel = Filter::makeFilter_gauss(3, 3, 3, 0.8);
kernel.print();
int searchSize = 10;
float h = 200;
Pic b = a.NLM(kernel, searchSize, h);
b.output(fileNameOut);
```
<img src=https://user-images.githubusercontent.com/27904418/55457339-77866180-559e-11e9-96f8-438191f4ba2c.jpg height=200>
<img src=https://user-images.githubusercontent.com/27904418/55457300-65a4be80-559e-11e9-9e90-c7c4a3d0c915.jpg height=200>
