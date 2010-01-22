/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageAndPathToImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004-05-03 16:58:49 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkImageAndPathToImageFilter_txx
#define __itkImageAndPathToImageFilter_txx

#include "itkImageAndPathToImageFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputImage, class TInputPath, class TOutputImage>
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::ImageAndPathToImageFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(2);
}

/**
 *
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::SetImageInput(const InputImageType *image)
{
  // We have 2 inputs:  a path and an image

  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0, 
                                   const_cast< InputImageType * >( image ) );
}

template <class TInputImage, class TInputPath, class TOutputImage>
const typename ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>::InputImageType *
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::GetImageInput(void) 
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<const TInputImage * >
    (this->ProcessObject::GetInput(0) );
}
  
template <class TInputImage, class TInputPath, class TOutputImage>
void
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::SetPathInput(const InputPathType *path)
{
  // We have 2 inputs:  a path and an image

  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(1, 
                                   const_cast< InputPathType * >( path ) );
}

template <class TInputImage, class TInputPath, class TOutputImage>
const typename ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>::InputPathType *
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::GetPathInput(void) 
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  
  return static_cast<const TInputPath * >
    (this->ProcessObject::GetInput(1) );
}
  
/**
 *
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace itk

#endif
