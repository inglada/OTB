/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageShapeModelEstimatorBase.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:32 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageShapeModelEstimatorBase_txx
#define _itkImageShapeModelEstimatorBase_txx

#include "itkImageShapeModelEstimatorBase.h"
#include "itkCommand.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
ImageShapeModelEstimatorBase<TInputImage, TOutputImage>
::ImageShapeModelEstimatorBase(void)
{

}

template <class TInputImage, class TOutputImage>
ImageShapeModelEstimatorBase<TInputImage, TOutputImage>
::~ImageShapeModelEstimatorBase()
{

}

template <class TInputImage, class TOutputImage>
void
ImageShapeModelEstimatorBase<TInputImage, TOutputImage>
::GenerateData()
{
  this->EstimateShapeModels();
}

/*
 * PrintSelf
 */
template <class TInputImage, class TOutputImage>
void
ImageShapeModelEstimatorBase<TInputImage, TOutputImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "                   " << std::endl;

  os << indent << "InputImage: ";
  os << m_InputImage.GetPointer() << std::endl;


}// end PrintSelf

} // namespace itk

#endif
