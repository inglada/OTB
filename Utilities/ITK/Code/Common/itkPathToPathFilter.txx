/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPathToPathFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-02-19 19:41:23 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPathToPathFilter_txx
#define __itkPathToPathFilter_txx

#include "itkPathToPathFilter.h"

namespace itk
{

/**
 *
 */
template <class TInputPath, class TOutputPath>
PathToPathFilter<TInputPath,TOutputPath>
::PathToPathFilter()
{
  // Let the superclass do everything
}

/**
 *
 */
template <class TInputPath, class TOutputPath>
void 
PathToPathFilter<TInputPath,TOutputPath>
::SetInput(const InputPathType *path)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0, 
                                   const_cast< InputPathType * >( path ) );
}


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputPath, class TOutputPath>
void 
PathToPathFilter<TInputPath,TOutputPath>
::SetInput( unsigned int index, const TInputPath * path ) 
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(index, 
                                   const_cast< TInputPath *>( path ) );
}

/**
 *
 */
template <class TInputPath, class TOutputPath>
const typename PathToPathFilter<TInputPath,TOutputPath>::InputPathType *
PathToPathFilter<TInputPath,TOutputPath>
::GetInput(void) 
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<const TInputPath * >
    (this->ProcessObject::GetInput(0) );
}
  
/**
 *
 */
template <class TInputPath, class TOutputPath>
const typename PathToPathFilter<TInputPath,TOutputPath>::InputPathType *
PathToPathFilter<TInputPath,TOutputPath>
::GetInput(unsigned int idx)
{
  return static_cast< const TInputPath * >
    (this->ProcessObject::GetInput(idx));
}

/**
 *
 */
template <class TInputPath, class TOutputPath>
void 
PathToPathFilter<TInputPath,TOutputPath>
::GenerateInputRequestedRegion()
{
  // ProcessObject::GenerateInputRequestedRegion() will (for each input) call
  // Path::SetRequestedRegionToLargestPossibleRegion(), which is empty.
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template <class TInputPath, class TOutputPath>
void 
PathToPathFilter<TInputPath,TOutputPath>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace itk

#endif
