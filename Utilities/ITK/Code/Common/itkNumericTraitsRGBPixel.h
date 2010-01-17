/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsRGBPixel.h,v $
  Language:  C++
  Date:      $Date: 2009-07-30 20:51:30 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumericTraitsRGBPixel_h
#define __itkNumericTraitsRGBPixel_h

#include "itkNumericTraits.h"
#include "itkRGBPixel.h"


namespace itk
{

//
// First we define a macro that can be customized to be used for a sequence of
// specializations or for a generic template instantiation. This Macro covers
// the implementation for good compilers and for Visual Studio 6.0.
//
#define itkNumericTraitsRGBPixelMacro(T) \
template < _TEMPLATE_ARGUMENT_ >  \
class NumericTraits<RGBPixel< T > >  \
{ \
public: \
  typedef T ValueType; \
 \
  typedef _TYPENAME_ NumericTraits<T>::AbsType        ElementAbsType; \
  typedef _TYPENAME_ NumericTraits<T>::AccumulateType ElementAccumulateType; \
  typedef _TYPENAME_ NumericTraits<T>::FloatType      ElementFloatType; \
  typedef _TYPENAME_ NumericTraits<T>::PrintType      ElementPrintType; \
  typedef _TYPENAME_ NumericTraits<T>::RealType       ElementRealType; \
 \
  typedef RGBPixel<T>                   Self; \
 \
  typedef RGBPixel<ElementAbsType>          AbsType; \
  typedef RGBPixel<ElementAccumulateType>   AccumulateType; \
  typedef RGBPixel<ElementFloatType>        FloatType; \
  typedef Self                              PrintType; \
  typedef RGBPixel<ElementRealType>         RealType; \
 \
  typedef ElementRealType ScalarRealType; \
 \
  static const Self max( const Self & a ) \
    {  \
      Self b;  \
      b.Fill( NumericTraits< T >::max() ); \
      return b; \
    } \
  static const Self min( const Self & a ) \
    {  \
      Self b;  \
      b.Fill( NumericTraits< T >::min() ); \
      return b; \
    } \
  static const Self max() \
    {  \
      Self b;  \
      b.Fill( NumericTraits< T >::max() ); \
      return b; \
    } \
  static const Self min() \
    {  \
      Self b;  \
      b.Fill( NumericTraits< T >::min() ); \
      return b; \
    } \
  static const Self NonpositiveMin() \
    {  \
      return NumericTraits< Self >::min(); \
    } \
  static const Self ZeroValue() \
  {  \
    Self b; \
    b.Fill( NumericTraits< T >::Zero ); \
    return b; \
  } \
  static const Self OneValue() \
  {  \
    Self b; \
    b.Fill( NumericTraits< T >::One ); \
    return b; \
  } \
  static const Self ITKCommon_EXPORT Zero; \
  static const Self ITKCommon_EXPORT One; \
};


//
// Visual Studio 6.0 is not capable of managing the template implementation
// defined at the end of this file. Therefore we provide an alternative
// primitive implementation based on macros that define explicit
// instantiations.
//
#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )

// These two symbols below are defined empty on purpose
#define _TYPENAME_
#define _TEMPLATE_ARGUMENT_

//
// List here the specializations of the Traits:
//
itkNumericTraitsRGBPixelMacro( char );
itkNumericTraitsRGBPixelMacro( unsigned char );
itkNumericTraitsRGBPixelMacro( short );
itkNumericTraitsRGBPixelMacro( unsigned short );
itkNumericTraitsRGBPixelMacro( int );
itkNumericTraitsRGBPixelMacro( unsigned int );
itkNumericTraitsRGBPixelMacro( long );
itkNumericTraitsRGBPixelMacro( unsigned long );
itkNumericTraitsRGBPixelMacro( float );
itkNumericTraitsRGBPixelMacro( double );

#else

// For all the other good compilers, we provide here a generic implementation
// based on creating types of RGBPixels whose components are the types of the
// NumericTraits from the original RGBPixel components. This implementation
// doesn't require specializations, since it is based on the concept that 
//
//    NumericTraits< RGBAPixle< T > >  is defined piecewise by
//    RGBAPixle< NumericTraits< T > >
//
//
// By defining the following symbols, the Macro above gets customized to become
// a generic template implementation of the traits
//
#define _TYPENAME_            typename
#define _TEMPLATE_ARGUMENT_   class T

//
// Then we simply call the macro once with the generic template argument T.
//
itkNumericTraitsRGBPixelMacro( T );

#endif

//
// Finally, to avoid contamination of other files with the symbols defined
// here, we undefine the helper macros
//
#undef _TYPENAME_
#undef _TEMPLATE_ARGUMENT_


} // end namespace itk

#endif // __itkNumericTraitsRGBPixel_h
