/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsTensorPixel.h,v $
  Language:  C++
  Date:      $Date: 2008-09-25 12:46:41 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumericTraitsTensorPixel_h
#define __itkNumericTraitsTensorPixel_h

#include "itkNumericTraits.h"
#include "itkSymmetricSecondRankTensor.h"
#include "itkDiffusionTensor3D.h"

// This file is meant to define numeric traits for tensor pixels types in itk

namespace itk
{
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 1 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 1> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 1> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 1> AbsType;
  typedef SymmetricSecondRankTensor<short, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 1> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 1> AbsType;
  typedef SymmetricSecondRankTensor<short, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 1> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 1> AbsType;
  typedef SymmetricSecondRankTensor<int, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 1> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 1> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 1> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 1> AbsType;
  typedef SymmetricSecondRankTensor<long, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 1> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 1> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 1> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 1> AbsType;
  typedef SymmetricSecondRankTensor<long, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 1> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 1> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 1> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 1> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 1> PrintType;
  typedef SymmetricSecondRankTensor<float, 1> AbsType;
  typedef SymmetricSecondRankTensor<double, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 1> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 1> PrintType;
  typedef SymmetricSecondRankTensor<double, 1> AbsType;
  typedef SymmetricSecondRankTensor<long double, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 1> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 1> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 1> PrintType;
  typedef SymmetricSecondRankTensor<long double, 1> AbsType;
  typedef SymmetricSecondRankTensor<long double, 1> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 1> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 2 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 2> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 2> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 2> AbsType;
  typedef SymmetricSecondRankTensor<short, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 2> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 2> AbsType;
  typedef SymmetricSecondRankTensor<short, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 2> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 2> AbsType;
  typedef SymmetricSecondRankTensor<int, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 2> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 2> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 2> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 2> AbsType;
  typedef SymmetricSecondRankTensor<long, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 2> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 2> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 2> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 2> AbsType;
  typedef SymmetricSecondRankTensor<long, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 2> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 2> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 2> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 2> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 2> PrintType;
  typedef SymmetricSecondRankTensor<float, 2> AbsType;
  typedef SymmetricSecondRankTensor<double, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 2> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 2> PrintType;
  typedef SymmetricSecondRankTensor<double, 2> AbsType;
  typedef SymmetricSecondRankTensor<long double, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 2> RealType;
  typedef double ScalarRealType;
  static const SymmetricSecondRankTensor< double,2 > ITKCommon_EXPORT Zero;
  static SymmetricSecondRankTensor<double,2> ZeroValue() { return Zero; }
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 2> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 2> PrintType;
  typedef SymmetricSecondRankTensor<long double, 2> AbsType;
  typedef SymmetricSecondRankTensor<long double, 2> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 2> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 3 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 3> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 3> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 3> AbsType;
  typedef SymmetricSecondRankTensor<short, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 3> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 3> AbsType;
  typedef SymmetricSecondRankTensor<short, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 3> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 3> AbsType;
  typedef SymmetricSecondRankTensor<int, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 3> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 3> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 3> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 3> AbsType;
  typedef SymmetricSecondRankTensor<long, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 3> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 3> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 3> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 3> AbsType;
  typedef SymmetricSecondRankTensor<long, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 3> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 3> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 3> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 3> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 3> PrintType;
  typedef SymmetricSecondRankTensor<float, 3> AbsType;
  typedef SymmetricSecondRankTensor<double, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 3> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 3> PrintType;
  typedef SymmetricSecondRankTensor<double, 3> AbsType;
  typedef SymmetricSecondRankTensor<long double, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 3> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 3> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 3> PrintType;
  typedef SymmetricSecondRankTensor<long double, 3> AbsType;
  typedef SymmetricSecondRankTensor<long double, 3> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 3> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 4 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 4> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 4> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 4> AbsType;
  typedef SymmetricSecondRankTensor<short, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 4> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 4> AbsType;
  typedef SymmetricSecondRankTensor<short, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 4> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 4> AbsType;
  typedef SymmetricSecondRankTensor<int, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 4> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 4> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 4> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 4> AbsType;
  typedef SymmetricSecondRankTensor<long, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 4> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 4> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 4> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 4> AbsType;
  typedef SymmetricSecondRankTensor<long, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 4> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 4> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 4> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 4> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 4> PrintType;
  typedef SymmetricSecondRankTensor<float, 4> AbsType;
  typedef SymmetricSecondRankTensor<double, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 4> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 4> PrintType;
  typedef SymmetricSecondRankTensor<double, 4> AbsType;
  typedef SymmetricSecondRankTensor<long double, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 4> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 4> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 4> PrintType;
  typedef SymmetricSecondRankTensor<long double, 4> AbsType;
  typedef SymmetricSecondRankTensor<long double, 4> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 4> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 5 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 5> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 5> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 5> AbsType;
  typedef SymmetricSecondRankTensor<short, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 5> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 5> AbsType;
  typedef SymmetricSecondRankTensor<short, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 5> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 5> AbsType;
  typedef SymmetricSecondRankTensor<int, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 5> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 5> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 5> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 5> AbsType;
  typedef SymmetricSecondRankTensor<long, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 5> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 5> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 5> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 5> AbsType;
  typedef SymmetricSecondRankTensor<long, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 5> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 5> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 5> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 5> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 5> PrintType;
  typedef SymmetricSecondRankTensor<float, 5> AbsType;
  typedef SymmetricSecondRankTensor<double, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 5> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 5> PrintType;
  typedef SymmetricSecondRankTensor<double, 5> AbsType;
  typedef SymmetricSecondRankTensor<long double, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 5> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 5> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 5> PrintType;
  typedef SymmetricSecondRankTensor<long double, 5> AbsType;
  typedef SymmetricSecondRankTensor<long double, 5> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 5> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 6 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 6> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 6> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 6> AbsType;
  typedef SymmetricSecondRankTensor<short, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 6> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 6> AbsType;
  typedef SymmetricSecondRankTensor<short, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 6> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 6> AbsType;
  typedef SymmetricSecondRankTensor<int, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 6> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 6> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 6> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 6> AbsType;
  typedef SymmetricSecondRankTensor<long, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 6> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 6> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 6> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 6> AbsType;
  typedef SymmetricSecondRankTensor<long, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 6> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 6> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 6> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 6> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 6> PrintType;
  typedef SymmetricSecondRankTensor<float, 6> AbsType;
  typedef SymmetricSecondRankTensor<double, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 6> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 6> PrintType;
  typedef SymmetricSecondRankTensor<double, 6> AbsType;
  typedef SymmetricSecondRankTensor<long double, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 6> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 6> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 6> PrintType;
  typedef SymmetricSecondRankTensor<long double, 6> AbsType;
  typedef SymmetricSecondRankTensor<long double, 6> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 6> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 7 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 7> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 7> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 7> AbsType;
  typedef SymmetricSecondRankTensor<short, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 7> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 7> AbsType;
  typedef SymmetricSecondRankTensor<short, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 7> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 7> AbsType;
  typedef SymmetricSecondRankTensor<int, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 7> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 7> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 7> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 7> AbsType;
  typedef SymmetricSecondRankTensor<long, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 7> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 7> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 7> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 7> AbsType;
  typedef SymmetricSecondRankTensor<long, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 7> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 7> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 7> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 7> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 7> PrintType;
  typedef SymmetricSecondRankTensor<float, 7> AbsType;
  typedef SymmetricSecondRankTensor<double, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 7> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 7> PrintType;
  typedef SymmetricSecondRankTensor<double, 7> AbsType;
  typedef SymmetricSecondRankTensor<long double, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 7> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 7> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 7> PrintType;
  typedef SymmetricSecondRankTensor<long double, 7> AbsType;
  typedef SymmetricSecondRankTensor<long double, 7> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 7> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 8 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 8> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 8> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 8> AbsType;
  typedef SymmetricSecondRankTensor<short, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 8> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 8> AbsType;
  typedef SymmetricSecondRankTensor<short, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 8> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 8> AbsType;
  typedef SymmetricSecondRankTensor<int, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 8> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 8> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 8> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 8> AbsType;
  typedef SymmetricSecondRankTensor<long, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 8> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 8> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 8> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 8> AbsType;
  typedef SymmetricSecondRankTensor<long, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 8> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 8> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 8> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 8> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 8> PrintType;
  typedef SymmetricSecondRankTensor<float, 8> AbsType;
  typedef SymmetricSecondRankTensor<double, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 8> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 8> PrintType;
  typedef SymmetricSecondRankTensor<double, 8> AbsType;
  typedef SymmetricSecondRankTensor<long double, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 8> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 8> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 8> PrintType;
  typedef SymmetricSecondRankTensor<long double, 8> AbsType;
  typedef SymmetricSecondRankTensor<long double, 8> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 8> RealType;
  typedef long double ScalarRealType;
};

