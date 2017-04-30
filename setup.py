from setuptools import setup, Extension
import numpy
import os
import config

def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)
    return '';

print('locating directories...')
defines      = [ ('MAJOR_VERSION',0),('MINOR_VERSION',1) ]
include_dirs = [ numpy.get_include() ]
libraries    = []
library_dirs = []

print('checking for tiffio.h...')
if find('tiffio.h', config.tiff_include_dir) != '':
    defines.append( ('cimg_use_tiff',1) )
    include_dirs.append( config.tiff_include_dir )
    libraries.append( 'tiff' )
    library_dirs.append( config.tiff_library_dir )

print('checking for png.h...')
if find('png.h', config.png_include_dir ) != '':
    defines.append( ('cimg_use_png',1) )
    include_dirs.append( config.png_include_dir )
    libraries.append( 'png' )
    library_dirs.append( config.png_library_dir )

for lib in config.libs:
    libraries.append( lib )

print('Setting up extension...')
easy_image_io = Extension('easy_image_io',
                    define_macros=defines,
                    sources=['easy_image_io.cpp'],
                    include_dirs=include_dirs,
                    library_dirs=library_dirs,
                    libraries=libraries )

print('Building extension...')
setup(name='easy_image_io', version='0.1', ext_modules=[ easy_image_io ] )
