/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptBlackTopHatImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-09-29 19:07:46 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptBlackTopHatImageFilter_h
#define __itkOptBlackTopHatImageFilter_h

#include "itkKernelImageFilter.h"

namespace itk {

/** \class BlackTopHatImageFilter
 * \brief Black top hat extract local minima that are smaller than the structuring element
 *
 * Black top hat extract local minima that are smaller than the structuring
 * element. It subtract the background in the input image.
 * The output of the filter transforms the black peaks in white peaks.
 *
 * Top-hats are described in Chapter 4.5 of Pierre Soille's book 
 * "Morphological Image Analysis: Principles and Applications", 
 * Second Edition, Springer, 2003.
 * 
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT BlackTopHatImageFilter : 
    public KernelImageFilter<TInputImage, TOutputImage, TKernel>
{
public:
  /** Standard class typedefs. */
  typedef BlackTopHatImageFilter                                Self;
  typedef KernelImageFilter<TInputImage, TOutputImage, TKernel> Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                                           InputImageType;
  typedef TOutputImage                                          OutputImageType;
  typedef typename InputImageType::Pointer                      InputImagePointer;
  typedef typename InputImageType::ConstPointer                 InputImageConstPointer;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename InputImageType::PixelType                    InputImagePixelType;
  typedef typename OutputImageType::Pointer                     OutputImagePointer;
  typedef typename OutputImageType::ConstPointer                OutputImageConstPointer;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename OutputImageType::PixelType                   OutputImagePixelType;
  
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
  itkTypeMacro(BlackTopHatImageFilter, 
               KernelImageFilter);

  /** A safe border is added to input image to avoid borders effects
   * and remove it once the closing is done */
  itkSetMacro(SafeBorder, bool);
  itkGetConstReferenceMacro(SafeBorder, bool);
  itkBooleanMacro(SafeBorder);

  /** define values used to determine which algorithm to use */
  enum {
    BASIC = 0,
    HISTO = 1,
    ANCHOR = 2,
    VHGW = 3
  } AlgorithmChoice;

  /** Set/Get the backend filter class. */
  itkSetMacro(Algorithm, int);
  itkGetMacro(Algorithm, int);
  
  itkSetMacro(ForceAlgorithm, bool);
  itkGetConstReferenceMacro(ForceAlgorithm, bool);
  itkBooleanMacro(ForceAlgorithm);

protected:
  BlackTopHatImageFilter();
  ~BlackTopHatImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();
  

private:
  BlackTopHatImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_SafeBorder;

  int m_Algorithm;

  bool m_ForceAlgorithm;

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOptBlackTopHatImageFilter.txx"
#endif

#endif
