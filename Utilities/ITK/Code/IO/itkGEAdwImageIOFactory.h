/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkGEAdwImageIOFactory.h,v $
Language:  C++
Date:      $Date: 2004/01/08 00:39:05 $
Version:   $Revision: 1.5 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGEAdwImageIOFactory_h
#define __itkGEAdwImageIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class GEAdwImageIOFactory
   * \brief Create instances of GEAdwImageIO objects using an object factory.
   */
class ITK_EXPORT GEAdwImageIOFactory : public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef GEAdwImageIOFactory   Self;
  typedef ObjectFactoryBase  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GEAdwImageIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    GEAdwImageIOFactory::Pointer metaFactory = GEAdwImageIOFactory::New();
    ObjectFactoryBase::RegisterFactory(metaFactory);
  }

protected:
  GEAdwImageIOFactory();
  ~GEAdwImageIOFactory();
  virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:
  GEAdwImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace itk

#endif
