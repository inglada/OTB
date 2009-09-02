/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToHistogramGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-05 19:44:33 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarImageToHistogramGenerator_txx
#define __itkScalarImageToHistogramGenerator_txx

#include "itkScalarImageToHistogramGenerator.h"


namespace itk { 
namespace Statistics {


template < class TImage >
ScalarImageToHistogramGenerator< TImage >
::ScalarImageToHistogramGenerator() 
{
  m_ImageToListAdaptor = AdaptorType::New();
  m_HistogramGenerator = GeneratorType::New();
#ifdef ITK_USE_REVIEW_STATISTICS
  m_HistogramGenerator->SetInput( m_ImageToListAdaptor );
#else
  m_HistogramGenerator->SetListSample( m_ImageToListAdaptor );
#endif
}

template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::SetInput( const ImageType * image ) 
{
  m_ImageToListAdaptor->SetImage( image );
}


template < class TImage >
const typename ScalarImageToHistogramGenerator< TImage >::HistogramType *
ScalarImageToHistogramGenerator< TImage >
::GetOutput() const
{
  return m_HistogramGenerator->GetOutput();
}

template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::Compute() 
{
  m_HistogramGenerator->Update();
}

template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::SetNumberOfBins( unsigned int numberOfBins ) 
{
  typename HistogramType::SizeType size;
#ifdef ITK_USE_REVIEW_STATISTICS
  size.SetSize(1);
  size.Fill( numberOfBins );
  m_HistogramGenerator->SetHistogramSize( size );
#else
  size.Fill( numberOfBins );
  m_HistogramGenerator->SetNumberOfBins( size );
#endif
}


template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::SetHistogramMin( RealPixelType minimumValue ) 
{
  typedef typename GeneratorType::MeasurementVectorType     MeasurementVectorType;
  MeasurementVectorType minVector;
  minVector[0] = minimumValue;
  m_HistogramGenerator->SetHistogramMin( minVector );
}


template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::SetHistogramMax( RealPixelType maximumValue ) 
{
  typedef typename GeneratorType::MeasurementVectorType     MeasurementVectorType;
  MeasurementVectorType maxVector;
  maxVector[0] = maximumValue;
  m_HistogramGenerator->SetHistogramMax( maxVector );
}

template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::SetMarginalScale( double marginalScale )
{
  m_HistogramGenerator->SetMarginalScale( marginalScale );
}

template < class TImage >
void
ScalarImageToHistogramGenerator< TImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << "ImageToListSample adaptor = " << m_ImageToListAdaptor << std::endl;
  os << "HistogramGenerator = " << m_HistogramGenerator << std::endl;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif
