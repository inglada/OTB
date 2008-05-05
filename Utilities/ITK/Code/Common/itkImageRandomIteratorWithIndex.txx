/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageRandomIteratorWithIndex.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:11 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageRandomIteratorWithIndex_txx
#define _itkImageRandomIteratorWithIndex_txx

#include "itkImageRandomIteratorWithIndex.h"

namespace itk
{



template< typename TImage >
ImageRandomIteratorWithIndex<TImage>
::ImageRandomIteratorWithIndex()
  : ImageRandomConstIteratorWithIndex<TImage>() 
{


}



template< typename TImage >
ImageRandomIteratorWithIndex<TImage>
::ImageRandomIteratorWithIndex(ImageType *ptr, const RegionType& region) :
  ImageRandomConstIteratorWithIndex<TImage>(   ptr, region ) 
{


}


 
template< typename TImage >
ImageRandomIteratorWithIndex<TImage>
::ImageRandomIteratorWithIndex( const ImageIteratorWithIndex<TImage> &it):
  ImageRandomConstIteratorWithIndex<TImage>(it)
{ 
}

 
template< typename TImage >
ImageRandomIteratorWithIndex<TImage>
::ImageRandomIteratorWithIndex( const ImageRandomConstIteratorWithIndex<TImage> &it):
  ImageRandomConstIteratorWithIndex<TImage>(it)
{ 
}

 
template< typename TImage >
ImageRandomIteratorWithIndex<TImage> &
ImageRandomIteratorWithIndex<TImage>
::operator=( const ImageRandomConstIteratorWithIndex<TImage> &it)
{ 
  this->ImageRandomConstIteratorWithIndex<TImage>::operator=(it);
  return *this;
}



} // end namespace itk

#endif
