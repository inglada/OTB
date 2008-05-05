/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMedianProjectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-02-08 04:52:21 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMedianProjectionImageFilter_h
#define __itkMedianProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"
#include "itkConceptChecking.h"

#include <vector>
#include <algorithm>

namespace itk {
/** \class MedianProjectionImageFilter
 * \brief Median projection
 *
 * This class was contributed to the Insight Journal by Gaetan Lehmann. 
 * The original paper can be found at 
 *      http://insight-journal.org/midas/handle.php?handle=1926/164
 *
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction,
 * INRA de Jouy-en-Josas, France.
 *
 * \sa ProjectionImageFilter
 * \sa StandardDeviationProjectionImageFilter
 * \sa SumProjectionImageFilter
 * \sa BinaryProjectionImageFilter
 * \sa MaximumProjectionImageFilter
 * \sa MinimumProjectionImageFilter
 * \sa MeanProjectionImageFilter
 */


namespace Function {
template <class TInputPixel>
class MedianAccumulator
{
public:
  MedianAccumulator( unsigned long size )
    {
    m_Values.reserve( size );
    }
  ~MedianAccumulator(){}

  inline void Initialize()
    {
    m_Values.clear();
    }

  inline void operator()( const TInputPixel &input )
    {
    m_Values.push_back( input );
    }

  inline TInputPixel GetValue()
    {
    typedef typename std::vector<TInputPixel>::iterator ContainerIterator;
    ContainerIterator medianIterator = m_Values.begin() +  m_Values.size() / 2;
    std::nth_element(m_Values.begin(), medianIterator, m_Values.end() );
    return *medianIterator;
    }

  std::vector<TInputPixel> m_Values;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT MedianProjectionImageFilter : public
  ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::MedianAccumulator< typename TInputImage::PixelType > >
{
public:
  typedef MedianProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::MedianAccumulator< 
      typename TInputImage::PixelType > > Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(MedianProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

protected:
  MedianProjectionImageFilter() {}
  virtual ~MedianProjectionImageFilter() {}

private:
  MedianProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


}; // end MedianProjectionImageFilter

} //end namespace itk

#endif
