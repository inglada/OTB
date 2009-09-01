/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStandardDeviationProjectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:35 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStandardDeviationProjectionImageFilter_h
#define __itkStandardDeviationProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"
#include "itkConceptChecking.h"

namespace itk {
/** \class StandardDeviationProjectionImageFilter
 * \brief Mean projection
 * 
 * This class was contributed to the Insight Journal by Gaetan Lehmann.
 * The original paper can be found at 
 *      http://insight-journal.org/midas/handle.php?handle=1926/164
 * 
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, 
 * INRA de Jouy-en-Josas, France.
 * 
 * 
 * \sa ProjectionImageFilter
 * \sa MedianProjectionImageFilter
 * \sa MeanProjectionImageFilter
 * \sa SumProjectionImageFilter
 * \sa MeanProjectionImageFilter
 * \sa MaximumProjectionImageFilter
 * \sa MinimumProjectionImageFilter
 * \sa BinaryProjectionImageFilter
 */


namespace Function {
template <class TInputPixel, class TAccumulate>
class StandardDeviationAccumulator
{
public:
  typedef typename NumericTraits<TInputPixel>::RealType RealType;

  StandardDeviationAccumulator( unsigned long size )
    {
    m_Size = size;
    m_Values.reserve( size );
    }
  ~StandardDeviationAccumulator(){}

  inline void Initialize()
    {
    m_Sum = NumericTraits< TAccumulate >::Zero;
    m_Values.clear();
    }

  inline void operator()( const TInputPixel &input ) 
    {
    m_Sum = m_Sum + input;
    m_Values.push_back( input );
    }

  inline RealType GetValue()
    {
    // to avoid division by zero
    if( m_Size <= 1 )
      return NumericTraits<RealType>::Zero;

    typename NumericTraits<TInputPixel>::RealType mean =
      ((RealType) m_Sum) / m_Size;
    typename std::vector<TInputPixel>::iterator it;
    RealType squaredSum = NumericTraits<RealType>::Zero;
    for( it = m_Values.begin(); it != m_Values.end(); it++ )
      {
      squaredSum += vnl_math_sqr(*it - mean);
      }
    return vcl_sqrt( squaredSum / ( m_Size - 1) );
    }

  TAccumulate              m_Sum;
  unsigned long            m_Size;
  std::vector<TInputPixel> m_Values;
};
} // end namespace Function


template <class TInputImage, 
          class TOutputImage, 
          class TAccumulate= ITK_TYPENAME 
          NumericTraits< ITK_TYPENAME TOutputImage::PixelType >
                           ::AccumulateType >
class ITK_EXPORT StandardDeviationProjectionImageFilter :
    public
    ProjectionImageFilter<TInputImage, TOutputImage,
      Function::StandardDeviationAccumulator< ITK_TYPENAME 
                         TInputImage::PixelType, TAccumulate > >
{
public:
  typedef StandardDeviationProjectionImageFilter Self;

  typedef ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::StandardDeviationAccumulator< typename 
                  TInputImage::PixelType, TAccumulate > > Superclass;

  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PixelType InputPixelType; 

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(StandardDeviationProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputPixelToOutputPixelTypeGreaterAdditiveOperatorCheck,
    (Concept::AdditiveOperators<TAccumulate,
                                InputPixelType,
                                TAccumulate>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputPixelType>));

  itkConceptMacro(AccumulateHasNumericTraitsCheck,
    (Concept::HasNumericTraits<TAccumulate>));
 
  /** End concept checking */
#endif


protected:
  StandardDeviationProjectionImageFilter() {}
  virtual ~StandardDeviationProjectionImageFilter() {}

private:
  //purposely not implemented
  StandardDeviationProjectionImageFilter(const Self&); 

  void operator=(const Self&); //purposely not implemented

}; // end StandardDeviationProjectionImageFilter

} //end namespace itk

#endif
