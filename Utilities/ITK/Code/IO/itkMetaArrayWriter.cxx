/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaArrayWriter.cxx,v $
  Language:  C++
  Date:      $Date: 2006/10/27 12:26:31 $
  Version:   $Revision: 1.2 $

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

  if(m_Buffer != NULL)
    {
    m_MetaArray.Write(m_FileName.c_str(), m_DataFileName.c_str(),
                      true, m_Buffer);
    }

  m_DataFileName.erase();
  }


} // namespace itk

#endif
