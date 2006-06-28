/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConditionalConstIterator.txx,v $
  Language:  C++
  Date:      $Date: 2006/03/18 20:14:38 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConditionalConstIterator_txx
#define __itkConditionalConstIterator_txx

#include "itkConditionalConstIterator.h"

namespace itk
{

template <typename TImageType>
ConditionalConstIterator<TImageType>
::ConditionalConstIterator()
{
}


template <typename TImageType>
ConditionalConstIterator<TImageType>
::~ConditionalConstIterator()
{
}

} // end namespace itk

#endif
