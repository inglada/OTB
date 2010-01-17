/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageIterator.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-18 20:31:36 $
  Version:   $Revision: 1.22 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageIterator_txx
#define __itkImageIterator_txx

#include "itkImageIterator.h"

namespace itk
{


//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage>
::ImageIterator()
{
}


//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage>
::ImageIterator(const Self& it):
  ImageConstIterator<TImage>(it)
{
}


//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage>
::ImageIterator(TImage *ptr, const RegionType & region ):
  ImageConstIterator<TImage>( ptr, region )
{
}
 

//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage>
::ImageIterator(const ImageConstIterator<TImage> & it):
  ImageConstIterator<TImage>( it )
{
}


   
//----------------------------------------------------------------------
//    Assignment Operator
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage> &
ImageIterator<TImage>
::operator=(const ImageConstIterator<TImage> & it)
{
  this->ImageConstIterator<TImage>::operator=( it );
  return *this;
} 
  

 
//----------------------------------------------------------------------
//    Assignment Operator
//----------------------------------------------------------------------
template<class TImage>
ImageIterator<TImage> &
ImageIterator<TImage>
::operator=(const Self& it)
{
  this->ImageConstIterator<TImage>::operator=( it );
  return *this;
} 
  


//----------------------------------------------------------------------------
// Begin() is the first pixel in the region.
template<class TImage>
ImageIterator<TImage>
ImageIterator<TImage>
::Begin() const
{
  return this->Superclass::Begin();
}

//----------------------------------------------------------------------------
// End() is one pixel past the last pixel in the current region.
// The index of this pixel is
//          [m_StartIndex[0] + m_Size[0],
//           m_StartIndex[1] + m_Size[1]-1, ...,
//           m_StartIndex[VImageDimension-2] + m_Size[VImageDimension-2]-1,
//           m_StartIndex[VImageDimension-1] + m_Size[VImageDimension-1]-1]
//
template<class TImage>
ImageIterator<TImage>
ImageIterator<TImage>
::End() const
{
  return this->Superclass::End();
}


} // end namespace itk

#endif
