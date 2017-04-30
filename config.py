from sys import platform
import os

libs = []
anaconda_dir = os.environ['CONDA_PREFIX']
if platform =='linux':
    pass
elif platform == 'win32':
    libs = [ 'shell32' ]
elif platform == 'darwin':
    pass

png_include_dir  = '%s/include' % anaconda_dir
png_library_dir  = '%s/lib' % anaconda_dir

tiff_include_dir = '%s/include' % anaconda_dir
tiff_library_dir = '%s/lib' % anaconda_dir
