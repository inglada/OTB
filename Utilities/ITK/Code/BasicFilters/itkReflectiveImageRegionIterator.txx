/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReflectiveImageRegionIterator.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-17 16:30:51 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkReflectiveImageRegionIterator_txx
#define __itkReflectiveImageRegionIterator_txx

#include "itkReflectiveImageRegionIterator.h"

namespace itk
{

template< typename TImage >
ReflectiveImageRegionIterator<TImage>
::ReflectiveImageRegionIterator()
  : ReflectiveImageRegionConstIterator<TImage>() 
{
}

template< typename TImage >
ReflectiveImageRegionIterator<TImage>
::ReflectiveImageRegionIterator(ImageType *ptr, const RegionType& region) :
  ReflectiveImageRegionConstIterator<TImage>(   ptr, region ) 
{
}
 
template< typename TImage >
ReflectiveImageRegionIterator<TImage>
::ReflectiveImageRegionIterator( const ImageIteratorWithIndex<TImage> &it):
  ReflectiveImageRegionConstIterator<TImage>(it)
{ 
}

template< typename TImage >
ReflectiveImageRegionIterator<TImage>
::ReflectiveImageRegionIterator( const ReflectiveImageRegionConstIterator<TImage> &it):
  ReflectiveImageRegionConstIterator<TImage>(it)
{ 
}

template< typename TImage >
ReflectiveImageRegionIterator<TImage> &
ReflectiveImageRegionIterator<TImage>
::operator=( const ReflectiveImageRegionConstIterator<TImage> &it)
{ 
  this->ReflectiveImageRegionConstIterator<TImage>::operator=(it);
  return *this;
}

} // end namespace itk

#endif
