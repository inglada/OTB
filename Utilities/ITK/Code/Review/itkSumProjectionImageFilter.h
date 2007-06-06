/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSumProjectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/01/23 23:44:22 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSumProjectionImageFilter_h
#define __itkSumProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"
#include "itkConceptChecking.h"

namespace itk {
/** \class SumProjectionImageFilter
 * \brief Sum projection
 *
 * This class was contributed to the Insight Journal by Gaetan Lehmann.
 * The original paper can be found at 
 *          http://hdl.handle.net/1926/164
 *
 * \author Gaetan Lehmann. Biologie du D�veloppement et de la Reproduction
 * , INRA de Jouy-en-Josas, France.
 *
 * \sa ProjectionImageFilter
 * \sa MedianProjectionImageFilter
 * \sa MeanProjectionImageFilter
 * \sa MeanProjectionImageFilter
 * \sa MaximumProjectionImageFilter
 * \sa MinimumProjectionImageFilter
 * \sa BinaryProjectionImageFilter
 * \sa StandardDeviationProjectionImageFilter
 */


namespace Function {
template <class TInputPixel, class TOuputPixel>
class SumAccumulator
{
public:
  SumAccumulator( unsigned long size ) {}
  ~SumAccumulator(){}

  inline void Initialize()
    {
    m_Sum = NumericTraits< TOuputPixel >::Zero;
    }

  inline TInputPixel operator()( const TInputPixel &input )
    {
    m_Sum = m_Sum + input;
    return m_Sum;
    }

  inline TOuputPixel GetValue()
    {
    return m_Sum;
    }

  TOuputPixel m_Sum;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT SumProjectionImageFilter :
    public
    ProjectionImageFilter<TInputImage, TOutputImage,
      Function::SumAccumulator< 
          typename TInputImage::PixelType, typename TOutputImage::PixelType > >
{
public:
  typedef SumProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::SumAccumulator< 
             typename TInputImage::PixelType, 
             typename TOutputImage::PixelType > > Superclass;

  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PixelType InputPixelType; 

  typedef TOutputImage                        OutputImageType;
  typedef typename OutputImageType::PixelType OutputPixelType; 


  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(SumProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputPixelToOutputPixelTypeGreaterAdditiveOperatorCheck,
    (Concept::AdditiveOperators<OutputPixelType,
                                InputPixelType,
                                OutputPixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  SumProjectionImageFilter() {}
  virtual ~SumProjectionImageFilter() {}

private:
  SumProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end SumProjectionImageFilter

} //end namespace itk

#endif
