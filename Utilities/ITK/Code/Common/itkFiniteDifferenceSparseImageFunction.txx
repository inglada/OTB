/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFiniteDifferenceSparseImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 23:25:41 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFiniteDifferenceSparseImageFunction_txx
#define __itkFiniteDifferenceSparseImageFunction_txx

#include "itkFiniteDifferenceSparseImageFunction.h"

namespace itk {

template <class TSparseImageType>
void
FiniteDifferenceSparseImageFunction <TSparseImageType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif
