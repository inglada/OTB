/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStandardDeviationPerComponentSampleFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStandardDeviationPerComponentSampleFilter_txx
#define __itkStandardDeviationPerComponentSampleFilter_txx

#include "itkStandardDeviationPerComponentSampleFilter.h"
#include "itkMeasurementVectorTraits.h"
#include "vnl/vnl_math.h"

namespace itk { 
namespace Statistics {

template< class TSample >
StandardDeviationPerComponentSampleFilter< TSample >
::StandardDeviationPerComponentSampleFilter()  
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);
  this->ProcessObject::SetNumberOfRequiredOutputs(2);

  this->ProcessObject::SetNthOutput( 0, this->MakeOutput(0) );
  this->ProcessObject::SetNthOutput( 1, this->MakeOutput(1) );
}

template< class TSample >
StandardDeviationPerComponentSampleFilter< TSample >
::~StandardDeviationPerComponentSampleFilter()  
{
}

template< class TSample >
void
StandardDeviationPerComponentSampleFilter< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template< class TSample >
void
StandardDeviationPerComponentSampleFilter< TSample >
::SetInput( const SampleType * sample )
{
  this->ProcessObject::SetNthInput(0, const_cast< SampleType* >( sample ) );
}

template< class TSample >
const TSample *
StandardDeviationPerComponentSampleFilter< TSample >
::GetInput( ) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const SampleType * >
  (this->ProcessObject::GetInput(0) );
}

template< class TSample >
typename StandardDeviationPerComponentSampleFilter< TSample>::DataObjectPointer
StandardDeviationPerComponentSampleFilter< TSample >
::MakeOutput(unsigned int index )
{
  if ( index == 0 )
    {
    typedef typename MeasurementVectorTraitsTypes< MeasurementVectorType >::ValueType   ValueType;
    MeasurementVectorType standardDeviation;
    MeasurementVectorTraits::SetLength( standardDeviation,  this->GetMeasurementVectorSize() );
    standardDeviation.Fill( NumericTraits< ValueType >::Zero );
    typename MeasurementVectorRealDecoratedType::Pointer decoratedStandardDeviation = MeasurementVectorRealDecoratedType::New();
    decoratedStandardDeviation->Set( standardDeviation ); 
    return static_cast< DataObject * >( decoratedStandardDeviation.GetPointer() );
    }

  if ( index == 1 )
    {
    typedef typename MeasurementVectorTraitsTypes< MeasurementVectorType >::ValueType   ValueType;
    MeasurementVectorType mean;
    MeasurementVectorTraits::SetLength( mean,  this->GetMeasurementVectorSize() );
    mean.Fill( NumericTraits< ValueType >::Zero );
    typename MeasurementVectorRealDecoratedType::Pointer decoratedStandardDeviation = MeasurementVectorRealDecoratedType::New();
    decoratedStandardDeviation->Set( mean ); 
    return static_cast< DataObject * >( decoratedStandardDeviation.GetPointer() );
    }

  itkExceptionMacro("Trying to create output of index " << index << " larger than the number of output");
}

template< class TSample >
typename StandardDeviationPerComponentSampleFilter< TSample >::MeasurementVectorSizeType 
StandardDeviationPerComponentSampleFilter< TSample >
::GetMeasurementVectorSize() const
{
  const SampleType *input = this->GetInput();

  if( input )
    {
    return input->GetMeasurementVectorSize();
    }

  // Test if the Vector type knows its length
  MeasurementVectorType vector;
  MeasurementVectorSizeType measurementVectorSize = MeasurementVectorTraits::GetLength( vector );

  if( measurementVectorSize )
    {
    return measurementVectorSize;
    }

  measurementVectorSize = 1; // Otherwise set it to an innocuous value

  return measurementVectorSize;
}

