/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToListAdaptor.txx,v $
  Language:  C++
  Date:      $Date: 2008-08-19 22:27:57 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkScalarImageToListAdaptor_txx
#define _itkScalarImageToListAdaptor_txx

namespace itk{ 
namespace Statistics{

template < class TImage >
void
ScalarImageToListAdaptor< TImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template < class TImage >
const typename ScalarImageToListAdaptor< TImage >::MeasurementVectorType &
ScalarImageToListAdaptor< TImage >
::GetMeasurementVector(const InstanceIdentifier &id) const
{
  if( this->GetUseBuffer() )
    {
    m_TempVector[0] = (*this->GetPixelContainer())[id];
    }
  else
    {
    m_TempVector[0] = this->GetImage()->GetPixel( this->GetImage()->ComputeIndex( id ) );
    }
  return m_TempVector;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif



