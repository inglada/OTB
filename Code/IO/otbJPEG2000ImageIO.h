/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbJPEG2000ImageIO_h
#define __otbJPEG2000ImageIO_h

#include "itkImageIOBase.h"

#ifndef USE_OPJ_DEPRECATED
#define USE_OPJ_DEPRECATED
#endif

extern "C"
{
#include "otb_openjpeg.h"
}

namespace otb
{

/** \class JPEG2000ImageIO
 *
 * \brief ImageIO object for reading and writing JPEG2000 format images
 *
 * The streaming (read and write) is implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT JPEG2000ImageIO : public itk::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef JPEG2000ImageIO         Self;
  typedef itk::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(JPEG2000ImageIO, itk::ImageIOBase);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead()
  {
    return true;
  }

  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite()
  {
    return true;
  }

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);

protected:
  /** Constructor.*/
  JPEG2000ImageIO();
  /** Destructor.*/
  virtual ~JPEG2000ImageIO();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  JPEG2000ImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Openjpeg decoder parameters */
  opj_dparameters_t m_Parameters;
  /** Openjpeg codec */
  opj_codec_t * m_Codec;
  /** File stream */
  FILE * m_File;
  /** Openjpeg image */
  opj_image_t * m_OpenJpegImage;
  /** Openjpeg stream */
  opj_stream_t * m_OpenJpegStream;
  /** pixel nb of octets */
  unsigned int m_NbOctetPixel;

};

} // end namespace otb

#endif // __otbJPEG2000ImageIO_h
