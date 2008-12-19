/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkByteSwapper.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-10 14:28:21 $
  Version:   $Revision: 1.22 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkByteSwapper_txx
#define __itkByteSwapper_txx
#include "itkByteSwapper.h"
#include "itkObjectFactory.h"
#include <memory>
#include <cstring>

namespace itk
{

// The following are the public methods --------------------------------
//
// Machine definitions
#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
bool ByteSwapper<T>::SystemIsBigEndian() { return true; }
template <class T>
bool ByteSwapper<T>::SystemIsLittleEndian() { return false; }
#else
template <class T>
bool ByteSwapper<T>::SystemIsBigEndian() { return false; }
template <class T>
bool ByteSwapper<T>::SystemIsLittleEndian() { return true; }
#endif  

  
//------Big Endian methods----------------------------------------------

// Use different swap methods based on type
#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapFromSystemToBigEndian(T *){}
#else
template <class T>
void 
ByteSwapper<T>
::SwapFromSystemToBigEndian(T *p)
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::Swap2((void *)p);
      return;
    case 4:
      ByteSwapper<T>::Swap4((void *)p);
      return;
    case 8:
      ByteSwapper<T>::Swap8((void *)p);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
    }
}
#endif  

#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapRangeFromSystemToBigEndian(T *, BufferSizeType )
{  
  // nothing needs to be done here...
}
#else
template <class T>
void 
ByteSwapper<T>
::SwapRangeFromSystemToBigEndian(T *p, BufferSizeType num)
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::Swap2Range((void *)p,num);
      return;
    case 4:
      ByteSwapper<T>::Swap4Range((void *)p,num);
      return;
    case 8:
      ByteSwapper<T>::Swap8Range((void *)p,num);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
      return;
    }
}
#endif  

#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapWriteRangeFromSystemToBigEndian(T *p, int num, OStreamType *fp)
{
  num *= sizeof(T);
  fp->write((char *)p, num);
}
#else
template <class T>
void 
ByteSwapper<T>
::SwapWriteRangeFromSystemToBigEndian(T *p, int num, OStreamType *fp)
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::SwapWrite2Range((void *)p, num, fp);
      return;
    case 4:
      ByteSwapper<T>::SwapWrite4Range((void *)p, num, fp);
      return;
    case 8:
      ByteSwapper<T>::SwapWrite8Range((void *)p, num, fp);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
    }
}
#endif

//------Little Endian methods----------------------------------------------

#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapFromSystemToLittleEndian(T *p)
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::Swap2((void *)p);
      return;
    case 4:
      ByteSwapper<T>::Swap4((void *)p);
      return;
    case 8:
      ByteSwapper<T>::Swap8((void *)p);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
    }
}
#else
template <class T>
void 
ByteSwapper<T>
::SwapFromSystemToLittleEndian(T *){}
#endif  


#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapRangeFromSystemToLittleEndian(T *p, BufferSizeType num) 
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::Swap2Range((void *)p,num);
      return;
    case 4:
      ByteSwapper<T>::Swap4Range((void *)p,num);
      return;
    case 8:
      ByteSwapper<T>::Swap8Range((void *)p,num);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
    }
}
#else
template <class T>
void 
ByteSwapper<T>
::SwapRangeFromSystemToLittleEndian(T *, BufferSizeType) {}
#endif  

#ifdef CMAKE_WORDS_BIGENDIAN
template <class T>
void 
ByteSwapper<T>
::SwapWriteRangeFromSystemToLittleEndian(T *p, int num, OStreamType *fp)
{
  switch ( sizeof(T) )
    {
    case 1:
      return;
    case 2:
      ByteSwapper<T>::SwapWrite2Range((void *)p, num, fp);
      return;
    case 4:
      ByteSwapper<T>::SwapWrite4Range((void *)p, num, fp);
      return;
    case 8:
      ByteSwapper<T>::SwapWrite8Range((void *)p, num, fp);
      return;
    default:  
      itkGenericExceptionMacro (<< "Cannot swap number of bytes requested");
    }
}
#else
template <class T>
void 
ByteSwapper<T>
::SwapWriteRangeFromSystemToLittleEndian(T *p, int num, OStreamType *fp)
{
  num *= sizeof(T);
  fp->write((char *)p, num);
}
#endif

// The following are the protected methods -------------------------
//

//------2-byte methods----------------------------------------------

// Swap 2 byte word.
template <class T>
void 
ByteSwapper<T>
::Swap2(void *pin)
{
  unsigned short h1,h2;
  short* p = reinterpret_cast<short*>(pin);
  
  h1 = static_cast<unsigned short>(*p) << 8;
  h2 = static_cast<unsigned short>(*p) >> 8;
  *p = (short) h1 | h2;
}

// Swap bunch of bytes. Num is the number of two byte words to swap.
template <class T>
void 
ByteSwapper<T>
::Swap2Range(void *ptr, BufferSizeType num)
{
  char one_byte;
  char *pos = reinterpret_cast<char *>(ptr);
  BufferSizeType i;
  
  for (i = 0; i < num; i++)
    {
    one_byte = pos[0];
    pos[0] = pos[1];
    pos[1] = one_byte;
    pos = pos + 2;
    }
}

