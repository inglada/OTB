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
#ifndef __otb_ScalarImageToPanTexTextureFilter_h
#define __otb_ScalarImageToPanTexTextureFilter_h

#include "itkImageToImageFilter.h"

#include "otbMaskedScalarImageToGreyLevelCoocurenceMatrixGenerator.h"

namespace otb
{
/** \class ScalarImageToPanTexTextureFilter 
 *  \brief This class computes a texture derived built-up precense index (PanTex)
 *
 * This class computes a texture-derived built-up presence index (PanTex) from textural
 * characteristics of scalar images.It is the min value of the contrast in 8 directions.
 *
 * Print references:
 *
 * Pesari, M., A. Gerhardinger, F. Kayitakire. 2008.  A robust built-up area precense
 * index by anisotropic rotation-invariant textural measure. 
 * IEEE Journal of selected topics in applied earth observations and remote sensing. 
 * Vol1,NO3.
 *
 *
 * \sa ScalarImageToGreyLevelCooccurrenceMatrixGenerator
 * \sa GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator
 * \sa ScalarImageTextureCalculator
 *
 */

template<class TInpuImage, class TOutputImage>
class ScalarImageToPanTexTextureFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToPanTexTextureFilter                            Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage>      Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToPanTexTextureFilter,ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                                             InputImageType;
  typedef typename InputImageType::Pointer                       InputImagePointerType;
  typedef typename InputImageType::PixelType                     InputPixelType;
  typedef typename InputImageType::RegionType                    InputRegionType;
  typedef typename InputRegionType::SizeType                     SizeType;
  typedef TOutputImage                                           OutputImageType;
  typedef typename OutputImageType::Pointer                      OutputImagePointerType;
  typedef typename OutputImageType::RegionType                   OutputRegionType;

  /** Co-occurence matrix and textures calculator */
  typedef otb::MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
    <InputImageType>                                             CoocurrenceMatrixGeneratorType;
  typedef typename CoocurrenceMatrixGeneratorType::Pointer       CoocurrenceMatrixGeneratorPointerType;
  typedef typename CoocurrenceMatrixGeneratorType::OffsetType    OffsetType;
  typedef typename std::vector<OffsetType>                       OffsetListType;
  typedef typename CoocurrenceMatrixGeneratorType::HistogramType HistogramType;
  typedef typename HistogramType::FrequencyType                  FrequencyType;
  typedef typename HistogramType::MeasurementType                MeasurementType;
  typedef typename HistogramType::Iterator HistogramIterator;
  
  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius,SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius,SizeType);

  /** Set the number of bin per axis for histogram generation */
  itkSetMacro(NumberOfBinsPerAxis,unsigned int);

  /** Get the number of bin per axis for histogram generation */
  itkGetMacro(NumberOfBinsPerAxis,unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum,InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum,InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum,InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum,InputPixelType);

protected:
  /** Constructor */
  ScalarImageToPanTexTextureFilter();
  /** Destructor */
  ~ScalarImageToPanTexTextureFilter();
  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();
  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType & outputRegion, int threadId);

private:
  ScalarImageToPanTexTextureFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType & region1, const OutputRegionType & region2);

  /** Radius of the window on which to compute textures */
  SizeType                                                       m_Radius;

  /** List of Offsets for co-occurence */
  OffsetListType                                                     m_OffsetList;

  /** Number of bins per axis for histogram generation */
  unsigned int                                                   m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType                                                 m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType                                                 m_InputImageMaximum;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToPanTexTextureFilter.txx"
#endif

#endif
