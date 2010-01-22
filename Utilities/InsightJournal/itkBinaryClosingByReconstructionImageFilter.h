/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryClosingByReconstructionImageFilter.h,v $
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryClosingByReconstructionImageFilter_h
#define __itkBinaryClosingByReconstructionImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/**
 * \class BinaryClosingByReconstructionImageFilter
 * \brief binary morphological closing of an image.
 *
 * This filter removes small (i.e., smaller than the structuring 
 * element) holes in the image. It is defined as:
 * Closing(f) = ReconstructionByErosion(Dilation(f)).
 *
 * The structuring element is assumed to be composed of binary
 * values (zero or one). Only elements of the structuring element
 * having values > 0 are candidates for affecting the center pixel.
 * 
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa MorphologyImageFilter, ClosingByReconstructionImageFilter, BinaryOpeningByReconstructionImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */

template<class TInputImage, class TKernel>
class ITK_EXPORT BinaryClosingByReconstructionImageFilter : 
    public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryClosingByReconstructionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryClosingByReconstructionImageFilter, 
               ImageToImageFilter);

  typedef TInputImage InputImageType;
  typedef TInputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;

  /** Declaration of pixel type. */
  typedef typename TInputImage::PixelType InputPixelType ;
  typedef typename TInputImage::PixelType OutputPixelType ;

  /** Kernel typedef. */
  typedef TKernel KernelType;

  /** Set kernel (structuring element). */
  itkSetMacro(Kernel, KernelType);

  /** Get the kernel (structuring element). */
  itkGetConstReferenceMacro(Kernel, KernelType);

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkSetMacro(ForegroundValue, InputPixelType);
 
  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkGetMacro(ForegroundValue, InputPixelType);
 
  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

protected:
  BinaryClosingByReconstructionImageFilter();
  ~BinaryClosingByReconstructionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryClosingByReconstructionImageFilter need to make sure they request enough of an
   * input image to account for the structuring element size.  The input
   * requested region is expanded by the radius of the structuring element.
   * If the request extends past the LargestPossibleRegion for the input,
   * the request is cropped by the LargestPossibleRegion. */
  void GenerateInputRequestedRegion() ;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleDilateImageFilter GrayscaleErodeImageFilter. */
  void  GenerateData ();

private:
  BinaryClosingByReconstructionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** kernel or structuring element to use. */
  KernelType m_Kernel ;
   
  InputPixelType m_ForegroundValue;
  
  bool                m_FullyConnected;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryClosingByReconstructionImageFilter.txx"
#endif

#endif


