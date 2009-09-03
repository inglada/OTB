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

#ifndef __otbLabelMapSource_txx
#define __otbLabelMapSource_txx

#include "otbLabelMapSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputLabelMap>
LabelMapSource<TOutputLabelMap>
::LabelMapSource()
{
  //std::cout <<"labelsource constructor"<<std::endl;
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0,OutputLabelMapType::New().GetPointer());
}

template <class TOutputLabelMap>
LabelMapSource<TOutputLabelMap>
::~LabelMapSource()
{
}

template <class TOutputLabelMap>
void
LabelMapSource<TOutputLabelMap>
::AllocateOutputs()
{
  OutputLabelMapPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i=0; i < this->GetNumberOfOutputs(); ++i)
  {
    outputPtr = this->GetOutput(i);
    
    //TODO Need to implement the Clear Method in the class itk::LabelMap
    //outputPtr->Clear();
  }
}


/**
 * Get the output LabelMap
 * \return The LabelMap produced.
 */
template <class TOutputLabelMap>
typename LabelMapSource<TOutputLabelMap>::OutputLabelMapType *
LabelMapSource<TOutputLabelMap>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs()<1)
  {
    return 0;
  }
  //std::cout << "labelsource: get output..." <<std::endl;
  return static_cast<OutputLabelMapType *> (this->ProcessObject::GetOutput(0));
}

template <class TOutputLabelMap>
typename LabelMapSource<TOutputLabelMap>::OutputLabelMapType *
LabelMapSource<TOutputLabelMap>
::GetOutput(unsigned int idx)
{
  return static_cast<OutputLabelMapType*>
         (this->Superclass::GetOutput(idx));
}

/**
 * PrintSelf Method
 */
template<class TOutputLabelMap>
void
LabelMapSource<TOutputLabelMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// End namespace otb

#endif
