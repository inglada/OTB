/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLineIterator.txx,v $
  Language:  C++
  Date:      $Date: 2005/06/06 14:53:24 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkLineIterator_txx
#define _itkLineIterator_txx

#include "itkLineIterator.h"

namespace itk
{

template<class TImage>
LineIterator<TImage>
::LineIterator(ImageType *imagePtr, const IndexType &firstIndex, const IndexType &lastIndex):
  LineConstIterator<TImage>(imagePtr, firstIndex, lastIndex)
{
}


template<class TImage>
LineIterator<TImage> &
LineIterator<TImage>
::operator=(const Self & it)
{
  this->LineConstIterator<TImage>::operator=( it );
  return *this;
}

} // end namespace itk

#endif
