/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageLinearIteratorWithIndex.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:11 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageLinearIteratorWithIndex_txx
#define _itkImageLinearIteratorWithIndex_txx

#include "itkImageLinearIteratorWithIndex.h"

namespace itk
{



template< typename TImage >
ImageLinearIteratorWithIndex<TImage>
::ImageLinearIteratorWithIndex()
  : ImageLinearConstIteratorWithIndex<TImage>() 
{


}



template< typename TImage >
ImageLinearIteratorWithIndex<TImage>
::ImageLinearIteratorWithIndex(ImageType *ptr, const RegionType& region) :
  ImageLinearConstIteratorWithIndex<TImage>(   ptr, region ) 
{


}


 
template< typename TImage >
ImageLinearIteratorWithIndex<TImage>
::ImageLinearIteratorWithIndex( const ImageIteratorWithIndex<TImage> &it):
  ImageLinearConstIteratorWithIndex<TImage>(it)
{ 
}

 
template< typename TImage >
ImageLinearIteratorWithIndex<TImage>
::ImageLinearIteratorWithIndex( const ImageLinearConstIteratorWithIndex<TImage> &it):
  ImageLinearConstIteratorWithIndex<TImage>(it)
{ 
}

 
template< typename TImage >
ImageLinearIteratorWithIndex<TImage> &
ImageLinearIteratorWithIndex<TImage>
::operator=( const ImageLinearConstIteratorWithIndex<TImage> &it)
{ 
  this->ImageLinearConstIteratorWithIndex<TImage>::operator=(it);
  return *this;
}



} // end namespace itk

#endif
