/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHistogramToIntensityImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:37 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHistogramToIntensityImageFilter_h
#define __itkHistogramToIntensityImageFilter_h

#include "itkHistogramToImageFilter.h"

namespace itk
{
  
/** \class HistogramToIntensityImageFilter
 * \brief The class takes a histogram as an input and produces an image
 * as the output. A pixel, at position I,  in the output image is given by
 *  
 * \f[
 * f(I) = q_I
 * \f]
 *  where  \f$q_I\f$ is the frequency of measurement vector, I.
 *  
 *  \sa HistogramToProbabilityImageFilter, HistogramToLogProbabilityImageFilter,
 *  HistogramToImageFilter, HistogramToEntropyImageFilter
 */

namespace Function {  
template< class TInput, class TOutput=unsigned long >
class HistogramIntensityFunction
{
public:
  
  //Intensity function returns pixels of unsigned long.. 
  typedef TOutput  OutputPixelType;
  
  HistogramIntensityFunction(): 
      m_TotalFrequency(1) {}

  ~HistogramIntensityFunction() {};
 
  inline OutputPixelType operator()( const TInput & A ) const
    {
    return static_cast<OutputPixelType>( A );
    }

  void SetTotalFrequency( unsigned long n ) 
    {
    m_TotalFrequency = n;
    }
  
  unsigned long GetTotalFrequency( ) const 
    {
    return m_TotalFrequency;
    }

private:
  unsigned long  m_TotalFrequency;
}; 
}

template <class THistogram, class TOutputPixel=unsigned long >
class ITK_EXPORT HistogramToIntensityImageFilter :
  public HistogramToImageFilter< THistogram, 
  Function::HistogramIntensityFunction< unsigned long, TOutputPixel > > 
{
public:
  
  /** Standard class typedefs. */
  typedef HistogramToIntensityImageFilter                  Self;

  /** Standard "Superclass" typedef. */
  typedef HistogramToImageFilter< THistogram, 
    Function::HistogramIntensityFunction< unsigned long, TOutputPixel > >
                                                           Superclass;

  //typedef typename Function::HistogramIntensityFunction  FunctorType;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;
  
  /** Run-time type information (and related methods).   */
  itkTypeMacro( HistogramToIntensityImageFilter, HistogramToImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  HistogramToIntensityImageFilter() {}
  virtual ~HistogramToIntensityImageFilter() {}
  
private:
  HistogramToIntensityImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
