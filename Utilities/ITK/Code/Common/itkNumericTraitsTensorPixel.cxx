/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsTensorPixel.cxx,v $
  Language:  C++
  Date:      $Date: 2005/06/10 16:54:48 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNumericTraitsTensorPixel.h"

namespace itk
{
const SymmetricSecondRankTensor<double , 2>  
  NumericTraits<SymmetricSecondRankTensor<double, 2> >::Zero = 
  SymmetricSecondRankTensor<double , 2>( NumericTraits<double >::Zero ); 

} // end namespace itk

