/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImportImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/05/22 02:27:16 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImportImageFilter_txx
#define _itkImportImageFilter_txx

#include "itkImportImageFilter.h"
#include "itkObjectFactory.h"

namespace itk
{

/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
ImportImageFilter<TPixel, VImageDimension>
::ImportImageFilter()
{
  unsigned int idx;
  
  for (idx = 0; idx < VImageDimension; ++idx)
    {
    m_Spacing[idx] = 1.0;
    m_Origin[idx] = 0.0;
    }

  m_ImportPointer = 0;
  m_FilterManageMemory = false;
  m_Size = 0;
}

/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
ImportImageFilter<TPixel, VImageDimension>
::~ImportImageFilter()
{
  if (m_ImportPointer && m_FilterManageMemory)
    {
    delete [] m_ImportPointer;
    }
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  int i;  

  Superclass::PrintSelf(os,indent);

  if (m_ImportPointer)
    {
    os << indent << "Imported pointer: (" << m_ImportPointer  << ")" << std::endl;
    }
  else
    {
    os << indent << "Imported pointer: (None)" << std::endl;
    }
  os << indent << "Import buffer size: " << m_Size << std::endl;
  os << indent << "Import buffer size: " << m_Size << std::endl;
  os << indent << "Filter manages memory: " << (m_FilterManageMemory ? "true" : "false") << std::endl;

  os << indent << "Spacing: [";
  for (i=0; i < static_cast<int>(VImageDimension) - 1; i++)
    {
    os << m_Spacing[i] << ", ";
    }
  os << m_Spacing[i] << "]" << std::endl;

  os << indent << "Origin: [";
  for (i=0; i < static_cast<int>(VImageDimension) - 1; i++)
    {
    os << m_Origin[i] << ", ";
    }
  os << m_Origin[i] << "]" << std::endl;
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::SetImportPointer(TPixel *ptr, unsigned long num, bool LetFilterManageMemory)
{
  if (ptr != m_ImportPointer)
    {
    if (m_ImportPointer && m_FilterManageMemory)
      {
      delete [] m_ImportPointer;
      }
    m_ImportPointer = ptr;
    this->Modified();
    }
  m_FilterManageMemory = LetFilterManageMemory;
  m_Size = num;
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
TPixel *
ImportImageFilter<TPixel, VImageDimension>
::GetImportPointer()
{
  return m_ImportPointer;
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::EnlargeOutputRequestedRegion(DataObject *output) 
{
  // call the superclass' implementation of this method
  Superclass::EnlargeOutputRequestedRegion(output);

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // set the requested region to the largest possible region (in this case
  // the amount of data that we have)
  outputPtr->SetRequestedRegion( outputPtr->GetLargestPossibleRegion() );
}


/** 
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // we need to compute the output spacing, the output origin, the
  // output image size, and the output image start index
  outputPtr->SetSpacing( m_Spacing );
  outputPtr->SetOrigin( m_Origin );
  outputPtr->SetLargestPossibleRegion( m_Region );
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::GenerateData()
{
  // Normally, GenerateData() allocates memory.  However, the application
  // provides the memory for this filter via the SetImportPointer() method.
  // Therefore, this filter does not call outputPtr->Allocate().
  
  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // the output buffer size is set to the size specified by the user via the
  // SetRegion() method.
  outputPtr->SetBufferedRegion( outputPtr->GetLargestPossibleRegion() );

  // pass the pointer down to the container during each Update() since
  // a call to Initialize() causes the container to forget the
  // pointer.  Note that we tell the container NOT to manage the
  // memory itself.  This filter will properly manage the memory (as
  // opposed to the container) if the user wants it to.
  outputPtr->GetPixelContainer()->SetImportPointer( m_ImportPointer,
                                                    m_Size, false );
}


/** 
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::SetSpacing( const SpacingType & spacing )
{
  double dspacing[VImageDimension];
  for(unsigned int i=0; i<VImageDimension; i++)
    {
    dspacing[i] = spacing[i];
    }
  this->SetSpacing( dspacing );
}


/** 
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
ImportImageFilter<TPixel, VImageDimension>
::SetOrigin( const OriginType & origin )
{
  double dorigin[VImageDimension];
  for(unsigned int i=0; i<VImageDimension; i++)
    {
    dorigin[i] = origin[i];
    }
  this->SetOrigin( dorigin );
}


} // end namespace itk

#endif
