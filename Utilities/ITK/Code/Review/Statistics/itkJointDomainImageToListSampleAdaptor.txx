/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkJointDomainImageToListSampleAdaptor.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-08 16:55:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkJointDomainImageToListSampleAdaptor_txx
#define __itkJointDomainImageToListSampleAdaptor_txx

#include "itkJointDomainImageToListSampleAdaptor.h"

namespace itk { 
namespace Statistics {

template < class TImage >
JointDomainImageToListSampleAdaptor< TImage >
::JointDomainImageToListSampleAdaptor()
{
  m_NormalizationFactors.Fill( 1.0f );
  m_Image = 0;
  m_UsePixelContainer = true;
}

/** returns the number of measurement vectors in this container*/
template < class TImage>
typename JointDomainImageToListSampleAdaptor< TImage>::InstanceIdentifier
JointDomainImageToListSampleAdaptor< TImage>
::Size() const
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return m_Image->GetPixelContainer()->Size();
}

template < class TImage>
inline typename JointDomainImageToListSampleAdaptor< TImage>::AbsoluteFrequencyType
JointDomainImageToListSampleAdaptor< TImage>
::GetFrequency( InstanceIdentifier ) const 
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return NumericTraits< AbsoluteFrequencyType >::One;
}


template < class TImage>
void
JointDomainImageToListSampleAdaptor< TImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Image: ";
  if ( m_Image.IsNotNull() )
    {
    os << m_Image << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }
  os << indent << "UsePixelContainer: "
     << this->GetUsePixelContainer() << std::endl;
}

template < class TImage>
void
JointDomainImageToListSampleAdaptor< TImage>
::SetImage(const TImage* image) 
{ 
  m_Image = image; 
  m_PixelContainer = image->GetPixelContainer();
  this->Modified();
}

template < class TImage>
const TImage*
JointDomainImageToListSampleAdaptor< TImage>
::GetImage() const
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return m_Image.GetPointer(); 
}  

template < class TImage>
typename JointDomainImageToListSampleAdaptor< TImage>::TotalAbsoluteFrequencyType
JointDomainImageToListSampleAdaptor< TImage>
::GetTotalFrequency() const
{ 
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return this->Size(); 
}


template < class TImage >
void
JointDomainImageToListSampleAdaptor< TImage >
::SetNormalizationFactors(NormalizationFactorsType& factors)
{
  if ( m_NormalizationFactors != factors )
    {
    m_NormalizationFactors = factors;
    this->Modified();
    }
}

template < class TImage >
const typename JointDomainImageToListSampleAdaptor< TImage >::MeasurementVectorType &
JointDomainImageToListSampleAdaptor< TImage >
::GetMeasurementVector(InstanceIdentifier id) const
{
  m_TempIndex = this->GetImage()->ComputeIndex( id );
  
  this->GetImage()->TransformIndexToPhysicalPoint( m_TempIndex, m_TempPoint );
  
  for ( unsigned int i = 0; i < TImage::ImageDimension; ++i )
    {
    m_TempVector[i] = m_TempPoint[i] / m_NormalizationFactors[i];
    }

  if( m_UsePixelContainer )
    {
    MeasurementVectorTraits::Assign( m_TempRangeVector,
                    (*m_PixelContainer)[id]); 
    }
  else
    {
    MeasurementVectorTraits::Assign( m_TempRangeVector,
        m_Image->GetPixel( m_Image->ComputeIndex( id ) ) ); 
    }
  
  for ( unsigned int i = TImage::ImageDimension; i < MeasurementVectorType::Length; ++i )
    {
    m_TempVector[i] = m_TempRangeVector[i - TImage::ImageDimension] 
      / m_NormalizationFactors[i];
    }
  
  return m_TempVector;
}


} // end of namespace Statistics 
} // end of namespace itk

#endif
