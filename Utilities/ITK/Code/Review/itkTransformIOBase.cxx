/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransformIOBase.cxx,v $
  Language:  C++
  Date:      $Date: 2010-06-22 17:29:12 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkTransformIOBase.h"
#include "itkTransformFactoryBase.h"
#include <iostream>
#include <fstream>
#include <string>

namespace itk
{
TransformIOBase::
TransformIOBase()
{
  this->m_AppendMode = false;
}
TransformIOBase::
~TransformIOBase()
{
}

void
TransformIOBase::
CreateTransform(TransformPointer &ptr,
                  const std::string &ClassName)
{
  // Instantiate the transform
  itkDebugMacro ( "About to call ObjectFactory" );
  LightObject::Pointer i;
  i = ObjectFactoryBase::CreateInstance ( ClassName.c_str() );
  itkDebugMacro ( "After call ObjectFactory");
  ptr = dynamic_cast<TransformBase*> ( i.GetPointer() );
  if ( ptr.IsNull() )
    {
    OStringStream msg;
    msg << "Could not create an instance of " << ClassName << std::endl
        << "The usual cause of this error is not registering the "
        << "transform with TransformFactory" << std::endl;
    msg << "Currently registered Transforms: " << std::endl;
    std::list<std::string> names = TransformFactoryBase::GetFactory()->GetClassOverrideWithNames();
    std::list<std::string>::iterator it;
    for ( it = names.begin(); it != names.end(); it++ )
      {
      msg << "\t\"" << *it << "\"" << std::endl;
      }
    itkExceptionMacro ( << msg.str() );
    }
}

void 
TransformIOBase
::OpenStream(std::ofstream &outputStream, bool binary)
{
#ifdef __sgi
  // Create the file. This is required on some older sgi's
  if (this->m_AppendMode)
    {
    std::ofstream tFile(m_FileName.c_str(),std::ios::out | std::ios::app);
    tFile.close();   
    }
  else
    {
    std::ofstream tFile(m_FileName.c_str(),std::ios::out);
    tFile.close(); 
    }
#endif
  std::ios::openmode mode(std::ios::out);
  if(binary) 
    {
    mode |= std::ios::binary;
    }
  if (this->m_AppendMode)
    {
    mode |= std::ios::app; 
    }

  outputStream.open(m_FileName.c_str(), mode);

  if( outputStream.fail() )
    {
    outputStream.close();
    itkExceptionMacro("Failed opening file" << m_FileName );
    }
}

void TransformIOBase::SetTransformList(ConstTransformListType &transformList)
{ 
  this->m_WriteTransformList = transformList;
}

void TransformIOBase::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "AppendMode: " << 
    (m_AppendMode ? "true" : "false") << std::endl;
  if(m_ReadTransformList.size() > 0)
    {
    os << indent << "ReadTransformList: " << std::endl;
    TransformListType::const_iterator it = m_ReadTransformList.begin();
    while( it != m_ReadTransformList.end() )
      {
      (*it)->Print(os,indent.GetNextIndent());
      ++it;
      }
    }
  if(m_WriteTransformList.size() > 0)
    {
    os << indent << "WriteTransformList: " << std::endl;

    ConstTransformListType::const_iterator it = m_WriteTransformList.begin();
    while( it != m_WriteTransformList.end() )
      {
      (*it)->Print(os,indent.GetNextIndent());
      ++it;
      }
    }
}

} // itk
