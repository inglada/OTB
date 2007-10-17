/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSampleToHistogramProjectionFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/07/26 15:55:05 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSampleToHistogramProjectionFilter_h
#define __itkSampleToHistogramProjectionFilter_h

#include "itkMacro.h"
#include "itkObject.h"
#include "itkSample.h"
#include "itkSubsample.h"
#include "itkHistogram.h"
#include "itkFunctionBase.h"
#include "itkSampleAlgorithmBase.h"
#include "itkArray.h"

namespace itk{ 
namespace Statistics{

/** \class SampleToHistogramProjectionFilter 
 *  \brief projects measurement vectors on to an axis to generate an
 * 1D histogram.
 *
 * Users should set the input sample, the output histogram, the mean of
 * input sample, the standard deviation of the input sample, and the projection 
 * axis respectively using the SetInputSample, the SetHistogram, the SetMean, 
 * SetStandardDeviation, and SetProjectionAxis method before run this algorithm.
 *
 * If the bin overlap value is set by the SetHistogramBinOverlap method and 
 * greater than 0.001, the frequency will be weighted based on its closeness
 * of the projected values.
 * 
 * <b>Recent API changes:</b>
 * The static const macro to get the length of a measurement vector,
 * \c MeasurementVectorSize has been removed to allow the length of a 
 * measurement vector to be specified at run time. This is now obtained from
 * the input sample. Typedefs for the Histogram mean \c MeanType etc have been
 * changed to Array.
 */

template < class TInputSample, class THistogramMeasurement >
class ITK_EXPORT SampleToHistogramProjectionFilter : 
    public SampleAlgorithmBase< TInputSample >
{
public:
  /** Standard class typedefs */
  typedef SampleToHistogramProjectionFilter  Self;
  typedef SampleAlgorithmBase< TInputSample > Superclass ;
  typedef SmartPointer< Self > Pointer ;
  typedef const SmartPointer< Self > ConstPointer ;

  /** Run-time type information (and related methods) */
  itkTypeMacro(SampleToHistogramProjectionFilter, SampleAlgorithmBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self) ;


  /** Enums and typedefs from the TInputSample */
  typedef typename TInputSample::MeasurementVectorType MeasurementVectorType ;
  typedef typename TInputSample::MeasurementType MeasurementType ;
  typedef typename TInputSample::FrequencyType FrequencyType ;
  typedef typename TInputSample::InstanceIdentifier InstanceIdentifier ;

  /** typedefs from the superclass */
  typedef typename Superclass::InputSampleType InputSampleType ;
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** 1D array typedef */ 
  typedef   Array< double > ArrayType;

  /** The center of the histogram */
  typedef Array< double > MeanType;

  /** Type of the output object */
  typedef Histogram< THistogramMeasurement, 1 > HistogramType ;

  /** Sets the output histogram */
  virtual void SetHistogram(HistogramType* histogram) ;

  /** Sets the mean of the sample */
  void SetMean(MeanType* center) ;
  
  /** Gets the mean of the sample */
  MeanType* GetMean() ;

  /** Sets the standard deviation of the sample */
  void SetStandardDeviation(double* value) ;

  /** Gets the standard deviation of the sample */
  double* GetStandardDeviation() ;

  /** Sets the projection axis */
  void SetProjectionAxis(ArrayType* axis) ;

  /** Gets the projection axis */
  ArrayType* GetProjectionAxis() ;

  /** Sets the overlap between adjacent bins.
   * If this value is not set, then the overlap calculation
   * will be skipped */
  void SetHistogramBinOverlap(double overlap) ;

protected:
  SampleToHistogramProjectionFilter() ;
  virtual ~SampleToHistogramProjectionFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const ;

  /** Calculates overlap weight for the bin based on
   * closeness to the adjacent bins */
  float CalculateOverlap(int binIndex,
                         float dotProduct, 
                         float scale,
                         float marginalDistance,
                         bool firstHalf) ;
  /** Runs this algorithm to fill the output histogram */
  void GenerateData() ;

private:
  SampleToHistogramProjectionFilter(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented

  bool m_HistogramUseEquiProbableBins ;
  double m_HistogramBinOverlap ;

  MeanType* m_Mean ;
  ArrayType* m_ProjectionAxis ;
  double* m_StandardDeviation ;
  HistogramType* m_Histogram ;
  FrequencyType m_MinimumFrequency ;
} ; // end of class

} // end of namespace Statistics 
} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSampleToHistogramProjectionFilter.txx"
#endif

#endif
