/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptAdaptiveHistogramEqualizationImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-12-21 19:15:48 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptAdaptiveHistogramEqualizationImageFilter_h
#define __itkOptAdaptiveHistogramEqualizationImageFilter_h


#include <itkBoxImageFilter.h>
#include <itkImage.h>

namespace itk
{
/**
 * \class tAdaptiveHistogramEqualizationImageFilter
 * \brief Power Law Adaptive Histogram Equalization
 *
 * Histogram equalization modifies the contrast in an image. The
 * AdaptiveHistogramEqualizationImageFilter is a superset of many
 * contrast enhancing filters. By modifying its parameters (alpha,
 * beta, and window), the AdaptiveHistogramEqualizationImageFilter can
 * produce an adaptively equalized histogram or a version of unsharp
 * mask (local mean subtraction). Instead of applying a strict
 * histogram equalization in a window about a pixel, this filter
 * prescribes a mapping function (power law) controlled by the
 * parameters alpha and beta.
 *
 * The parameter alpha controls how much the filter acts like the
 * classical histogram equalization method (alpha=0) to how
 * much the filter acts like an unsharp mask (alpha=1).
 *
 * The parameter beta controls how much the filter acts like an
 * unsharp mask (beta=0) to much the filter acts like pass through
 * (beta=1, with alpha=1).
 *
 * The parameter window controls the size of the region over which
 * local statistics are calculated.
 *
 * By altering alpha, beta and window, a host of equalization and unsharp
 * masking filters is available.
 *
 * For detail description, reference "Adaptive Image Contrast
 * Enhancement using Generalizations of Histogram Equalization."
 * J.Alex Stark. IEEE Transactions on Image Processing, May 2000.
 * 
 * \ingroup ImageEnhancement
 */
template <class TImageType>
class ITK_EXPORT AdaptiveHistogramEqualizationImageFilter :
    public BoxImageFilter< TImageType, TImageType >
{
public:
  /**
   * Standard class typedefs
   */ 
  typedef AdaptiveHistogramEqualizationImageFilter     Self;
  typedef ImageToImageFilter< TImageType, TImageType > Superclass;
  typedef SmartPointer<Self>                           Pointer;
  typedef SmartPointer<const Self>                     ConstPointer;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImageType::ImageDimension );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AdaptiveHistogramEqualizationImageFilter, ImageToImageFilter);

  /** Image type typedef support. */
  typedef TImageType                   ImageType;
  typedef typename ImageType::SizeType ImageSizeType;

  /** Set/Get the value of alpha.  Alpha=0 produces the adaptive
   * histogram equalization (provided beta=0).  Alpha=1 produces an
   * unsharp mask. Default is 0.3. */
  itkSetMacro(Alpha, float);
  itkGetMacro(Alpha, float);

  /** Set/Get the value of beta.  If beta=1 (and alpha=1),
   * then the output image matches the input image.  As beta
   * approaches 0, the filter behaves as an unsharp mask. Default is
   * 0.3. */
  itkSetMacro(Beta, float);
  itkGetMacro(Beta, float);

  /** Set/Get whether an optimized lookup table for the intensity
   * mapping function is used.  Default is off. */
  itkSetMacro(UseLookupTable, bool);
  itkGetMacro(UseLookupTable, bool);
  itkBooleanMacro(UseLookupTable);

protected:
  AdaptiveHistogramEqualizationImageFilter()
    {
    m_Alpha = .3;
    m_Beta = .3;
    this->SetRadius( 5 );
    m_UseLookupTable = false;
    }
  virtual ~AdaptiveHistogramEqualizationImageFilter(){}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /**
   * Standard pipeline method
   */
  void GenerateData();

private:
  AdaptiveHistogramEqualizationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The beta parameter of the AdaptiveHistogramEqualization. */
  float m_Alpha;

  /** The alpha parameter of the AdaptiveHistogramEqualization. */
  float m_Beta;

  /** Should we use a lookup table to optimize the use of the
   * intensity mapping function? */
  bool m_UseLookupTable;

  /** A function which is used in GenerateData(). */
  float CumulativeFunction(float u, float v);
   
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOptAdaptiveHistogramEqualizationImageFilter.txx"
#endif

#endif
