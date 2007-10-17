/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDynamicLoader.h,v $
  Language:  C++
  Date:      $Date: 2006/11/29 18:19:00 $
  Version:   $Revision: 1.26 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDynamicLoader_h
#define __itkDynamicLoader_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include <itksys/DynamicLoader.hxx>

namespace itk
{

/** \class DynamicLoader
 * \brief Portable loading of dynamic libraries or dll's.
 *
 * DynamicLoader provides a portable interface to loading dynamic
 * libraries or dll's into a process.
 *
 * \ingroup OSSystemObjects
 */

//BTX
typedef itksys::DynamicLoader::LibraryHandle LibHandle;
// Cannot use this as this is a void (*)() but ITK old API used to be void*
typedef itksys::DynamicLoader::SymbolPointer SymbolPointer;
//ETX

class ITKCommon_EXPORT DynamicLoader : public Object
{
public:
  /** Standard class typedefs. */
  typedef DynamicLoader       Self;
  typedef Object  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DynamicLoader,Object);

  /** Load a dynamic library into the current process.
   * The returned LibHandle can be used to access the symbols in the
   * library. */
  static LibHandle OpenLibrary(const char*);

  /** Attempt to detach a dynamic library from the
   * process.  A value of true is returned if it is sucessful. */
  static int CloseLibrary(LibHandle);

  /** Find the address of the symbol in the given library. */
  static void* GetSymbolAddress(LibHandle, const char*);

  /** Return the library prefix for the given architecture */
  static const char* LibPrefix();

  /** Return the library extension for the given architecture. */
  static const char* LibExtension();

  /** Return the last error produced from a calls made on this class. */
  static const char* LastError();

protected:
  DynamicLoader();
  ~DynamicLoader();

private:
  DynamicLoader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