// Swap bunch of bytes. Num is the number of four byte words to swap.
template <class T>
void 
ByteSwapper<T>
::SwapWrite2Range(void *ptr, BufferSizeType num, OStreamType *fp)
{
  char one_byte;
  char *pos;
  BufferSizeType i;
  char *cpy;
  BufferSizeType chunkSize = 1000000;

  if (num < chunkSize)
    {
    chunkSize = num;
    }
  cpy = new char [chunkSize * 2];
 
  while (num)
    {
    memcpy(cpy, ptr, chunkSize * 2);
 
    pos = cpy; 
    for (i = 0; i < chunkSize; i++)
      {
      one_byte = pos[0];
      pos[0] = pos[1];
      pos[1] = one_byte;
      pos = pos + 2;
      }
    fp->write((char *)cpy, 2*chunkSize);
    ptr = (char *) ptr + chunkSize * 2;
    num -= chunkSize;
    if (num < chunkSize)
      {
      chunkSize = num;
      }
    }
  delete [] cpy;
}

//------4-byte methods----------------------------------------------

// Swap four byte word.
template <class T>
void 
ByteSwapper<T>
::Swap4(void *ptr)
{
  char one_byte;
  char *p = reinterpret_cast<char *>(ptr);

  one_byte    = p[0];
  p[0] = p[3];
  p[3] = one_byte;

  one_byte    = p[1];
  p[1] = p[2];
  p[2] = one_byte;
}

// Swap bunch of bytes. Num is the number of four byte words to swap.
template <class T>
void 
ByteSwapper<T>
::Swap4Range(void *ptr, BufferSizeType num)
{
  char one_byte;
  char *pos = reinterpret_cast<char *>(ptr);
  BufferSizeType i;

  for (i = 0; i < num; i++)
    {
    one_byte = pos[0];
    pos[0] = pos[3];
    pos[3] = one_byte;
    
    one_byte = pos[1];
    pos[1] = pos[2];
    pos[2] = one_byte;
    pos = pos + 4;
    }
}

// Swap bunch of bytes. Num is the number of four byte words to swap.
template <class T>
void 
ByteSwapper<T>
::SwapWrite4Range(void *ptr, BufferSizeType num, OStreamType *fp)
{
  char one_byte;
  char *pos;
  BufferSizeType i;
  char *cpy;
  BufferSizeType chunkSize = 1000000;

  if (num < chunkSize)
    {
    chunkSize = num;
    }
  cpy = new char [chunkSize * 4];
 
  while (num)
    {
    memcpy(cpy, ptr, chunkSize * 4);
    
    pos = cpy;   
    for (i = 0; i < chunkSize; i++)
      {
      one_byte = pos[0];
      pos[0] = pos[3];
      pos[3] = one_byte;
      
      one_byte = pos[1];
      pos[1] = pos[2];
      pos[2] = one_byte;
      pos = pos + 4;
      }
    fp->write((char *)cpy, 4*chunkSize);
    ptr  = (char *) ptr + chunkSize*4;
    num -= chunkSize;
    if (num < chunkSize)
      {
      chunkSize = num;
      }
    }
  delete [] cpy;
}

//------8-byte methods----------------------------------------------

// Swap 8 byte double precision
template <class T>
void 
ByteSwapper<T>
::Swap8(void *ptr)
{
  char one_byte;
  char *p = reinterpret_cast<char *>(ptr);

  one_byte    = p[0];
  p[0] = p[7];
  p[7] = one_byte;

  one_byte    = p[1];
  p[1] = p[6];
  p[6] = one_byte;

  one_byte    = p[2];
  p[2] = p[5];
  p[5] = one_byte;

  one_byte    = p[3];
  p[3] = p[4];
  p[4] = one_byte;
}

// Swap bunch of bytes. Num is the number of eight byte words to swap.
template <class T>
void 
ByteSwapper<T>
::Swap8Range(void *ptr, BufferSizeType num)
{
  char one_byte;
  char *pos = reinterpret_cast<char *>(ptr);
  BufferSizeType i;

  for (i = 0; i < num; i++)
    {
    one_byte    = pos[0];
    pos[0] = pos[7];
    pos[7] = one_byte;

    one_byte    = pos[1];
    pos[1] = pos[6];
    pos[6] = one_byte;

    one_byte    = pos[2];
    pos[2] = pos[5];
    pos[5] = one_byte;

    one_byte    = pos[3];
    pos[3] = pos[4];
    pos[4] = one_byte;
    pos = pos + 8;
    }
}

// Swap bunch of bytes. Num is the number of four byte words to swap.
template <class T>
void 
ByteSwapper<T>
::SwapWrite8Range(void *ptr, BufferSizeType num, OStreamType *fp)
{
  char one_byte;
  char *pos;
  BufferSizeType i;
  char *cpy;
  BufferSizeType chunkSize = 1000000;

  if (num < chunkSize)
    {
    chunkSize = num;
    }
  cpy = new char [chunkSize * 8];
 
  while (num)
    {
    memcpy(cpy, ptr, chunkSize * 8);
    
    pos = cpy;   
    for (i = 0; i < chunkSize; i++)
      {
      one_byte    = pos[0];
      pos[0] = pos[7];
      pos[7] = one_byte;

      one_byte    = pos[1];
      pos[1] = pos[6];
      pos[6] = one_byte;

      one_byte    = pos[2];
      pos[2] = pos[5];
      pos[5] = one_byte;

      one_byte    = pos[3];
      pos[3] = pos[4];
      pos[4] = one_byte;
      pos = pos + 8;
      }
    fp->write((char *)cpy, 8*chunkSize);
    ptr  = (char *) ptr + chunkSize*8;
    num -= chunkSize;
    if (num < chunkSize)
      {
      chunkSize = num;
      }
    }
  delete [] cpy;
}

} // end namespace itk

#endif
