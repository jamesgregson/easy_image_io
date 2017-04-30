import numpy as np
import easy_image_io as eiio

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

# make a 4-channel image 512 pixels wide and 256 pixels
# high and fill the channels with data
height = 1080;
width  = 1920;
x,y = np.meshgrid( np.arange(width), np.arange(height) )
RGB = np.zeros( (3,height,width), dtype=np.uint16 )
RGB[0,:,:] = x*255
RGB[1,:,:] = y*255
RGB[2,:,:] = (x+y)*255

LUM = np.uint16((x+y)*255);

# save the 4 channel image as a png
eiio.imwrite( RGB, 'RGB.png' )
eiio.imwrite( RGB, 'RGB.tif' )
eiio.imwrite( LUM, 'LUM.png' )
eiio.imwrite( LUM, 'LUM.tif' )


tRGB = eiio.imread( 'RGB.png' )
print( 'Checking RGB PNG, error sum: %d ' % np.sum(np.abs(RGB.flatten()-tRGB.flatten())) )

tRGB = eiio.imread( 'RGB.tif' )
print( 'Checking RGB TIFF, error sum: %d ' % np.sum(np.abs(RGB.flatten()-tRGB.flatten())) )

tLUM = eiio.imread( 'LUM.png' )
print( 'Checking monochrome PNG, error sum: %d ' % np.sum(np.abs(LUM.flatten()-tLUM.flatten())) )

tLUM = eiio.imread( 'LUM.tif' )
print( 'Checking monochrome TIFF, error sum: %d ' % np.sum(np.abs(LUM.flatten()-tLUM.flatten())) )
