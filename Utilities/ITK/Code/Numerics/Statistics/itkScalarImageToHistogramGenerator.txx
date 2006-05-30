/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToHistogramGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2003/12/05 18:47:38 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkScalarImageToHistogramGenerator_txx
#define _itkScalarImageToHistogramGenerator_txx

#include "itkScalarImageToHistogramGenerator.h"


namespace itk { 
namespace Statistics {


template < class TImage >
ScalarImageToHistogramGenerator< TImage >
::ScalarImageToHistogramGenerator() 
{
  m_ImageToListAdaptor = AdaptorType::New();
  m_HistogramGenerator = GeneratorType::New();
  m_HistogramGenerator->SetListSample( m_ImageToListAdaptor );
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
  size.Fill( numberOfBins );
  m_HistogramGenerator->SetNumberOfBins( size );
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


