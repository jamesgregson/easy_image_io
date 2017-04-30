from sys import platform
import os

libs = []
anaconda_dir = os.environ['CONDA_PREFIX']
# if platform =='linux':
#     anaconda_dir = '/anaconda'
# elif platform == 'win32':
#     anaconda_dir = 'C:/Program Files/Anaconda3'
#     libs = [ 'shell32' ]
# elif platform == 'darwin':
#     anaconda_dir = '/anaconda'

png_include_dir  = '%s/include' % anaconda_dir
png_library_dir  = '%s/lib' % anaconda_dir

tiff_include_dir = '%s/include' % anaconda_dir
tiff_library_dir = '%s/lib' % anaconda_dir
