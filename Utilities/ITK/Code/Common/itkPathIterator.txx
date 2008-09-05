/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPathIterator.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:20 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkPathIterator_txx
#define _itkPathIterator_txx

#include "itkPathIterator.h"
#include "itkOffset.h"        // for operator++

namespace itk
{

template<class TImage, class TPath>
PathIterator<TImage, TPath>
::PathIterator(ImageType *imagePtr, const PathType  *pathPtr):
  PathConstIterator<TImage,TPath>(imagePtr, pathPtr)
{
}


template<class TImage, class TPath>
PathIterator<TImage, TPath> &
PathIterator<TImage, TPath>
::operator=(const Self & it)
{
  this->PathConstIterator<TImage,TPath>::operator=( it );
  return *this;
}

} // end namespace itk

#endif
