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
#ifndef __otbLocalHistogramIFFactory_txx
#define __otbLocalHistogramIFFactory_txx

#include "otbLocalHistogramIFFactory.h"
#include "otbStreamingMinMaxImageFilter.h"


namespace otb
{
template <class TImageType, class TCoordRep, class TPrecision>
void
LocalHistogramIFFactory<TImageType, TCoordRep, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TImageType, class TCoordRep, class TPrecision>
void
LocalHistogramIFFactory<TImageType, TCoordRep, TPrecision>
::Create(InputImageType * image, 
         ParamContainerType param, 
         MetaImageFunctionPointerType metaIF, 
         DataObjectContainerType * container)
{
  typename AdaptedLocalHistogramIF::Pointer function = AdaptedLocalHistogramIF::New();
  
  typedef otb::StreamingMinMaxImageFilter<InputImageType>               MinMaxFilterType;
  typename MinMaxFilterType::Pointer filter = MinMaxFilterType::New();
  filter->SetInput(image);
  filter->Update();

  function->SetInputImage(image);
  function->GetInternalImageFunction()->SetNeighborhoodRadius(param[0]);
  function->GetInternalImageFunction()->SetNumberOfHistogramBins(param[1]);
  function->GetInternalImageFunction()->SetHistogramMin(filter->GetMinimum());
  function->GetInternalImageFunction()->SetHistogramMax(filter->GetMaximum());

  metaIF->AddFunction(function);
  container->push_back(image);
}

}//end namespace
#endif
