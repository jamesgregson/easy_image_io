# easy_image_io
James Gregson (james [dot] gregson [at] gmail [dot] com)

## Overview
easy_image_io is a Python3 extension module aimed at simplifying image reading and writing 16 bit images in Python. It provides only two functions, ```imwrite``` and ```imread```, which behave similarly to their Matlab equivalents.

The code is extremely simple, and only wraps the excellent [CImg](http://cimg.eu) library. CImg provides loading and saving support for a wide variety of formats via [ImageMagick](http://www.imagemagick.org/script/index.php), which is required. CImg in turn can optionally make use of libpng and libtiff to directly save the PNG and TIFF formats, which is recommended.

This code was born out of the following issues:
- Lack of support for scikit-image/freeimage in Anaconda3
- Limited 16 bit support in PIL/PILLOW
- Performance issues in pypng, which limit its use in data-intensive applications

## Requirements

- setuptools
- pip
- numpy
- ImageMagick
- libpng, optional, but recommended
- libtiff, optional, but recommended

## Known Issues

- On OS-X 10.12, using CImg without libpng results in PNG images with small pixel values being saved multiplied by a factor of 16. A workaround is to use the TIFF format, or to enable libpng

## Installation

```
$ conda build .
$ conda install --use-local easy_image_io
```

*outdated*

Only tested on OS-X (soon Ubuntu) but likely to be relatively straightforward on other platforms. Download the source code and extract. In the top level directory, edit the paths to libpng and libtiff (if desired).

Open a terminal in the source code directory.  

```
$ python setup.py bdist_wheel
$ pip install dist/easy_image_io*.whl
```

Try out the code by running ```example.py```, which serves as the main example and self-check. It should create RGB and LUM images in both TIFF and PNG formats, then read these back in and ensure that the loaded images match the original data.

## Basic Usage

See ```example.py``` for exmaple and self-check.

There are only two functions:

**imread( filename )** - Reads an image file given a filename. Returns a numpy array containing ```nump.uint16``` values. The result is returned stored as *planes*, i.e. each color channel is stored contiguously by scanline. For an RGB image loaded via ```I = easy_image_io.imread('input.png')```, ```I[0,:,:]``` would return a 2D array consisting of only the red channel.

**imwrite( I, filename )*** - Saves an image given a numpy array of ```numpy.uint8``` or ```numpy.uint16``` values, also stored planewise.

### What are these planes? Why not interleave the data?

CImg stores its images in this format, and it turns out to be very useful.  ```easy_image_io``` simply adopts this convention, for simplicity.  Converting between the two data layouts is very simple however:

```python
# return an image stored as planes (default for easy_image_io) as packed pixels
# suitable for plotting with matplotlib's imshow
def planes_to_packed( img ):
    # create the output image
    out = np.zeros( (img.shape[1],img.shape[2],img.shape[0]), dtype=img.dtype )
    out[:,:,0] = img[0,:,:]
    out[:,:,1] = img[1,:,:]
    out[:,:,2] = img[2,:,:]
    return out

# reverse of plane_to_packed, not needed here but listed for completeness
def packed_to_planes( img ):
    # create the output image
    out = np.zeros( (img.shape[2],img.shape[0],img.shape[1]), dtype=img.dtype )
    out[0,:,:] = img[:,:,0]
    out[1,:,:] = img[:,:,1]
    out[2,:,:] = img[:,:,2]
    return out
```
