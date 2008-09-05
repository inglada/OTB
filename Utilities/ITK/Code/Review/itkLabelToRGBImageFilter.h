/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelToRGBImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007-10-22 12:54:03 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkLabelToRGBImageFilter_h
#define __itkLabelToRGBImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkLabelToRGBFunctor.h"

namespace itk
{

/** \class LabelToRGBImageFilter
 * \brief Apply a colormap to a label image
 *
 * Apply a colormap to a label image. The set of colors
 * is a good selection of distinct colors. The user can choose to use a background
 * value. In that case, a gray pixel with the same intensity than the background
 * label is produced.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 * \author Richard Beare. Department of Medicine, Monash University, Melbourne, Australia.
 *
 * \sa ScalarToRGBPixelFunctor LabelOverlayImageFilter
 * \ingroup Multithreaded
 *
 */
template <class TLabelImage, typename  TOutputImage>
class ITK_EXPORT LabelToRGBImageFilter :
    public
UnaryFunctorImageFilter<TLabelImage, TOutputImage, 
                        Functor::LabelToRGBFunctor< 
  typename TLabelImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef LabelToRGBImageFilter     Self;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef UnaryFunctorImageFilter<TLabelImage, TOutputImage, 
                        Functor::LabelToRGBFunctor< 
                            typename TLabelImage::PixelType, 
                            typename TOutputImage::PixelType>   >  Superclass;

  typedef TOutputImage OutputImageType;
  typedef TLabelImage  LabelImageType;

  typedef typename TOutputImage::PixelType                     OutputPixelType;
  typedef typename TLabelImage::PixelType                      LabelPixelType;
  typedef typename NumericTraits< OutputPixelType >::ValueType OutputPixelValueType;

  /** Runtime information support. */
  itkTypeMacro(LabelToRGBImageFilter, UnaryFunctorImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Set/Get the background value */
  itkSetMacro( BackgroundValue, LabelPixelType );
  itkGetConstReferenceMacro( BackgroundValue, LabelPixelType );

  /** Set/Get the background color in the output image */
  itkSetMacro( BackgroundColor, OutputPixelType );
  itkGetConstReferenceMacro( BackgroundColor, OutputPixelType );

protected:
  LabelToRGBImageFilter();
  virtual ~LabelToRGBImageFilter() {};

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void);

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const;
  
private:
  LabelToRGBImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  OutputPixelType   m_BackgroundColor;
  LabelPixelType    m_BackgroundValue;
};


  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelToRGBImageFilter.txx"
#endif
  
#endif
