/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaImageIO.h,v $
  Language:  C++
  Date:      $Date: 2007/09/11 12:16:58 $
  Version:   $Revision: 1.34 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaImageIO_h
#define __itkMetaImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <fstream>
#include "itkImageIOBase.h"
#include "metaObject.h"
#include "metaImage.h"

namespace itk
{

/** \class MetaImageIO
 *
 *  \brief Read MetaImage file format.
 *
 *  \ingroup IOFilters
 */
class ITK_EXPORT MetaImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef MetaImageIO        Self;
  typedef ImageIOBase        Superclass;
  typedef SmartPointer<Self> Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MetaImageIO, Superclass);

  /** The different types of ImageIO's can support data of varying
   * dimensionality. For example, some file formats are strictly 2D
   * while others can support 2D, 3D, or even n-D. This method returns
   * true/false as to whether the ImageIO can support the dimension
   * indicated. */
  virtual bool SupportsDimension(unsigned long )
    {
    return true;
    }

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();
  
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  MetaImage * GetMetaImagePointer(void);
  
  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();
  
  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);

  /** Set the filename for the Data file. Setting this will make the
      Writer to use the non-Local mode and save header and data in 
      independent files */
  virtual void SetDataFileName( const char * filename );

  /** set the precision in the MetaImage member
   */
  virtual void SetDoublePrecision(unsigned int precision)
    {
    m_MetaImage.SetDoublePrecision(precision);
    }

  /** Method for supporting streaming.  Given a requested region, calculate what
   * could be the region that we can read from the file. This is called the
   * streamable region, which will be smaller than the LargestPossibleRegion and
   * greater or equal to the RequestedRegion */
  virtual ImageIORegion 
  GenerateStreamableReadRegionFromRequestedRegion( const ImageIORegion & requested ) const;

  /** Determine if the ImageIO can stream reading from this
   *  file. Default is false. */
  virtual bool CanStreamRead()
    {
    return true;
    }

  /** Determine if the ImageIO can stream writing to this
      file. Default is false. */
  virtual bool CanStreamWrite()
    {
    return false;
    }

  /** Determing the subsampling factor in case
   *  we want a coarse version of the image/
   * \Warning this is only used when streaming is on. */
  itkSetMacro(SubSamplingFactor,unsigned int);
  itkGetMacro(SubSamplingFactor,unsigned int);

protected:
  MetaImageIO();
  ~MetaImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;
  
private:
  
  MetaImage m_MetaImage;

  MetaImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_SubSamplingFactor;
  
};

} // end namespace itk

#endif // __itkMetaImageIO_h
