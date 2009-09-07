/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOGRVectorDataIOFactory_h
#define __otbOGRVectorDataIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class OGRVectorDataIOFactory
 * \brief Creation d'un instance d'un objet VectorDataImageIO utilisant les object factory.
 */
template <class TData> class ITK_EXPORT OGRVectorDataIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OGRVectorDataIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRVectorDataIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    typename OGRVectorDataIOFactory<TData>::Pointer SHPFactory = OGRVectorDataIOFactory<TData>::New();
    itk::ObjectFactoryBase::RegisterFactory(SHPFactory);
  }

protected:
  OGRVectorDataIOFactory();
  virtual ~OGRVectorDataIOFactory();

private:
  OGRVectorDataIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRVectorDataIOFactory.txx"
#endif

#endif
