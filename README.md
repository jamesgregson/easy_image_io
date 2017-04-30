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

- [Anaconda for Python3](https://www.continuum.io/downloads)
- [ImageMagick](https://www.imagemagick.org/script/index.php)
- [Git](https://git-scm.com/) - Optional

## Installation

Install Anaconda3 and create a Python3 environment and activate it (skip if you've done this already)

```
$ conda create -n test python=3.5
$ activate test # or 'source activate test'
```

Clone the ```easy_image_io``` repository (or download and extract .zip). Open a terminal window and navigate to the ```<extraction_path>/easy_image_io/conda``` directory and run ```conda build .```, then

E.g.

```
$ git clone https://github.com/jamesgregson/easy_image_io
$ cd easy_image_io/conda
$ conda build .
$ conda install --use-local easy_image_io
```

Try out the code by running ```example.py```, which serves as the main example and self-check. It should create RGB and LUM images in both TIFF and PNG formats, then read these back in and ensure that the loaded images match the original data.

## Basic Usage

See ```example.py``` for example and self-check.

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
