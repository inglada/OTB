/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRGBPixel.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/08 21:36:17 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkRGBPixel_txx
#define _itkRGBPixel_txx
#include "itkRGBPixel.h"
#include "itkNumericTraits.h"

namespace itk
{

/*
 * Assigment from a plain array
 */
template<class T>
RGBPixel<T>&
RGBPixel<T>
::operator= (const ComponentType r[3])
{
  BaseArray::operator=(r);
  return *this;
}


  
/**
 * Returns a temporary copy of a vector
 */
template<class T>
RGBPixel<T> 
RGBPixel<T>
::operator+(const Self & r) const
{
  Self result;
  for( unsigned int i=0; i<3; i++) 
    {
    result[i] = (*this)[i] + r[i];
    }
  return result;
}




/**
 * Returns a temporary copy of a vector
 */
template<class T>
RGBPixel<T> 
RGBPixel<T>
::operator-(const Self & r) const
{
  Self result;
  for( unsigned int i=0; i<3; i++) 
    {
    result[i] = (*this)[i] - r[i];
    }
  return result;
}


 
/**
 * Returns a temporary copy of a vector
 */
template<class T>
const RGBPixel<T> & 
RGBPixel<T>
::operator+=(const Self & r) 
{
  for( unsigned int i=0; i<3; i++) 
    {
    (*this)[i] += r[i];
    }
  return *this;
}



 
/**
 * Returns a temporary copy of a vector
 */
template<class T>
const RGBPixel<T> & 
RGBPixel<T>
::operator-=(const Self & r)
{
  for( unsigned int i=0; i<3; i++) 
    {
    (*this)[i] -= r[i];
    }
  return *this;
}





/**
 * Returns a temporary copy of a vector
 */
template<class T>
RGBPixel<T> 
RGBPixel<T>
::operator*(const ComponentType & r) const
{
  Self result;
  for( unsigned int i=0; i<3; i++) 
    {
    result[i] = (*this)[i] * r;
    }
  return result;
}




/*
 * Compute luminance
 */
template<class T>
typename RGBPixel<T>::ComponentType
RGBPixel<T>
::GetLuminance() const
{
  const double luminance = 
    0.30  * this->GetRed()   +
    0.59  * this->GetGreen() +
    0.11  * this->GetBlue();
  return static_cast<ComponentType>( luminance );
}




/**
 * Print content to an ostream
 */
template<class TComponent>
std::ostream &
operator<<(std::ostream& os,const RGBPixel<TComponent> & c ) 
{
  os <<  static_cast<typename NumericTraits<TComponent>::PrintType>(c[0]) << "  ";
  os <<  static_cast<typename NumericTraits<TComponent>::PrintType>(c[1]) << "  ";
  os <<  static_cast<typename NumericTraits<TComponent>::PrintType>(c[2]) ;
  return os;
}


/**
 * Read content from an istream
 */
template<class TComponent>
std::istream &
operator>>(std::istream& is, RGBPixel<TComponent> & c ) 
{
  TComponent red;
  TComponent green;
  TComponent blue;
  is >> red >> green >> blue;
  c.SetRed( red );
  c.SetGreen( green );
  c.SetBlue( blue );
  return is;
}

} // end namespace itk

#endif
