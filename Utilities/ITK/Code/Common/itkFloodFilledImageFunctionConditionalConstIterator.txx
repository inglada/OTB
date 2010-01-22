/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFloodFilledImageFunctionConditionalConstIterator.txx,v $
  Language:  C++
  Date:      $Date: 2006-05-20 14:15:24 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFloodFilledImageFunctionConditionalConstIterator_txx
#define __itkFloodFilledImageFunctionConditionalConstIterator_txx

#include "itkFloodFilledImageFunctionConditionalConstIterator.h"

namespace itk
{
template<class TImage, class TFunction>
bool
FloodFilledImageFunctionConditionalConstIterator<TImage, TFunction>
::IsPixelIncluded(const IndexType & index) const
{
  return this->m_Function->EvaluateAtIndex(index);
}

} // end namespace itk

#endif
