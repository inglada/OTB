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
#ifndef __otbRAMDrivenStrippedStreamingManager_h
#define __otbRAMDrivenStrippedStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class RAMDrivenStrippedStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  according to a user-defined available RAM
 *
 * You can use SetAvailableRAMInMB to set the available RAM. An estimation of the pipeline
 * memory print will be done, and the number of divisions will then be computed to fit
 * the available RAM
 *
 * \sa StreamingImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT RAMDrivenStrippedStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef RAMDrivenStrippedStreamingManager Self;
  typedef StreamingManager<TImage>          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(RAMDrivenStrippedStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkSetMacro(AvailableRAMInMB, unsigned int);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkGetConstMacro(AvailableRAMInMB, unsigned int);

  /** The multiplier to apply to the memory print estimation */
  itkSetMacro(Bias, double);

  /** The multiplier to apply to the memory print estimation */
  itkGetConstMacro(Bias, double);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

protected:
  RAMDrivenStrippedStreamingManager();
  virtual ~RAMDrivenStrippedStreamingManager();

  /** The splitter type used to generate the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /** The number of MegaBytes of RAM available */
  unsigned int m_AvailableRAMInMB;

  /** The multiplier to apply to the memory print estimation */
  double m_Bias;

private:
  RAMDrivenStrippedStreamingManager(const RAMDrivenStrippedStreamingManager &);
  void operator =(const RAMDrivenStrippedStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAMDrivenStrippedStreamingManager.txx"
#endif

#endif

