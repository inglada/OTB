/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNrrdImageIO.h,v $
  Language:  C++
  Date:      $Date: 2005/09/28 15:41:55 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNrrdImageIO_h
#define __itkNrrdImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkImageIOBase.h"
#include <fstream>
#include "NrrdIO/NrrdIO.h"

namespace itk
{

/** \class NrrdImageIO
 *
 * \brief Read and write the "Nearly Raw Raster Data" (Nrrd) image format.
 * The Nrrd format was developed as part of the Teem package
 * (teem.sourceforge.net).
 *
 *  \ingroup IOFilters
 */
class ITK_EXPORT NrrdImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef NrrdImageIO            Self;
  typedef ImageIOBase  Superclass;
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NrrdImageIO, ImageIOBase);

  /** The different types of ImageIO's can support data of varying
   * dimensionality. For example, some file formats are strictly 2D
   * while others can support 2D, 3D, or even n-D. This method returns
   * true/false as to whether the ImageIO can support the dimension
   * indicated. */
  virtual bool SupportsDimension(unsigned long );

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);

protected:
  NrrdImageIO() {}
  ~NrrdImageIO() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Utility functions for converting between enumerated data type
      representations */
  int ITKToNrrdComponentType( const ImageIOBase::IOComponentType ) const;
  ImageIOBase::IOComponentType NrrdToITKComponentType( const int ) const;

private:
  NrrdImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif // __itkNrrdImageIO_h
