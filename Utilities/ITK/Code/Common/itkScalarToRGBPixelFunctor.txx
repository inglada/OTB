/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarToRGBPixelFunctor.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:07 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarToRGBPixelFunctor_txx
#define __itkScalarToRGBPixelFunctor_txx

#include "itkScalarToRGBPixelFunctor.h"

namespace itk {

namespace Functor {  

template <class TScalar>
ScalarToRGBPixelFunctor<TScalar>
::ScalarToRGBPixelFunctor()
{
  m_Index[0] = m_Index[1] = m_Index[2] = 0;
  const ::size_t l = sizeof(ScalarType);
  for (unsigned int i = 0; i < l && i < 3; ++i)
    {      m_Index[i] = i;    }

#ifdef ITK_WORDS_BIGENDIAN
  m_IsBigEndian = true;
#else
  m_IsBigEndian = false;
#endif
}

  
template <class TScalar>
typename ScalarToRGBPixelFunctor<TScalar>::RGBPixelType
ScalarToRGBPixelFunctor<TScalar>
::operator()( const TScalar & v) const
{
  unsigned int i;
  int j;

  TScalar buf = v;
  unsigned char *bytes = (unsigned char *)(&buf);  

  RGBPixelType ans;
  if (this->m_IsBigEndian == true)
    { // swap bytes
    TScalar tmp;
    for (j = sizeof(TScalar) - 1, i = 0; j >=0; j--, i++)
      {
      ((unsigned char *)(&tmp))[i] = bytes[j];
      }
    buf = tmp;
    }
  
  ans[0] = static_cast<RGBComponentType>( bytes[m_Index[0]] * 3 );
  ans[1] = static_cast<RGBComponentType>( (bytes[m_Index[0]] + bytes[m_Index[1]]) * 5 );
  ans[2] = static_cast<RGBComponentType>( (bytes[m_Index[0]] + bytes[m_Index[2]])  );
                                         
  return ans;
}


  


} // end namespace Functor

} // end namespace itk


#endif
