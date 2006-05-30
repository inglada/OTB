/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLightObject.cxx,v $
  Language:  C++
  Date:      $Date: 2006/02/06 22:01:56 $
  Version:   $Revision: 1.31 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "itkFastMutexLock.h"

#include <list>
#include <memory>

// Better name demanging for gcc
#if __GNUC__ > 3 || ( __GNUC__ == 3 && __GNUC_MINOR__ > 0 )
#define GCC_USEDEMANGLE
#endif

#ifdef GCC_USEDEMANGLE
#include <cstdlib>
#include <cxxabi.h>
#endif 

namespace itk
{

LightObject::Pointer
LightObject::New()
{
  Pointer smartPtr;
  LightObject *rawPtr = ::itk::ObjectFactory<LightObject>::Create();
  if(rawPtr == NULL)
    {
    rawPtr = new LightObject;
    }
  smartPtr = rawPtr;
  rawPtr->UnRegister();
  return smartPtr;
}

LightObject::Pointer
LightObject::CreateAnother() const
{
  return LightObject::New();
}

/**
 * Delete a itk object. This method should always be used to delete an object 
 * when the new operator was used to create it. Using the C++ delete method
 * will not work with reference counting.
 */
void 
LightObject
::Delete() 
{
  this->UnRegister();
}


/**
 * Avoid DLL boundary problems.
 */
#ifdef _WIN32
void*
LightObject
::operator new(size_t n)
{
  return new char[n];
}

void*
LightObject
::operator new[](size_t n)
{
  return new char[n];
}

void
LightObject
::operator delete(void* m)
{
  delete [] (char*)m;
}

void
LightObject
::operator delete[](void* m, size_t)
{
  delete [] (char*)m;
}
#endif 


/**
 * This function will be common to all itk objects.  It just calls the
 * header/self/trailer virtual print methods, which can be overriden by
 * subclasses (any itk object).
 */
void 
LightObject
::Print(std::ostream& os, Indent indent) const
{
  this->PrintHeader(os, indent); 
  this->PrintSelf(os, indent.GetNextIndent());
  this->PrintTrailer(os, indent);
}


/**
 * This method is called when itkExceptionMacro executes. It allows 
 * the debugger to break on error.
 */
void 
LightObject
::BreakOnError()
{
  ;  
}


/**
 * Increase the reference count (mark as used by another object).
 */
void 
LightObject
::Register() const
{
  m_ReferenceCountLock.Lock();
  m_ReferenceCount++;
  m_ReferenceCountLock.Unlock();
}


/**
 * Decrease the reference count (release by another object).
 */
void 
LightObject
::UnRegister() const
{
  m_ReferenceCountLock.Lock();
  m_ReferenceCount--;
  m_ReferenceCountLock.Unlock();
  
  // ReferenceCount in now unlocked.  We may have a race condition
  // to delete the object.
  if ( m_ReferenceCount <= 0)
    {
    delete this;
    }
}


/**
 * Sets the reference count (use with care)
 */
void 
LightObject
::SetReferenceCount(int ref)
{
  m_ReferenceCountLock.Lock();
  m_ReferenceCount = ref;
  m_ReferenceCountLock.Unlock();

  if ( m_ReferenceCount <= 0)
    {
    delete this;
    }
}

LightObject
::~LightObject() 
{
  /**
   * warn user if reference counting is on and the object is being referenced
   * by another object
   */
  if ( m_ReferenceCount > 0)
    {
    itkExceptionMacro(<< "Trying to delete object with non-zero reference count.");
    }
}


/**
 * Chaining method to print an object's instance variables, as well as
 * its superclasses.
 */
void 
LightObject
::PrintSelf(std::ostream& os, Indent indent) const
{
#ifdef GCC_USEDEMANGLE
  char const * mangledName = typeid(*this).name();
  int status;
  char* unmangled = abi::__cxa_demangle(mangledName, 0, 0, &status);
  
  os << indent << "RTTI typeinfo:   ";

  if(status == 0)
    {
    os << unmangled;
    free(unmangled);
    }
  else
    {
    os << mangledName;
    }

  os << std::endl;
#else
  os << indent << "RTTI typeinfo:   " << typeid( *this ).name() << std::endl;
#endif
  os << indent << "Reference Count: " << m_ReferenceCount << std::endl;
}


/**
 * Define a default print header for all objects.
 */
void 
LightObject
::PrintHeader(std::ostream& os, Indent indent) const
{
  os << indent << this->GetNameOfClass() << " (" << this << ")\n";
}


/**
 * Define a default print trailer for all objects.
 */
void 
LightObject
::PrintTrailer(std::ostream& itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


/**
 * This operator allows all subclasses of LightObject to be printed via <<.
 * It in turn invokes the Print method, which in turn will invoke the
 * PrintSelf method that all objects should define, if they have anything
 * interesting to print out.
 */
std::ostream& 
operator<<(std::ostream& os, const LightObject& o)
{
  o.Print(os);
  return os;
}


} // end namespace itk
