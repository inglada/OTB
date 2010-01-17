/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToLabelImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-07-30 22:54:24 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMapToLabelImageFilter_h
#define __itkLabelMapToLabelImageFilter_h

#include "itkLabelMapFilter.h"

namespace itk {

/** \class LabelMapToLabelImageFilter
 * \brief Converts a LabelMap to a labeled image.
 *
 * LabelMapToBinaryImageFilter to a label image.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 * \ingroup LabeledImageFilters
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT LabelMapToLabelImageFilter : 
    public LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToLabelImageFilter                Self;
  typedef LabelMapFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TOutputImage                             OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;
  
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelMapToLabelImageFilter, ImageToImageFilter);

protected:
  LabelMapToLabelImageFilter();
  ~LabelMapToLabelImageFilter() {};

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );  

private:
  LabelMapToLabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelMapToLabelImageFilter.txx"
#endif

#endif
