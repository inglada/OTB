/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-07 14:03:44 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageFunction_txx
#define __itkImageFunction_txx

#include "itkImageFunction.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutput, class TCoordRep>
ImageFunction<TInputImage, TOutput, TCoordRep>
::ImageFunction()
{
  m_Image = NULL;
  m_StartIndex.Fill(0);
  m_EndIndex.Fill(0);
  m_StartContinuousIndex.Fill(0.0);
  m_EndContinuousIndex.Fill(0.0);
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class TCoordRep>
void
ImageFunction<TInputImage, TOutput, TCoordRep>
::PrintSelf(
  std::ostream& os,
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "InputImage: " << m_Image.GetPointer() << std::endl;
  os << indent << "StartIndex: " << m_StartIndex << std::endl;
  os << indent << "EndIndex: " << m_EndIndex << std::endl;
  os << indent << "StartContinuousIndex: " << m_StartContinuousIndex << std::endl;
  os << indent << "EndContinuousIndex: " << m_EndContinuousIndex << std::endl;
}


/**
 * Initialize by setting the input image
 */
template <class TInputImage, class TOutput, class TCoordRep>
void
ImageFunction<TInputImage, TOutput, TCoordRep>
::SetInputImage(
  const InputImageType * ptr )
{
  // set the input image
  m_Image = ptr;

  if ( ptr )
    {
    typedef typename IndexType::IndexValueType IndexValueType;
    typename InputImageType::SizeType size = ptr->GetBufferedRegion().GetSize();
    m_StartIndex = ptr->GetBufferedRegion().GetIndex();

    for ( unsigned int j = 0; j < ImageDimension; j++ )
      {
      m_EndIndex[j] = m_StartIndex[j] + static_cast<IndexValueType>( size[j] ) - 1;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      m_StartContinuousIndex[j] = static_cast<CoordRepType>( m_StartIndex[j] - 0.5 );
      m_EndContinuousIndex[j]   = static_cast<CoordRepType>( m_EndIndex[j] + 0.5 );
#else
      m_StartContinuousIndex[j] = static_cast<CoordRepType>( m_StartIndex[j] );
      m_EndContinuousIndex[j]   = static_cast<CoordRepType>( m_EndIndex[j] );
#endif
      }
    }
}


} // end namespace itk

#endif
