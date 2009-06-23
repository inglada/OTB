/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbNearestPointDeformationFieldGenerator_txx
#define __otbNearestPointDeformationFieldGenerator_txx

#include "otbNearestPointDeformationFieldGenerator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/** Main computation method */
template <class TPointSet,class TDeformationField>
void
NearestPointDeformationFieldGenerator<TPointSet, TDeformationField>
::GenerateData(void)
{
  DeformationFieldPointerType outputPtr = this->GetOutput();
  PixelType defaultValue(2);
  defaultValue.Fill(this->GetDefaultValue());
  outputPtr->Allocate();
  outputPtr->FillBuffer(defaultValue);

  typedef itk::ImageRegionIteratorWithIndex<DeformationFieldType> IteratorType;
  IteratorType it(outputPtr,outputPtr->GetRequestedRegion());

  for (it.GoToBegin();!it.IsAtEnd();++it)
  {
    IndexVectorType indexVector = this->GenerateNearestValidPointsPointSet(it.GetIndex(),1);
    PixelType p(2);
    if (indexVector.size()>=1)
    {
      p[0] = static_cast<ValueType>(this->GetPointSet()->GetPointData()->GetElement(indexVector[0])[1]);
      p[1] = static_cast<ValueType>(this->GetPointSet()->GetPointData()->GetElement(indexVector[0])[2]);
    }
    else
    {
      p=defaultValue;
    }
    it.Set(p);
  }
}
/**
 * PrintSelf Method
 */
template <class TPointSet,class TDeformationField>
void
NearestPointDeformationFieldGenerator<TPointSet, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
