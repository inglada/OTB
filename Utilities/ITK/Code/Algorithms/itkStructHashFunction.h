/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStructHashFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-05-12 17:26:21 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStructHashFunction_h
#define __itkStructHashFunction_h

namespace itk
{

/** \class StructHashFunction
 *
 *  \brief Generic hash function for an arbitrary struct (or class).
 *
 *  This regards the input key as a string of bytes, and applies a
 *  hash function similar to one that has been used in perl.  If the
 *  data in the input key has pointers to other classes, then the
 *  function will be hashing on the value of the pointer itself, not
 *  on the data it points to.  It is imagined that this function will
 *  be used more for structs (with fully exposed data) than for
 *  general classes.
 */
template< class TInput >
class ITK_EXPORT StructHashFunction
{
public:

  /** Standard class typedefs. */
  typedef StructHashFunction Self;

  /** Input type */
  typedef TInput InputType;

  unsigned int operator()( const InputType& key ) const;

};

template< class TInput >
inline unsigned int
StructHashFunction< TInput >
::operator()( const InputType& key ) const
{
  ::size_t len = sizeof( InputType );
  const char* p = reinterpret_cast< const char* >( &key );
  unsigned int hash = 0;
  while( len-- )
    {
    hash = hash * 65 + *p++;
    }
  hash += (hash >> 5);
  return hash;
}

}

#endif  // ndef itkStructHashFunction_h
