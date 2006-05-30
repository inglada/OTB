/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsRGBPixel.cxx,v $
  Language:  C++
  Date:      $Date: 2005/01/09 15:58:08 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNumericTraitsRGBPixel.h"

namespace itk
{

const RGBPixel<unsigned char>  NumericTraits<RGBPixel<unsigned char> >::Zero = RGBPixel<unsigned char>( NumericTraits<unsigned char>::Zero );
const RGBPixel<unsigned char>  NumericTraits<RGBPixel<unsigned char> >::One = RGBPixel<unsigned char>( NumericTraits<unsigned char>::One );
 
} // end namespace itk
