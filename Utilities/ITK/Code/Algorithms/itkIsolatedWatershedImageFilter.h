/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIsolatedWatershedImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:53:35 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIsolatedWatershedImageFilter_h
#define __itkIsolatedWatershedImageFilter_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"

namespace itk
{

/** \class IsolatedWatershedImageFilter
 * \brief Isolate watershed basins using two seeds
 *
 * IsolatedWatershedImageFilter labels pixels with ReplaceValue1 that
 * are in the same watershed basin as Seed1 AND NOT the same as
 * Seed2. The filter adjusts the waterlevel until the two seeds are
 * not in different basins. The user supplies a Watershed
 * threshold. The algorithm uses a binary search to adjust the upper
 * waterlevel, starting at UpperValueLimit. UpperValueLimit defaults
 * to the 1.0.
 * \ingroup WatershedSegmentation
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT IsolatedWatershedImageFilter:
    public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef IsolatedWatershedImageFilter                 Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                           Pointer;
  typedef SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).  */
  itkTypeMacro(IsolatedWatershedImageFilter,
               ImageToImageFilter);

  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType; 
  typedef typename InputImageType::PixelType    InputImagePixelType; 
  typedef typename InputImageType::IndexType    IndexType;
  typedef typename InputImageType::SizeType     SizeType;
  
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType; 
  typedef typename OutputImageType::PixelType  OutputImagePixelType; 
  
  typedef WatershedImageFilter<InputImageType>                         WatershedType;
  typedef GradientMagnitudeImageFilter<InputImageType,OutputImageType> GradientMagnitudeType;
  void PrintSelf ( std::ostream& os, Indent indent ) const;

  /** Set seed point 1. This seed will be isolated from Seed2 (if
   *  possible). All pixels connected to this seed will be replaced  
   *  with ReplaceValue1. */
  itkSetMacro(Seed1, IndexType);

  /** Set seed point 2. This seed will be isolated from Seed1 (if
   *  possible). All pixels connected to this seed will be replaced
   *  with ReplaceValue2. */ 
  itkSetMacro(Seed2, IndexType);

  /** Set/Get the Watershed threshold. The default is 0. */
  itkSetMacro(Threshold, double);
  itkGetConstMacro(Threshold, double);

  /** Set/Get the precision required for the intensity threshold
   *  value. The default is .001. */
  itkSetMacro(IsolatedValueTolerance, double);
  itkGetConstMacro(IsolatedValueTolerance, double);

  /** Set/Get the limit on the upper waterlevel value. The default is
   *  1.0. */
  itkSetMacro(UpperValueLimit, double);
  itkGetConstMacro(UpperValueLimit, double);


  /** Set/Get value to replace Seed1(Seed2) pixels, pixels that are
   *  within the basin that contains Seed1(Seed2) this  value. The
   *  default is 1(0). */
  itkSetMacro(ReplaceValue1, OutputImagePixelType);
  itkGetConstMacro(ReplaceValue1, OutputImagePixelType);
  itkSetMacro(ReplaceValue2, OutputImagePixelType);
  itkGetConstMacro(ReplaceValue2, OutputImagePixelType);

  /** Get value that isolates the two seeds. */
  itkGetConstMacro(IsolatedValue, double);

protected:
  IsolatedWatershedImageFilter();
  ~IsolatedWatershedImageFilter(){};
  IndexType            m_Seed1;
  IndexType            m_Seed2;
  OutputImagePixelType m_ReplaceValue1;
  OutputImagePixelType m_ReplaceValue2;

  typename GradientMagnitudeType::Pointer m_GradientMagnitude;
  typename WatershedType::Pointer         m_Watershed;

  double m_Threshold;
  double m_IsolatedValue;
  double m_IsolatedValueTolerance;
  double m_UpperValueLimit;

  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion();

  // Override since the filter produces the entire dataset
  void EnlargeOutputRequestedRegion(DataObject *output);

  void GenerateData();
  
private:
  IsolatedWatershedImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIsolatedWatershedImageFilter.txx"
#endif

#endif
