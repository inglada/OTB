/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPNGImageIO.cxx,v $
  Language:  C++
  Date:      $Date: 2007/08/13 21:38:38 $
  Version:   $Revision: 1.64 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkPNGImageIO.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itk_png.h"
#include <itksys/SystemTools.hxx>

namespace itk
{

extern "C"
{
  /* The PNG library does not expect the error function to return.
     Therefore we must use this ugly longjmp call.  */
  void itkPNGWriteErrorFunction(png_structp png_ptr,
                                png_const_charp itkNotUsed(error_msg))
    {
    longjmp(png_ptr->jmpbuf, 1);
    }
}


extern "C"
{
  void itkPNGWriteWarningFunction(png_structp itkNotUsed(png_ptr),
                                  png_const_charp itkNotUsed(warning_msg))
    {
    }
}


// simple class to call fopen on construct and
// fclose on destruct
class PNGFileWrapper
{
public:
  PNGFileWrapper(const char * const fname, const char * const openMode):m_FilePointer(NULL)
    {
    m_FilePointer = fopen(fname, openMode);
    }
  virtual ~PNGFileWrapper()
    {
    if(m_FilePointer)
      {
      fclose(m_FilePointer);
      }
    }
  FILE* m_FilePointer;
};

bool PNGImageIO::CanReadFile(const char* file) 
{ 
  // First check the extension
  std::string filename = file;
  if(  filename == "" )
    {
    itkDebugMacro(<<"No filename specified.");
    return false;
    }

  // Now check the file header
  PNGFileWrapper pngfp(file,"rb");
  if(pngfp.m_FilePointer==NULL)
    {
    return false;
    }
  unsigned char header[8];
  fread(header, 1, 8, pngfp.m_FilePointer);
  bool is_png = !png_sig_cmp(header, 0, 8);
  if(!is_png)
    {
    return false;
    }
  png_structp png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
     NULL, NULL);
  if (!png_ptr)
    {
    return false;
    }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
    png_destroy_read_struct(&png_ptr,
                            (png_infopp)NULL, (png_infopp)NULL);
    return false;
    }
  
  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
    {
    png_destroy_read_struct(&png_ptr, &info_ptr,
                            (png_infopp)NULL);
    return false;
    }
  png_destroy_read_struct(&png_ptr, &info_ptr,
                          &end_info);
  
  return true;
}
  
  
void PNGImageIO::ReadVolume(void*)
{
  
}

  
void PNGImageIO::Read(void* buffer)
{
  itkDebugMacro("Read: file dimensions = " << this->GetNumberOfDimensions() );
  // use this class so return will call close
  PNGFileWrapper pngfp(this->GetFileName(),"rb"); 
  FILE* fp = pngfp.m_FilePointer;
  if(!fp)
    {
    itkExceptionMacro("PNGImageIO could not open file: "
                      << this->GetFileName() << " for reading."
                      << std::endl
                      << "Reason: "
                      << itksys::SystemTools::GetLastSystemError());
    return;
    }
  unsigned char header[8];
  fread(header, 1, 8, fp);
  bool is_png = !png_sig_cmp(header, 0, 8);
  if(!is_png)
    {
    itkExceptionMacro("File is not png type: " << this->GetFileName());
    return;
    }
  png_structp png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
     NULL, NULL);
  if (!png_ptr)
    {
    itkExceptionMacro("File is not png type" << this->GetFileName());
    return;
    }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
    png_destroy_read_struct(&png_ptr,
                            (png_infopp)NULL, (png_infopp)NULL);
    itkExceptionMacro("File is not png type " << this->GetFileName());
    return;
    }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
    {
    png_destroy_read_struct(&png_ptr, &info_ptr,
                            (png_infopp)NULL);
    itkExceptionMacro("File is not png type " << this->GetFileName());
    return;
    }
  
  if( setjmp( png_jmpbuf( png_ptr ) ) )
    {
    png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
    itkExceptionMacro("File is not png type " << this->GetFileName());
    return;
    }

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  png_uint_32 width, height;
  int bitDepth, colorType, interlaceType;
  int compression_type, filter_method;
  png_get_IHDR(png_ptr, info_ptr, 
               &width, &height,
               &bitDepth, &colorType, &interlaceType,
               &compression_type, &filter_method);

  // convert palettes to RGB
  if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
    png_set_palette_to_rgb(png_ptr);
    }

  // minimum of a byte per pixel
  if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) 
    {
    png_set_gray_1_2_4_to_8(png_ptr);
    }

  // add alpha if any alpha found
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
    {
    png_set_tRNS_to_alpha(png_ptr);
    }

  if (bitDepth > 8)
    {
#ifndef ITK_WORDS_BIGENDIAN
    png_set_swap(png_ptr);
#endif
    }

  if (info_ptr->valid & PNG_INFO_sBIT)
    {
    png_set_shift(png_ptr, &(info_ptr->sig_bit));
    }
  // have libpng handle interlacing
  //int number_of_passes = png_set_interlace_handling(png_ptr);
  // update the info now that we have defined the filters
  png_read_update_info(png_ptr, info_ptr);

  unsigned long rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  unsigned char *tempImage = static_cast<unsigned char*>(buffer);
  png_bytep *row_pointers = new png_bytep [height];
  for (unsigned int ui = 0; ui < height; ++ui)
    {
    row_pointers[ui] = tempImage + rowbytes*ui;
    }
  png_read_image(png_ptr, row_pointers);
  delete [] row_pointers;
  // close the file
  png_read_end(png_ptr, NULL);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

}


