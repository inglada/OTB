/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransformFileReader.h,v $
  Language:  C++
  Date:      $Date: 2009-01-16 11:37:58 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTransformFileReader_h
#define __itkTransformFileReader_h

// First make sure that the configuration is available.
// This line can be removed once the factory based version
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_TRANSFORM_IO_FACTORIES
#include "itkTransformFileReaderWithFactory.h"
#else

#include "itkLightProcessObject.h"
#include "metaTransform.h"
#include "itkTransformBase.h"

namespace itk
{

/** \class TransformFileReader
 * 
 * \brief TODO
 */
class TransformFileReader : public LightProcessObject
{
public:

  /** SmartPointer typedef support */
  typedef TransformFileReader Self;
  typedef SmartPointer<Self>  Pointer;
  typedef TransformBase       TransformType;

  typedef TransformType::ParametersType ParametersType;
  typedef TransformType::Pointer        TransformPointer;
  typedef std::list<TransformPointer>   TransformListType;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  typedef Object Superclass;
  itkTypeMacro(TransformFileReader, LightProcessObject);

  /** Set the filename  */
  itkSetStringMacro(FileName);

  /** Get the filename */
  itkGetStringMacro(FileName);

  /** Write out the transform */
  void Update();

  /** Get the list of transform */
  TransformListType * GetTransformList() {return & m_TransformList;}

protected:
  TransformFileReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  std::string m_FileName;

  TransformFileReader();
  virtual ~TransformFileReader();

private:

  TransformListType    m_TransformList;
};

} // namespace itk

#endif

#endif // __itkTransformFileReader_h
