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
#ifndef __otbImageFileReader_h
#define __otbImageFileReader_h

#include "itkImageFileReader.h"
#include "otbCurlHelperInterface.h"
#include "otbImageKeywordlist.h"

namespace otb
{

/** \class ImageFileReader
 * \brief Resource to read an image from a file.
 *
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup IOFilters
 *
 */
template <class TOutputImage>
class ITK_EXPORT ImageFileReader : public itk::ImageFileReader<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageFileReader                    Self;
  typedef itk::ImageFileReader<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, itk::ImageFileReader);

  /** The pixel type of the output image. */
  typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType SizeType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType ImageRegionType;

  /** The pixel type of the output image. */
  //typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** Prepare image allocation at the first call of the pipeline processing */
  virtual void GenerateOutputInformation(void);

  /** Does the real work. */
  virtual void GenerateData();

  /** Give the reader a chance to indicate that it will produce more
   * output than it was requested to produce. ImageFileReader cannot
   * currently read a portion of an image (since the ImageIO objects
   * cannot read a portion of an image), so the ImageFileReader must
   * enlarge the RequestedRegion to the size of the image on disk. */
  virtual void EnlargeOutputRequestedRegion(itk::DataObject *output);

  /** Set/Get the dataset index to extract (starting at 0)
   * used in the case of image file containing multiple datasets
   * (Modis hdf files for example) */
  itkSetMacro(DatasetNumber, unsigned int);
  itkGetMacro(DatasetNumber, unsigned int);

  itkSetObjectMacro(Curl, CurlHelperInterface);

protected:
  ImageFileReader();
  virtual ~ImageFileReader();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  unsigned int m_DatasetNumber;

private:
  /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      approriate message will be thrown. */
  void TestFileExistanceAndReadability();

  /** Generate the filename (for GDALImageI for example). If filename is a directory, look if is a
    * CEOS product (file "DAT...") In this case, the GdalFileName contain the open image file.
    */
  bool GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName);

  ImageFileReader(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string m_ExceptionMessage;

  CurlHelperInterface::Pointer m_Curl;
};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileReader.txx"
#endif

#endif // __otbImageFileReader_h