PNGImageIO::PNGImageIO()
{
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;
  m_UseCompression = false;
  m_CompressionLevel = 4; // Range 0-9; 0 = no file compression, 9 = maximum file compression
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
}

PNGImageIO::~PNGImageIO()
{
}

void PNGImageIO::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compression Level : " << m_CompressionLevel << "\n";
}

  
  
void PNGImageIO::ReadImageInformation()
{
  m_Spacing[0] = 1.0;  // We'll look for PNG pixel size information later,
  m_Spacing[1] = 1.0;  // but set the defaults now

  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  // use this class so return will call close
  PNGFileWrapper pngfp(m_FileName.c_str(),"rb");
  FILE* fp = pngfp.m_FilePointer;
  if(!fp)
    {
    return;
    }
  unsigned char header[8];
  fread(header, 1, 8, fp);
  bool is_png = !png_sig_cmp(header, 0, 8);
  if(!is_png)
    {
    return;
    }
  png_structp png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
     NULL, NULL);
  if (!png_ptr)
    {
    return;
    }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
    png_destroy_read_struct(&png_ptr,
                            (png_infopp)NULL, (png_infopp)NULL);
    return;
    }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
    {
    png_destroy_read_struct(&png_ptr, &info_ptr,
                            (png_infopp)NULL);
    return;
    }
  
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  png_uint_32 width, height;
  int bitDepth, colorType, interlaceType;
  int compression_type, filter_method;
  png_get_IHDR(png_ptr, info_ptr, 
               &width, &height,
               &bitDepth, &colorType, &interlaceType,
               &compression_type, &filter_method);

  // convert palettes to RGB
  if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
    png_set_palette_to_rgb(png_ptr);
    }

  // minimum of a byte per pixel
  if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) 
    {
    png_set_gray_1_2_4_to_8(png_ptr);
    }

  // add alpha if any alpha found
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
    {
    png_set_tRNS_to_alpha(png_ptr);
    }

  // update the info now that we have defined the filters
  png_read_update_info(png_ptr, info_ptr);
  this->SetNumberOfDimensions(2);
  m_Dimensions[0] = width;
  m_Dimensions[1] = height;
  if (bitDepth <= 8)
    {
    m_PixelType = SCALAR;
    m_ComponentType = UCHAR;
    }
  else
    {
    m_PixelType = SCALAR;
    m_ComponentType = USHORT;
    }
  this->SetNumberOfComponents(png_get_channels(png_ptr, info_ptr));

  if (this->GetNumberOfComponents() == 3)
    {
    m_PixelType = RGB;
    }
  else if (this->GetNumberOfComponents() == 4)
    {
    m_PixelType = RGBA;
    }

  // see if the PNG file stored spacing information,
  // ignore the units (for now).
  double px_width = 1.0, px_height = 1.0; // use default values if not in file
  int units = PNG_SCALE_UNKNOWN;
  png_get_sCAL(png_ptr, info_ptr, &units, &px_width, &px_height);

  m_Spacing[0] = px_width;
  m_Spacing[1] = px_height;


  // clean up
  png_destroy_read_struct(&png_ptr, &info_ptr,
                          &end_info);

  return;
}

bool PNGImageIO::CanWriteFile( const char * name )
{
  std::string filename = name;

  if (filename == "")
    {
    return false;
    }
  
  std::string::size_type pngPos = filename.rfind(".png");
  if ( (pngPos != std::string::npos)
       && (pngPos == filename.length() - 4) )
    {
    return true;
    }

  pngPos = filename.rfind(".PNG");
  if ( (pngPos != std::string::npos)
       && (pngPos == filename.length() - 4) )
    {
    return true;
    }


  return false;
}


void PNGImageIO::WriteImageInformation(void)
{
}

void PNGImageIO::Write(const void* buffer)
{
  this->WriteSlice(m_FileName, buffer);
}