template <> class NumericTraits<SymmetricSecondRankTensor<unsigned char, 9 > > {
public:
  typedef unsigned char ValueType;
  typedef SymmetricSecondRankTensor<unsigned char, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 9> AbsType;
  typedef SymmetricSecondRankTensor<unsigned short, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<signed char, 9> > {
public:
  typedef signed char ValueType;
  typedef SymmetricSecondRankTensor<signed char, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 9> AbsType;
  typedef SymmetricSecondRankTensor<short, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<char, 9> > {
public:
  typedef char ValueType;
  typedef SymmetricSecondRankTensor<char, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned char, 9> AbsType;
  typedef SymmetricSecondRankTensor<short, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<short, 9> > {
public:
  typedef short ValueType;
  typedef SymmetricSecondRankTensor<short, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 9> AbsType;
  typedef SymmetricSecondRankTensor<int, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned short, 9> > {
public:
  typedef unsigned short ValueType;
  typedef SymmetricSecondRankTensor<unsigned short, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned short, 9> AbsType;
  typedef SymmetricSecondRankTensor<unsigned int, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<int, 9> > {
public:
  typedef int ValueType;
  typedef SymmetricSecondRankTensor<int, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 9> AbsType;
  typedef SymmetricSecondRankTensor<long, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned int, 9> > {
public:
  typedef unsigned int ValueType;
  typedef SymmetricSecondRankTensor<unsigned int, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned int, 9> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long, 9> > {
public:
  typedef long ValueType;
  typedef SymmetricSecondRankTensor<long, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 9> AbsType;
  typedef SymmetricSecondRankTensor<long, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<unsigned long, 9> > {
public:
  typedef unsigned long ValueType;
  typedef SymmetricSecondRankTensor<unsigned long, 9> PrintType;
  typedef SymmetricSecondRankTensor<unsigned long, 9> AbsType;
  typedef SymmetricSecondRankTensor<unsigned long, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<float, 9> > {
public:
  typedef float ValueType;
  typedef SymmetricSecondRankTensor<float, 9> PrintType;
  typedef SymmetricSecondRankTensor<float, 9> AbsType;
  typedef SymmetricSecondRankTensor<double, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<double, 9> > {
public:
  typedef double ValueType;
  typedef SymmetricSecondRankTensor<double, 9> PrintType;
  typedef SymmetricSecondRankTensor<double, 9> AbsType;
  typedef SymmetricSecondRankTensor<long double, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<double, 9> RealType;
  typedef double ScalarRealType;
};
template <> class NumericTraits<SymmetricSecondRankTensor<long double, 9> > {
public:
  typedef long double ValueType;
  typedef SymmetricSecondRankTensor<long double, 9> PrintType;
  typedef SymmetricSecondRankTensor<long double, 9> AbsType;
  typedef SymmetricSecondRankTensor<long double, 9> AccumulateType;
  typedef SymmetricSecondRankTensor<long double, 9> RealType;
  typedef long double ScalarRealType;
};
} // end namespace itk

#endif // __itkNumericTraitsTensorPixel_h  

