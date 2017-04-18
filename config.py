from sys import platform

libs = []
if platform =='linux':
    anaconda_dir = '/anaconda'
elif platform == 'win32':
    anaconda_dir = 'C:/Program Files/Anaconda3'
    libs = [ 'shell32' ]

png_include_dir  = '%s/include' % anaconda_dir
png_library_dir  = '%s/lib' % anaconda_dir

tiff_include_dir = '%s/include' % anaconda_dir
tiff_library_dir = '%s/lib' % anaconda_dir
