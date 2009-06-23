/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageRegionReverseIterator.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-18 21:13:25 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageRegionReverseIterator_txx
#define __itkImageRegionReverseIterator_txx

#include "itkImageRegionReverseIterator.h"

namespace itk
{


template< typename TImage >
ImageRegionReverseIterator<TImage>
::ImageRegionReverseIterator()
  : ImageRegionReverseConstIterator<TImage>() 
{


}


template< typename TImage >
ImageRegionReverseIterator<TImage>
::ImageRegionReverseIterator(ImageType *ptr, const RegionType& region) :
  ImageRegionReverseConstIterator<TImage>(   ptr, region ) 
{


}


 
template< typename TImage >
ImageRegionReverseIterator<TImage>
::ImageRegionReverseIterator( const ImageConstIterator<TImage> &it): Superclass(it)
{ 
}

 
template< typename TImage >
ImageRegionReverseIterator<TImage>
::ImageRegionReverseIterator( const ImageRegionReverseConstIterator<TImage> &it):
  Superclass(it)
{ 
}

 
template< typename TImage >
ImageRegionReverseIterator<TImage> &
ImageRegionReverseIterator<TImage>
::operator=( const ImageRegionReverseConstIterator<TImage> &it)
{ 
  this->Superclass::operator=(it);
  return *this;
}


template< typename TImage >
ImageRegionReverseIterator<TImage> 
ImageRegionReverseIterator<TImage>
::Begin() const
{ 
  return this->Superclass::Begin();
}


template< typename TImage >
ImageRegionReverseIterator<TImage> 
ImageRegionReverseIterator<TImage>
::End() const
{ 
  return this->Superclass::End();
}


} // end namespace itk

#endif