template< class TSample >
inline void
StandardDeviationPerComponentSampleFilter< TSample >
::GenerateData() 
{
  const SampleType *input = this->GetInput();

  MeasurementVectorSizeType measurementVectorSize = input->GetMeasurementVectorSize();
 
  MeasurementVectorRealDecoratedType * decoratedStandardDeviationOutput =
            static_cast< MeasurementVectorRealDecoratedType * >(
              this->ProcessObject::GetOutput(0));

  MeasurementVectorRealDecoratedType * decoratedMean =
            static_cast< MeasurementVectorRealDecoratedType * >(
              this->ProcessObject::GetOutput(1));


  MeasurementVectorRealType sum;
  MeasurementVectorRealType sumOfSquares;
  MeasurementVectorRealType mean;
  MeasurementVectorRealType standardDeviation;

  MeasurementVectorTraits::SetLength( sum, measurementVectorSize );
  MeasurementVectorTraits::SetLength( mean, measurementVectorSize );
  MeasurementVectorTraits::SetLength( sumOfSquares, measurementVectorSize );
  MeasurementVectorTraits::SetLength( standardDeviation, measurementVectorSize );

  sum.Fill(0.0);
  mean.Fill(0.0);
  sumOfSquares.Fill(0.0);
  standardDeviation.Fill(0.0);

  typename TSample::AbsoluteFrequencyType frequency;

  typedef typename TSample::TotalAbsoluteFrequencyType TotalAbsoluteFrequencyType;
  TotalAbsoluteFrequencyType totalFrequency = 
    itk::NumericTraits< TotalAbsoluteFrequencyType >::Zero;

  typename TSample::ConstIterator iter = input->Begin();
  typename TSample::ConstIterator end = input->End();

  MeasurementVectorType diff;
  MeasurementVectorType measurements;

  MeasurementVectorTraits::SetLength( diff, measurementVectorSize );
  MeasurementVectorTraits::SetLength( measurements, measurementVectorSize );

  //Compute the mean first
  while (iter != end)
    {
    frequency = iter.GetFrequency();
    totalFrequency += frequency;
    measurements = iter.GetMeasurementVector();

    for( unsigned int i = 0; i < measurementVectorSize; ++i )
      {
      double value = measurements[i]; 
      sum[i] += frequency * value;
      sumOfSquares[i] += frequency * value * value;
      }
    ++iter;
    }

  for( unsigned int i = 0; i < measurementVectorSize; ++i )
    {
    const double meanValue = sum[i] / totalFrequency;
    mean[i] = meanValue;
    const double variance = 
     ( sumOfSquares[i] - meanValue * meanValue * totalFrequency ) / ( totalFrequency - 1.0 );
    standardDeviation[i] = vcl_sqrt( variance );
    }

  decoratedStandardDeviationOutput->Set( standardDeviation );
  decoratedMean->Set( mean );
}

template< class TSample >
const typename StandardDeviationPerComponentSampleFilter< TSample>::MeasurementVectorRealDecoratedType *
StandardDeviationPerComponentSampleFilter< TSample >
::GetStandardDeviationPerComponentOutput() const
{
  return static_cast<const MeasurementVectorRealDecoratedType *>(this->ProcessObject::GetOutput(0));
}

template< class TSample >
const typename StandardDeviationPerComponentSampleFilter< TSample>::MeasurementVectorRealType
StandardDeviationPerComponentSampleFilter< TSample >
::GetStandardDeviationPerComponent() const 
{
  return this->GetStandardDeviationPerComponentOutput()->Get();
}

template< class TSample >
const typename StandardDeviationPerComponentSampleFilter< TSample>::MeasurementVectorRealDecoratedType *
StandardDeviationPerComponentSampleFilter< TSample >
::GetMeanPerComponentOutput() const
{
  return static_cast<const MeasurementVectorRealDecoratedType *>(this->ProcessObject::GetOutput(1));
}

template< class TSample >
const typename StandardDeviationPerComponentSampleFilter< TSample>::MeasurementVectorRealType
StandardDeviationPerComponentSampleFilter< TSample >
::GetMeanPerComponent() const 
{
  return this->GetMeanPerComponentOutput()->Get();
}


} // end of namespace Statistics 
} // end of namespace itk

#endif
