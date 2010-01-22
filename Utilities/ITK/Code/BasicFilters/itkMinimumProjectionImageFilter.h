/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumProjectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:31 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMinimumProjectionImageFilter_h
#define __itkMinimumProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"
#include "itkConceptChecking.h"

namespace itk {
/** \class MinimumProjectionImageFilter
 * \brief Minimum projection
 *
 * This class was contributed to the Insight Journal by Gaetan Lehmann.
 * The original paper can be found at 
 *      http://insight-journal.org/midas/handle.php?handle=1926/164
 * 
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction,
 * INRA de Jouy-en-Josas, France.
 *
 * \sa ProjectionImageFilter
 * \sa StandardDeviationProjectionImageFilter
 * \sa SumProjectionImageFilter
 * \sa BinaryProjectionImageFilter
 * \sa MaximumProjectionImageFilter
 * \sa MeanProjectionImageFilter
 */


namespace Function {
template <class TInputPixel>
class MinimumAccumulator
{
public:
  MinimumAccumulator( unsigned long ) {}
  ~MinimumAccumulator(){}

  inline void Initialize()
    {
    m_Minimum = NumericTraits< TInputPixel >::max();
    }

  inline void operator()( const TInputPixel &input ) 
    {
    m_Minimum = vnl_math_min( m_Minimum, input );
    }

  inline TInputPixel GetValue()
    {
    return m_Minimum;
    }

  TInputPixel m_Minimum;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT MinimumProjectionImageFilter : public
  ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::MinimumAccumulator< typename TInputImage::PixelType > >
{
public:
  typedef MinimumProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::MinimumAccumulator< 
      typename TInputImage::PixelType > > Superclass;

  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PixelType InputPixelType; 
  
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(MinimumProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputPixelTypeGreaterThanComparable,
    (Concept::LessThanComparable<InputPixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  MinimumProjectionImageFilter() {}
  virtual ~MinimumProjectionImageFilter() {}

private:
  MinimumProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end MinimumProjectionImageFilter

} //end namespace itk

#endif
