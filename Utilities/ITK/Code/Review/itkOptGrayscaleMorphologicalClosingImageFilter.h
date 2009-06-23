/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptGrayscaleMorphologicalClosingImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-09-29 19:07:46 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptGrayscaleMorphologicalClosingImageFilter_h
#define __itkOptGrayscaleMorphologicalClosingImageFilter_h

#include "itkKernelImageFilter.h"
#include "itkMovingHistogramErodeImageFilter.h"
#include "itkMovingHistogramDilateImageFilter.h"
#include "itkBasicErodeImageFilter.h"
#include "itkBasicDilateImageFilter.h"
#include "itkAnchorCloseImageFilter.h"
#include "itkVanHerkGilWermanErodeImageFilter.h"
#include "itkVanHerkGilWermanDilateImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkFlatStructuringElement.h"
#include "itkNeighborhood.h"

namespace itk {

/**
 * \class GrayscaleMorphologicalClosingImageFilter
 * \brief gray scale dilation of an image
 *
 * Erode an image using grayscale morphology. Dilation takes the
 * maximum of all the pixels identified by the structuring element.
 *
 * The structuring element is assumed to be composed of binary
 * values (zero or one). Only elements of the structuring element
 * having values > 0 are candidates for affecting the center pixel.
 * 
 * \sa MorphologyImageFilter, GrayscaleFunctionErodeImageFilter, BinaryErodeImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */

template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT GrayscaleMorphologicalClosingImageFilter : 
    public KernelImageFilter<TInputImage, TOutputImage, TKernel>
{
public:
  /** Standard class typedefs. */
  typedef GrayscaleMorphologicalClosingImageFilter              Self;
  typedef KernelImageFilter<TInputImage,TOutputImage, TKernel>  Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(GrayscaleMorphologicalClosingImageFilter, 
               KernelImageFilter);
  
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
                      
  /** Image related typedefs. */
  typedef TInputImage                                           InputImageType;
  typedef TOutputImage                                          OutputImageType;
  typedef typename TInputImage::RegionType                      RegionType;
  typedef typename TInputImage::SizeType                        SizeType;
  typedef typename TInputImage::IndexType                       IndexType;
  typedef typename TInputImage::PixelType                       PixelType;
  typedef typename TInputImage::OffsetType                      OffsetType;
  typedef typename Superclass::OutputImageRegionType            OutputImageRegionType;

  typedef FlatStructuringElement< itkGetStaticConstMacro(ImageDimension) >
                                                                FlatKernelType;
  typedef MovingHistogramErodeImageFilter< TInputImage, TOutputImage, TKernel >
                                                                HistogramErodeFilterType;
  typedef MovingHistogramDilateImageFilter< TInputImage, TOutputImage, TKernel >
                                                                HistogramDilateFilterType;
  typedef BasicDilateImageFilter< TInputImage, TInputImage, TKernel >
                                                                BasicDilateFilterType;
  typedef BasicErodeImageFilter< TInputImage, TOutputImage, TKernel >
                                                                BasicErodeFilterType;
  typedef AnchorCloseImageFilter< TInputImage, FlatKernelType > AnchorFilterType;
  typedef VanHerkGilWermanErodeImageFilter< TInputImage, FlatKernelType >
                                                                VanHerkGilWermanErodeFilterType;
  typedef VanHerkGilWermanDilateImageFilter< TInputImage, FlatKernelType >
                                                                VanHerkGilWermanDilateFilterType;
  typedef CastImageFilter< TInputImage, TOutputImage >          SubtractFilterType;
  
  /** Kernel typedef. */
  typedef TKernel KernelType;
//   typedef typename KernelType::Superclass KernelSuperClass;
//   typedef Neighborhood< typename KernelType::PixelType, ImageDimension > KernelSuperClass;
  
  /** Set kernel (structuring element). */
  void SetKernel( const KernelType& kernel );
  
  /** Set/Get the backend filter class. */
  void SetAlgorithm(int algo );
  itkGetMacro(Algorithm, int);
  
  /** GrayscaleMorphologicalClosingImageFilter need to set its internal filters as modified */
  virtual void Modified() const;

  /** define values used to determine which algorithm to use */
  enum {
    BASIC = 0,
    HISTO = 1,
    ANCHOR = 2,
    VHGW = 3
  } AlgorithmChoice;


  /** A safe border is added to input image to avoid borders effects
   * and remove it once the closing is done */
  itkSetMacro(SafeBorder, bool);
  itkGetConstReferenceMacro(SafeBorder, bool);
  itkBooleanMacro(SafeBorder);

protected:
  GrayscaleMorphologicalClosingImageFilter();
  ~GrayscaleMorphologicalClosingImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  void GenerateData();

private:
  GrayscaleMorphologicalClosingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // the filters used internally
  typename HistogramErodeFilterType::Pointer         m_HistogramErodeFilter;
  typename HistogramDilateFilterType::Pointer        m_HistogramDilateFilter;
  typename BasicErodeFilterType::Pointer             m_BasicErodeFilter;
  typename BasicDilateFilterType::Pointer            m_BasicDilateFilter;
  typename VanHerkGilWermanDilateFilterType::Pointer m_VanHerkGilWermanDilateFilter;
  typename VanHerkGilWermanErodeFilterType::Pointer  m_VanHerkGilWermanErodeFilter;
  typename AnchorFilterType::Pointer                 m_AnchorFilter;

  // and the name of the filter
  int m_Algorithm;

  bool m_SafeBorder;

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOptGrayscaleMorphologicalClosingImageFilter.txx"
#endif

#endif
