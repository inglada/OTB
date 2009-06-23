/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReflectImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-17 16:30:50 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkReflectImageFilter_h
#define __itkReflectImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{
  
/** \class ReflectImageFilter
 * \brief Implements a Reflection of an image along a selected direction.
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.  
 * 
 * \ingroup   IntensityImageFilters     Singlethreaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReflectImageFilter : public ImageToImageFilter<TInputImage,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef ReflectImageFilter                            Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ReflectImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                            InputImageType;
  typedef typename    InputImageType::Pointer    InputImagePointer;
  typedef typename    InputImageType::RegionType InputImageRegionType; 
  typedef typename    InputImageType::PixelType  InputImagePixelType; 

  typedef TOutputImage                             OutputImageType;
  typedef typename     OutputImageType::Pointer    OutputImagePointer;
  typedef typename     OutputImageType::RegionType OutputImageRegionType;
  typedef typename     OutputImageType::PixelType  OutputImagePixelType;

  /** Set the direction in which to reflect the data. */
  itkGetMacro( Direction, unsigned int );
  itkSetMacro( Direction, unsigned int );

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<InputImagePixelType, OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  ReflectImageFilter();
  virtual ~ReflectImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** This method implements the actual reflection of the image.
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void GenerateData(void);

private:
  ReflectImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_Direction;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkReflectImageFilter.txx"
#endif

#endif
