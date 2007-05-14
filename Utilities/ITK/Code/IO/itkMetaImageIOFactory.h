/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaImageIOFactory.h,v $
  Language:  C++
  Date:      $Date: 2005/11/17 22:41:09 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaImageIOFactory_h
#define __itkMetaImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class MetaImageIOFactory
 * \brief Create instances of MetaImageIO objects using an object factory.
 */
class ITK_EXPORT MetaImageIOFactory : public ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef MetaImageIOFactory   Self;
  typedef ObjectFactoryBase  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion() const;
  virtual const char* GetDescription() const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MetaImageIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory()
  {
    MetaImageIOFactory::Pointer metaFactory = MetaImageIOFactory::New();
    ObjectFactoryBase::RegisterFactory(metaFactory);
  }

protected:
  MetaImageIOFactory();
  ~MetaImageIOFactory();

private:
  MetaImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
 
} // end namespace itk

#endif
