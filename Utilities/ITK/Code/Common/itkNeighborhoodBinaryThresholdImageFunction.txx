/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodBinaryThresholdImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-06 22:01:57 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNeighborhoodBinaryThresholdImageFunction_txx
#define __itkNeighborhoodBinaryThresholdImageFunction_txx

#include "itkNeighborhoodBinaryThresholdImageFunction.h"
#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
NeighborhoodBinaryThresholdImageFunction<TInputImage,TCoordRep>
::NeighborhoodBinaryThresholdImageFunction()
{
  m_Radius.Fill(1);
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
NeighborhoodBinaryThresholdImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Radius: " << m_Radius << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
bool
NeighborhoodBinaryThresholdImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  
  if( !this->GetInputImage() )
    {
    return ( false );
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
    return ( false );
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  ConstNeighborhoodIterator<InputImageType>
    it(m_Radius, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  // Walk the neighborhood
  bool allInside = true;
  PixelType lower = this->GetLower();
  PixelType upper = this->GetUpper();
  PixelType value;
  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
    {
    value = it.GetPixel(i);
    if (lower > value || value > upper)
      {
      allInside = false;
      break;
      }
    }

  return ( allInside );
}


} // end namespace itk

#endif
