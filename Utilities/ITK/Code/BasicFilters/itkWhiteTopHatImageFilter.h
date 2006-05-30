/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWhiteTopHatImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:16:38 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWhiteTopHatImageFilter_h
#define __itkWhiteTopHatImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class WhiteTopHatImageFilter
 * \brief White top hat extract local maxima that are larger than the structuring element
 *
 * Top-hats are described in Chapter 4.5 of Pierre Soille's book 
 * "Morphological Image Analysis: Principles and Applications", 
 * Second Edition, Springer, 2003.
 * 
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA of Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT WhiteTopHatImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef WhiteTopHatImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
 /** Kernel typedef. */
  typedef TKernel KernelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(WhiteTopHatImageFilter, 
               ImageToImageFilter);

  /** Set kernel (structuring element). */
  itkSetMacro(Kernel, KernelType);
  
  /** Get the kernel (structuring element). */
  itkGetConstReferenceMacro(Kernel, KernelType);

protected:
  WhiteTopHatImageFilter();
  ~WhiteTopHatImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** WhiteTopHatImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** WhiteTopHatImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  void GenerateData();
  

private:
  WhiteTopHatImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** kernel or structuring element to use. */
  KernelType m_Kernel ;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWhiteTopHatImageFilter.txx"
#endif

#endif


