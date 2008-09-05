/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaArrayWriter.cxx,v $
  Language:  C++
  Date:      $Date: 2007-03-22 14:28:51 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaArrayWriter_cxx
#define __itkMetaArrayWriter_cxx

#include "itkMetaArrayWriter.h"

namespace itk
{

MetaArrayWriter
::MetaArrayWriter()
{
  m_FileName = "";
  m_Precision = 6;
  m_Buffer = NULL;
  m_Binary = false;
}

MetaArrayWriter
::~MetaArrayWriter()
{
}

void MetaArrayWriter
::ConvertTo(MET_ValueEnumType _metaElementType)
{
  if(m_Buffer != NULL)
    {
    m_MetaArray.ImportBufferToElementData(m_Buffer,
                                          m_MetaArray.ElementType());
    }
  m_MetaArray.ConvertElementDataTo(_metaElementType);
}

/** Update the writer */
void MetaArrayWriter
::Update()
{  
  m_MetaArray.SetDoublePrecision(m_Precision);
  
  m_MetaArray.BinaryData(m_Binary);
  if(m_Buffer != NULL)
    {
    m_MetaArray.Write(m_FileName.c_str(), m_DataFileName.c_str(),
                      true, m_Buffer);
    }

  m_DataFileName.erase();
}

} // namespace itk

#endif
