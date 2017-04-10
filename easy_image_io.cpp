#include <string>
#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>

#define cimg_verbosity 0
#define cimg_display 0
#include "CImg.h"
namespace cl = cimg_library;

static char module_docstring[] = "easy_image_io: read/write 8/16 bit png files";
static char error_docstring[]  = "easy_image_io: read/write 8/16 bit png files";

static PyObject *imread( PyObject *self, PyObject *args );
static PyObject *imwrite( PyObject *self, PyObject *args );

static PyMethodDef module_methods[] = {
  { "imread",  imread,  METH_VARARGS, error_docstring },
  { "imwrite", imwrite, METH_VARARGS, error_docstring },
  { NULL, NULL, 0, NULL }
};

static PyModuleDef module_def = {
  PyModuleDef_HEAD_INIT,
  "easy_image_io",
  "Loads & saves image files",
  -1, 
  module_methods
};

PyMODINIT_FUNC PyInit_easy_image_io(){
  Py_Initialize();
  import_array();
  return PyModule_Create( &module_def );
}

static PyObject *raise_error( PyObject *module, const char *error_str ){
  PyErr_SetString( PyExc_ValueError, error_str );
  return NULL;
}

std::string get_file_extension(const std::string& FileName){
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

template< typename T >
struct is_16_bit { static const bool value=false; };

template<>
struct is_16_bit<unsigned char>{ static const bool value=false; };

template<>
struct is_16_bit<unsigned short>{ static const bool value=true; };

template< typename T >
void save( PyArrayObject* arr, const char* filename ){
  int width, height, depth, spectrum;

  // handle 2D arrays
  if( arr->nd == 2 ){
    width    = arr->dimensions[1];
    height   = arr->dimensions[0];
    depth    = 1;
    spectrum = 1;
  } else if( arr->nd == 3 && ( arr->dimensions[0] == 3 || arr->dimensions[0] == 4 ) ){
    width    = arr->dimensions[2];
    height   = arr->dimensions[1];
    depth    = 1;
    spectrum = arr->dimensions[0];
  } else {
    std::cout << "Reached never-reached!" << std::endl;
    throw "Huh?";
  }
  // initialize an output image and copy its data
  cl::CImg<T> out( width, height, depth, spectrum, T(0) );
  int N = width*height*depth*spectrum;
  memcpy( out.data(), arr->data, N*sizeof(T) );

  // use TMP to determine if we should save as 16 bit, make sure the file type supports it
  bool use_16bit = is_16_bit<T>::value;
  std::string ext = get_file_extension( filename );
  if( use_16bit ){
    if(ext == "png" || ext == "PNG" ){
      std::cout << "Saving as 16 bit PNG" << std::endl;
      out.save_png( filename );
      return;
    } else if( ext == "tif" || ext == "TIF" ){
      std::cout << "Saving as 16 bit TIFF" << std::endl;
      out.save_tiff( filename );
      return;
    } else {
      std::cout << "Unsupported format for 16 bit inputs, falling back to 8 bit save" << std::endl;
      out.save( filename );
      return;
    }
  } else {
    out.save( filename );
  }
}

/*
  Loads image files
*/
static PyObject *imwrite( PyObject *self, PyObject *args ){
  if( PyTuple_Size(args) != 2 ){
    return raise_error( self, "Expected two arguments" );
  }
  // parse input arguments as an input python array object and a string filename
  char *s;
  PyArrayObject *arr;
  if( !PyArg_ParseTuple(args,"O!s", &PyArray_Type, &arr, &s ) ){
    return raise_error( self, "Expected first argument to be a NumPy array and second to be filename");
  }
  // check the extension type
  std::string ext = get_file_extension(s);
  if( ext != "png" && ext != "PNG" && ext != "tif" && ext != "TIF" ){
    std::cout << "Only PNG (.png,.PNG) and TIFF (.tif,.TIF) files support 16 bit saving" << std::endl;
  }
  if( arr->descr->type_num != PyArray_UINT8 && arr->descr->type_num != PyArray_UINT16 ){
    return raise_error( self, "Expected type of array to be uint8 or uint16");
  }
  if( arr->nd == 2 || ( arr->nd == 3 && arr->dimensions[0] <= 4 ) ){
    if( arr->descr->type_num == PyArray_UINT8 ){
      save<unsigned char>( arr, s );
    } else if( arr->descr->type_num == PyArray_UINT16 ){
      save<unsigned short>( arr, s );
    }
    return Py_BuildValue("");
  }
  return Py_BuildValue("");
}


static PyObject *imread( PyObject *self, PyObject *args ){
  // make sure that we get a string input..
  char *s;
  if( !PyArg_ParseTuple(args,"s", &s ) ){
    return raise_error( self, "Expected an input filename as argument!" );
  }

  try {
    cl::CImg<unsigned short> input( s );
    if( input.depth() > 1 )
      return raise_error( self, "Input image can only have 1-4 channels" );

    if( input.spectrum() == 1 ){
      // read a single-channel image
      npy_intp dims[] = { input.height(), input.width() };
      unsigned short *data = (unsigned short*)malloc( input.width()*input.height()*sizeof(unsigned short) );
      memcpy( data, input.data(), input.width()*input.height()*sizeof(unsigned short) );
      PyObject *ret = PyArray_SimpleNewFromData( 2, dims, (int)NPY_UINT16, data );
      PyArray_ENABLEFLAGS( (PyArrayObject*)ret, NPY_ARRAY_OWNDATA);
      return ret;
    } else if( input.spectrum() == 3 ){
      // read a RGB image
      npy_intp dims[] = { 3, input.height(), input.width() };
      unsigned short *data = (unsigned short*) malloc( 3*input.width()*input.height()*sizeof(unsigned short) );
      memcpy( data, input.data(), 3*input.width()*input.height()*sizeof(unsigned short) );
      PyObject *ret = PyArray_SimpleNewFromData( 3, dims, (int)NPY_UINT16, data );
      PyArray_ENABLEFLAGS( (PyArrayObject*)ret, NPY_ARRAY_OWNDATA);
      return ret;
    } else {
      return raise_error(self,"Expected a 1 or 3 channel image");
    }
  } catch(...){
    char err[1024];
    sprintf( err, "Failed to load input '%s'", s );
    return raise_error( self, err );
  }

  return NULL;
}
