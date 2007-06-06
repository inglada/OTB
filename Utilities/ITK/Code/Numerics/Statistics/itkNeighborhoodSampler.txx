/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodSampler.txx,v $
  Language:  C++
  Date:      $Date: 2005/07/26 15:55:04 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNeighborhoodSampler_txx
#define __itkNeighborhoodSampler_txx

#include "itkNeighborhoodSampler.h"
#include "vnl/vnl_math.h"

namespace itk{ 
namespace Statistics{

template< class TSample >
NeighborhoodSampler< TSample >
::NeighborhoodSampler()
{
  m_Center = 0 ;
  m_Radius = 0 ;
  m_Subsample = SubsampleType::New() ;
}

template< class TSample >
void
NeighborhoodSampler< TSample >
::PrintSelf(std::ostream& os, Indent indent) const 
{
  Superclass::PrintSelf(os,indent) ;

  os << indent << "Center: " ;
  if ( m_Center != 0 )
    {
     os << (*m_Center) << std::endl;
    }
  else
    {
    os << "not set. " << std::endl ;
    }

  os << indent << "Radius: " ;
  if ( m_Radius != 0 )
    {
    os << (*m_Radius) << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "Output      " << m_Subsample << std::endl;
  os << indent << "Output Size " << m_Subsample->Size() << std::endl ;
}

template< class TSample >
typename NeighborhoodSampler< TSample >::OutputPointer
NeighborhoodSampler< TSample >
::GetOutput()
{
  return m_Subsample ;
}

template< class TSample >
void
NeighborhoodSampler< TSample >
::GenerateData()
{
  if (m_Radius == 0 || m_Center == 0 || this->GetInputSample() == 0)
    {
    itkExceptionMacro("Member variables have not been properly set.") ;
    }
  
  // Assert at run time that the given center has the same length as 
  // measurement vectors in the sample and that the size is non-zero.
    if( this->GetMeasurementVectorSize() && 
      ( m_Center->Size() != this->GetMeasurementVectorSize() ) )
      {
      itkExceptionMacro( << "Size of measurement vectors in the sample is: " <<
         this->GetMeasurementVectorSize() << " but size of center is: " <<
         m_Center->Size() );
      }
  
  m_Subsample->SetSample(this->GetInputSample()) ;

  unsigned int j ;
  double squaredRadius ;
  double distance ;
  double coordinateDistance ;
  MeasurementVectorType tempVector ;

  squaredRadius = (*m_Radius) * (*m_Radius) ;

  m_Subsample->Clear() ;
  typename TSample::ConstIterator iter = this->GetInputSample()->Begin() ;
  typename TSample::ConstIterator last = this->GetInputSample()->End() ;
  while (iter != last)
    {
    distance = 0.0 ;
    tempVector = iter.GetMeasurementVector() ;
    for (j = 0 ; j < this->GetMeasurementVectorSize() && distance < squaredRadius ; j++)
      {
      coordinateDistance = (double)tempVector[j] - (*m_Center)[j] ;
      if (vnl_math_abs(coordinateDistance) > (*m_Radius) )
        {
        distance = squaredRadius ;
        }
      }
      
    for (j = 0 ; j < this->GetMeasurementVectorSize() 
                        && distance < squaredRadius ; j++)
      {
      coordinateDistance = (double)tempVector[j] - (*m_Center)[j] ;
      distance += coordinateDistance * coordinateDistance ;
      }
      
    if (distance < squaredRadius)
      {
      m_Subsample->AddInstance(iter.GetInstanceIdentifier()) ;
      }
    ++iter ;
    }
}

} // end of namespace Statistics 
} // end of namespace itk

#endif
