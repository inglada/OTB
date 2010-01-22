/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToMeshFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 16:45:10 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToMeshFilter_txx
#define __itkImageToMeshFilter_txx
#include "itkImageToMeshFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputMesh>
ImageToMeshFilter<TInputImage,TOutputMesh>
::ImageToMeshFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);

  OutputMeshPointer output
    = dynamic_cast<OutputMeshType*>(this->MakeOutput(0).GetPointer()); 

  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, output.GetPointer());

}

/**
 *
 */
template <class TInputImage, class TOutputMesh>
ImageToMeshFilter<TInputImage,TOutputMesh>
::~ImageToMeshFilter()
{
}
  

/**
 *   Make Ouput
 */
template <class TInputImage, class TOutputMesh>
DataObject::Pointer
ImageToMeshFilter<TInputImage,TOutputMesh>
::MakeOutput(unsigned int)
{
  OutputMeshPointer  outputMesh = OutputMeshType::New();
  return dynamic_cast< DataObject *>( outputMesh.GetPointer() );
}

/**
 *
 */
template <class TInputImage, class TOutputMesh>
void 
ImageToMeshFilter<TInputImage,TOutputMesh>
::SetInput(unsigned int idx,const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObject::SetNthInput(idx, 
                                   const_cast< InputImageType * >(input) );
}


  
/**
 *
 */
template <class TInputImage, class TOutputMesh>
const typename ImageToMeshFilter<TInputImage,TOutputMesh>::InputImageType *
ImageToMeshFilter<TInputImage,TOutputMesh>
::GetInput(unsigned int idx) 
{
  return dynamic_cast<const InputImageType*>
    (this->ProcessObject::GetInput(idx));
}

 
/**
 *
 */
template <class TInputImage, class TOutputMesh>
typename ImageToMeshFilter<TInputImage,TOutputMesh>::OutputMeshType *
ImageToMeshFilter<TInputImage,TOutputMesh>
::GetOutput(void) 
{
  return dynamic_cast<OutputMeshType*>
    (this->ProcessObject::GetOutput(0));
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the 
 * ProcessObject version to be called
 */
template <class TInputImage, class TOutputMesh>
void 
ImageToMeshFilter<TInputImage,TOutputMesh>
::GenerateOutputInformation()
{
}


} // end namespace itk

#endif
