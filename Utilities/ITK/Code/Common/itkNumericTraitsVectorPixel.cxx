/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsVectorPixel.cxx,v $
  Language:  C++
  Date:      $Date: 2009-12-01 16:53:08 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNumericTraitsVectorPixel.h"

namespace itk
{

itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, char );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, unsigned char );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, signed char );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, short );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, unsigned short );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, int );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, unsigned int );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, long );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, unsigned long );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, float );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, double );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, long double );
#ifdef ITK_TYPE_USE_LONG_LONG
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, long long );
itkStaticNumericTraitsGenericArrayDimensionsMacro( Vector, unsigned long long );
#endif // ITK_TYPE_USE_LONG_LONG

} // end namespace itk
