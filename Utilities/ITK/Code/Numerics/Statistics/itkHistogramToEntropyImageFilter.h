/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHistogramToEntropyImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:37 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHistogramToEntropyImageFilter_h
#define __itkHistogramToEntropyImageFilter_h

#include "itkHistogramToImageFilter.h"

namespace itk
{
  
/** \class HistogramToEntropyImageFilter
 * \brief The class takes a histogram as an input and gives the entropy
 * image as the output. A pixel, at position I,  in the output image is given by
 *  
 * \f[
 * f(I) = -p \log_2 p
 * \f]
 * 
 * where 
 * \f[
 * p = \frac{q_I}{\sum_{i \in I} q_I}
 * \f]
 *  where  \f$q_I\f$ is the frequency of measurement vector, I.
 *
 * \f$p\f$ is the frequency of a measurement vector by the sum of all frequencies =
 * Probability of the the measurement vector
 * 
 * The output image is of type double.
 * 
 * This is useful in plotting the joint histograms during registration.
 * 
 *  \sa HistogramToImageFilter, HistogramToLogProbabilityImageFilter,
 *  HistogramToIntensityImageFilter, HistogramToProbabilityImageFilter
 * 
 */

namespace Function {  
template< class TInput, class TOutput=double >
class HistogramEntropyFunction
{
public:
  
  //Probability function = Number of occurances in each bin /
  //   Total Number of occurances. 
  //
  // Returns pixels of float.. 
  typedef  TOutput  OutputPixelType;
  
  
  HistogramEntropyFunction(): 
      m_TotalFrequency(1) {}
  
  ~HistogramEntropyFunction() {};
  
  inline OutputPixelType operator()( const TInput & A ) const
    {
    if( A ) 
      {
      const double p = static_cast<OutputPixelType>(A) / 
        static_cast<OutputPixelType>(m_TotalFrequency);
      return static_cast<OutputPixelType>( (-1) * p * vcl_log(p) / vcl_log(2.0)); 
      }
    else
      {
      const double p = static_cast<OutputPixelType>(A+1) / 
        static_cast<OutputPixelType>(m_TotalFrequency);
      return static_cast<OutputPixelType>( (-1) * p * vcl_log(p) / vcl_log(2.0)); 
      }
    }

  void SetTotalFrequency( const unsigned long n ) 
    {
    m_TotalFrequency = n;
    }
  
  unsigned long GetTotalFrequency() const
    {
    return m_TotalFrequency;
    }

private:
  unsigned long  m_TotalFrequency;
}; 
}

template <class THistogram, class TOutputPixel=double >
class ITK_EXPORT HistogramToEntropyImageFilter :
  public HistogramToImageFilter< THistogram, 
  Function::HistogramEntropyFunction< unsigned long, TOutputPixel > > 
{
public:
  
  /** Standard class typedefs. */
  typedef HistogramToEntropyImageFilter Self;

  /** Standard "Superclass" typedef. */
  typedef HistogramToImageFilter< THistogram, 
    Function::HistogramEntropyFunction< unsigned long, TOutputPixel > >
                                        Superclass;

  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;
  
  /** Run-time type information (and related methods).   */
  itkTypeMacro( HistogramToEntropyImageFilter, HistogramToImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  HistogramToEntropyImageFilter() {}
  virtual ~HistogramToEntropyImageFilter() {}
  
private:
  HistogramToEntropyImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
