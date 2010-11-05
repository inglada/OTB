/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransformFileWriter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-25 12:25:45 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTransformFileWriter_h
#define __itkTransformFileWriter_h

// First make sure that the configuration is available.
// This line can be removed once the factory based version
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_TRANSFORM_IO_FACTORIES
#include "itkTransformFileWriterWithFactory.h"
#else


#include "itkLightProcessObject.h"
#include "itkTransformBase.h"

namespace itk
{

/** \class TransformFileWriter
 * 
 * \brief TODO
 */
class TransformFileWriter : public LightProcessObject
{
public:

  /** SmartPointer typedef support */
  typedef TransformFileWriter    Self;
  typedef SmartPointer<Self>     Pointer;
  typedef TransformBase          TransformType;
  typedef TransformType::Pointer TransformPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  typedef Object Superclass;
  itkTypeMacro(TransformFileWriter, LightProcessObject);

  /** Set the filename  */
  itkSetStringMacro(FileName);

  /** Get the filename */
  itkGetStringMacro(FileName);

  /** Set/Get the write mode (append/overwrite) for the Filter */
  void SetAppendOff( );
  void SetAppendOn( );
  void SetAppendMode(bool mode);
  bool GetAppendMode( );

  /** Set/Get the input transform to write */
  void SetInput(const TransformType* transform);
  const TransformType * GetInput() {return *(m_TransformList.begin());}

  /** Add a transform to be written */
  void AddTransform(const TransformType* transform);

  /** Set/Get the precision of the writing */
  itkSetMacro(Precision,unsigned int);
  itkGetConstMacro(Precision,unsigned int);

  /** Write out the transform */
  void Update();

protected:
  TransformFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  std::string m_FileName;

  TransformFileWriter();
  virtual ~TransformFileWriter();

private:

  std::list<const TransformType*>  m_TransformList;
  unsigned int                     m_Precision;
  bool                             m_AppendMode;
};

} // namespace itk

#endif

#endif // __itkTransformFileWriter_h
