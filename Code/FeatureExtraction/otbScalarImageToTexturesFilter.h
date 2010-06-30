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
#ifndef __otbScalarImageToTexturesFilter_h
#define __otbScalarImageToTexturesFilter_h

#include "itkImageToImageFilter.h"

#include "otbMaskedScalarImageToGreyLevelCoocurenceMatrixGenerator.h"
#include "itkGreyLevelCooccurrenceMatrixTextureCoefficientsCalculator.h"

namespace otb
{
/** \class ScalarImageToTexturesFilter
 *  \brief This class compute 8 local Haralick textures features.
 *
 *  This filter computes the following Haralick textures over a sliding windows with
 *  user defined radius:
 *  (where \f$ g(i, j) \f$ is the element in
 * cell i, j of a a normalized GLCM):
 *
 * "Energy" \f$ = f_1 = \sum_{i,j}g(i, j)^2 \f$
 *
 * "Entropy" \f$ = f_2 = -\sum_{i,j}g(i, j) \log_2 g(i, j)\f$, or 0 if \f$g(i, j) = 0\f$
 *
 * "Correlation" \f$ = f_3 = \sum_{i,j}\frac{(i - \mu)(j - \mu)g(i, j)}{\sigma^2} \f$
 *
 * "Difference Moment" \f$= f_4 = \sum_{i,j}\frac{1}{1 + (i - j)^2}g(i, j) \f$
 *
 * "Inertia" \f$ = f_5 = \sum_{i,j}(i - j)^2g(i, j) \f$ (sometimes called "contrast.")
 *
 * "Cluster Shade" \f$ = f_6 = \sum_{i,j}((i - \mu) + (j - \mu))^3 g(i, j) \f$
 *
 * "Cluster Prominence" \f$ = f_7 = \sum_{i,j}((i - \mu) + (j - \mu))^4 g(i, j) \f$
 *
 * "Haralick's Correlation" \f$ = f_8 = \frac{\sum_{i,j}(i, j) g(i, j) -\mu_t^2}{\sigma_t^2} \f$
 * where \f$\mu_t\f$ and \f$\sigma_t\f$ are the mean and standard deviation of the row
 * (or column, due to symmetry) sums.
 *
 * Above, \f$ \mu =  \f$ (weighted pixel average) \f$ = \sum_{i,j}i \cdot g(i, j) =
 * \sum_{i,j}j \cdot g(i, j) \f$ (due to matrix summetry), and
 *
 * \f$ \sigma =  \f$ (weighted pixel variance) \f$ = \sum_{i,j}(i - \mu)^2 \cdot g(i, j) =
 * \sum_{i,j}(j - \mu)^2 \cdot g(i, j)  \f$  (due to matrix summetry)
 *
 * Neighborhood size can be set using the SetRadius() method. Offset for co-occurence estimation
 * is set using the SetOffset() method.
 *
 * \sa otb::MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
 * \sa itk::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToTexturesFilter                            Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage>      Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToTexturesFilter,ImageToImageFilter);

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
  typedef typename CoocurrenceMatrixGeneratorType::HistogramType HistogramType;
  typedef itk::Statistics::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator
  <HistogramType>                                                TextureCoefficientsCalculatorType;
  typedef typename TextureCoefficientsCalculatorType::Pointer    TextureCoefficientsCalculatorPointerType;

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius,SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius,SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset,OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset,OffsetType);

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

  /** Get the energy output image */
  OutputImageType * GetEnergyOutput();

  /** Get the entropy output image */
  OutputImageType * GetEntropyOutput();

  /** Get the correlation output image */
  OutputImageType * GetCorrelationOutput();

  /** Get the inverse difference moment output image */
  OutputImageType * GetInverseDifferenceMomentOutput();

  /** Get the inertia output image */
  OutputImageType * GetInertiaOutput();

  /** Get the cluster shade output image */
  OutputImageType * GetClusterShadeOutput();

  /** Get the cluster prominence image */
  OutputImageType * GetClusterProminenceOutput();

  /** Get the Haralick correlation output image */
  OutputImageType * GetHaralickCorrelationOutput();

protected:
  /** Constructor */
  ScalarImageToTexturesFilter();
  /** Destructor */
  ~ScalarImageToTexturesFilter();
  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();
  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType & outputRegion, int threadId);

private:
  ScalarImageToTexturesFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType & region1, const OutputRegionType & region2);

  /** Radius of the window on which to compute textures */
  SizeType                                                       m_Radius;

  /** Offset for co-occurence */
  OffsetType                                                     m_Offset;

  /** Number of bins per axis for histogram generation */
  unsigned int                                                   m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType                                                 m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType                                                 m_InputImageMaximum;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToTexturesFilter.txx"
#endif

#endif
