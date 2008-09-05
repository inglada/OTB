/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarToArrayCastImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006-03-30 15:36:25 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarToArrayCastImageFilter_h
#define __itkScalarToArrayCastImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{
  
/** \class ScalarToArrayCastImageFilter
 *
 * \brief Creates the output image with vector type pixels filled with
 * the intensity values from one or more input images with scalar
 * pixels.
 *
 * This filter is templated over the input image type and 
 * output image type. The each dimension of the output image pixel is
 * filled with each input image pixel's scalar pixel value. This
 * filter can be used to cast a scalar image to a vector image if
 * there is only one input image.
 *
 * \ingroup Multithreaded
 */


template <class TInputImage, class TOutputImage>
class ITK_EXPORT ScalarToArrayCastImageFilter :
    public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef ScalarToArrayCastImageFilter  Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Standard class macros */
  itkNewMacro(Self);
  itkTypeMacro(ScalarToArrayCastImageFilter, ImageToImageFilter) ;

  typedef typename Superclass::OutputImageRegionType OutputImageRegionType ;
  typedef typename TOutputImage::PixelType OutputImagePixelType ;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename OutputImagePixelType::ValueType>));
  itkConceptMacro(OutputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  ScalarToArrayCastImageFilter() ;
  virtual ~ScalarToArrayCastImageFilter() {}
  
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread,
                            int threadId) ;
 
private:
  ScalarToArrayCastImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarToArrayCastImageFilter.txx"
#endif

#endif
