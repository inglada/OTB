/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFixedArray.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 13:28:49 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFixedArray_txx
#define __itkFixedArray_txx

#include "itkFixedArray.h"
#include "itkNumericTraitsFixedArrayPixel.h"

namespace itk
{

/**
 * Default constructor uses compiler's default initialization of memory.
 * For efficiency, no initialization to zero is done.
 */
template <typename TValueType, unsigned int VLength>
FixedArray<TValueType, VLength>
::FixedArray()
{
}

/**
 * Constructor assumes input points to array of correct size.
 * Values are copied individually instead of with a binary copy.  This
 * allows the ValueType's assignment operator to be executed.
 */
template <typename TValueType, unsigned int VLength>
FixedArray<TValueType, VLength>
::FixedArray(const ValueType r[VLength])
{
  ConstIterator input = r;
  Iterator i = this->Begin();
  while( i != this->End() )
    {
    *i++ = *input++;
    }
}


/**
 * Assignment operator assumes input points to array of correct size.
 * Values are copied individually instead of with a binary copy.  This
 * allows the ValueType's assignment operator to be executed.
 */
template <typename TValueType, unsigned int VLength>
FixedArray<TValueType, VLength>&
FixedArray<TValueType, VLength>
::operator= (const ValueType r[VLength])
{
  if(r == m_InternalArray) return *this;
  ConstIterator input = r;
  Iterator i = this->Begin();
  while( i != this->End() )
    {
    *i++ = *input++;
    }
  return *this;
}


/**
 * Operator != compares different types of arrays.
 */
template <typename TValueType, unsigned int VLength>
bool
FixedArray<TValueType, VLength>
::operator== (const FixedArray& r) const
{
  ConstIterator i = this->Begin();
  ConstIterator j = r.Begin();
  
  while( i != this->End() )
    {
    if ( *i != *j )
      {
      return false;
      }
    ++j;
    ++i;
    }

  return true;
}

/**
 * Get an Iterator for the beginning of the FixedArray.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::Iterator
FixedArray<TValueType, VLength>
::Begin()
{
  return Iterator(m_InternalArray);
}


/**
 * Get a ConstIterator for the beginning of the FixedArray.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ConstIterator
FixedArray<TValueType, VLength>
::Begin() const
{
  return ConstIterator(m_InternalArray);
}


/**
 * Get an Iterator for the end of the FixedArray.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::Iterator
FixedArray<TValueType, VLength>
::End()
{
  return Iterator(m_InternalArray+VLength);
}


/**
 * Get a ConstIterator for the end of the FixedArray.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ConstIterator
FixedArray<TValueType, VLength>
::End() const
{
  return ConstIterator(m_InternalArray+VLength);
}


/**
 * Get a begin ReverseIterator.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ReverseIterator
FixedArray<TValueType, VLength>
::rBegin() 
{
  return ReverseIterator(m_InternalArray+VLength);
}


/**
 * Get a begin ConstReverseIterator.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ConstReverseIterator
FixedArray<TValueType, VLength>
::rBegin() const
{
  return ConstReverseIterator(m_InternalArray+VLength);
}


/**
 * Get an end ReverseIterator.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ReverseIterator
FixedArray<TValueType, VLength>
::rEnd() 
{
  return ReverseIterator(m_InternalArray);
}


/**
 * Get an end ConstReverseIterator.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::ConstReverseIterator
FixedArray<TValueType, VLength>
::rEnd() const 
{
  return ConstReverseIterator(m_InternalArray);
}


/**
 * Get the size of the FixedArray.
 */
template <typename TValueType, unsigned int VLength>
typename FixedArray<TValueType, VLength>::SizeType
FixedArray<TValueType, VLength>
::Size() const
{
  return VLength; 
}


/**
 * Fill all elements of the array with the given value.
 */
template <typename TValueType, unsigned int VLength>
void
FixedArray<TValueType, VLength>
::Fill(const ValueType& value)
{
  Iterator i = this->Begin();
  while( i != this->End() )
    {
    *i++ = value;
    }
}


/**
 * Return an FixedArray with all elements assigned to the given value.
 */
template <typename TValueType, unsigned int VLength>
FixedArray<TValueType, VLength>
FixedArray<TValueType, VLength>
::Filled(const ValueType& value)
{
  FixedArray<ValueType, VLength> array;
  array.Fill(value);
  return array;
}

template <typename TValueType, unsigned int VLength>
std::ostream & operator<<(std::ostream &os, const FixedArray<TValueType,VLength> &arr)
{
  os << "[";
  if ( VLength == 1 )
    {
    os << arr[0];
    }
  else
    {
    for (int i=0; i < static_cast<int>(VLength) - 1; ++i)
      {
      os << arr[i] << ", ";
      }
    os << arr[VLength-1];
    }
  os << "]";
  return os;
}

} // namespace itk

#endif