void PNGImageIO::WriteSlice(const std::string& fileName, const void* buffer)
{
  const unsigned char *outPtr = ( (const unsigned char *) buffer);

  // use this class so return will call close
  PNGFileWrapper pngfp(fileName.c_str(),"wb");
  FILE* fp = pngfp.m_FilePointer;
  if(!fp)
    {
    // IMPORTANT: The itkExceptionMacro() cannot be used here due to a bug in Visual
    //            Studio 7.1 in release mode. That compiler will corrupt the RTTI type
    //            of the Exception and prevent the catch() from recognizing it.
    //            For details, see Bug # 1872 in the bugtracker.

    ::itk::ExceptionObject excp(__FILE__, __LINE__, "Problem while opening the file.", ITK_LOCATION); 
    throw excp; 
    }

  int bitDepth;
  switch (this->GetComponentType())
    {
    case UCHAR:
      bitDepth = 8;
      break;

    case USHORT:
      bitDepth = 16;
      break;

    default:
      {
      // IMPORTANT: The itkExceptionMacro() cannot be used here due to a bug in Visual
      //            Studio 7.1 in release mode. That compiler will corrupt the RTTI type
      //            of the Exception and prevent the catch() from recognizing it.
      //            For details, see Bug # 1872 in the bugtracker.
      ::itk::ExceptionObject excp(__FILE__, __LINE__, "PNG supports unsigned char and unsigned short", ITK_LOCATION);
      throw excp; 
      }
    }
  
  png_structp png_ptr = png_create_write_struct
    (PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
  if (!png_ptr)
    {
    itkExceptionMacro(<<"Unable to write PNG file! png_create_write_struct failed.");
    }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
    png_destroy_write_struct(&png_ptr,
                             (png_infopp)NULL);
    itkExceptionMacro(<<"Unable to write PNG file!. png_create_info_struct failed.");
    }

  png_init_io(png_ptr, fp);
  png_set_error_fn(png_ptr, png_ptr,
                   itkPNGWriteErrorFunction, itkPNGWriteWarningFunction);
  if (setjmp(png_ptr->jmpbuf))
    {
    fclose(fp);
    itkExceptionMacro("Error while writing Slice to file: "
                      <<this->GetFileName()
                      << std::endl
                      << "Reason: "
                      << itksys::SystemTools::GetLastSystemError());
    return;
    } 

  int colorType;
  unsigned int numComp = this->GetNumberOfComponents();
  switch ( numComp )
    {
    case 1: colorType = PNG_COLOR_TYPE_GRAY;
      break;
    case 2: colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;
    case 3: colorType = PNG_COLOR_TYPE_RGB;
      break;
    default: colorType = PNG_COLOR_TYPE_RGB_ALPHA;
      break;
    }
  
  png_uint_32 width, height;
  double rowSpacing, colSpacing;
  width = this->GetDimensions(0);
  colSpacing = m_Spacing[0];

  if( m_NumberOfDimensions > 1 )
    {
    height = this->GetDimensions(1);
    rowSpacing = m_Spacing[1];
    }
  else
    {
    height = 1;
    rowSpacing = 1;
    }
  
  png_set_IHDR(png_ptr, info_ptr, width, height,
               bitDepth, colorType, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, 
               PNG_FILTER_TYPE_DEFAULT);
  // interlaceType - PNG_INTERLACE_NONE or
  //                 PNG_INTERLACE_ADAM7
    
  if(m_UseCompression)
    {
    // Set the image compression level.
    png_set_compression_level(png_ptr, m_CompressionLevel); 
    }

  // write out the spacing information:
  //      set the unit_type to unknown.  if we add units to ITK, we should
  //          convert pixel size to meters and store units as meters (png
  //          has three set of units: meters, radians, and unknown).
  png_set_sCAL(png_ptr, info_ptr, PNG_SCALE_UNKNOWN, colSpacing,
               rowSpacing);

  //std::cout << "PNG_INFO_sBIT: " << PNG_INFO_sBIT << std::endl;

  png_write_info(png_ptr, info_ptr);
  // default is big endian
  if (bitDepth > 8)
    {
#ifndef ITK_WORDS_BIGENDIAN
    png_set_swap(png_ptr);
#endif
    }
  png_byte **row_pointers = new png_byte *[height];
  int rowInc = width*numComp*bitDepth/8;
  for (unsigned int ui = 0; ui < height; ui++)
    {
    row_pointers[ui] = const_cast<png_byte *>(outPtr);
    outPtr = const_cast<unsigned char *>(outPtr) + rowInc;
    }
  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, info_ptr);

  delete [] row_pointers;
  png_destroy_write_struct(&png_ptr, &info_ptr);
}


} // end namespace itk
