/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRBFBackPropagationLearningFunction.txx,v $
  Language:  C++
  Date:      $Date: 2007-08-17 13:10:57 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRBFBackPropagationLearningFunction_txx
#define __itkRBFBackPropagationLearningFunction_txx

#include "itkRBFBackPropagationLearningFunction.h"

namespace itk
{
namespace Statistics
{

template<class LayerType, class TTargetVector>
RBFBackPropagationLearningFunction<LayerType,TTargetVector>
::RBFBackPropagationLearningFunction()
{
  m_LearningRate1 = 0.05;
  m_LearningRate2 = 3;
  m_LearningRate3 = 0.75;
}

template<class LayerType, class TTargetVector>
void
RBFBackPropagationLearningFunction<LayerType,TTargetVector>
::Learn(LayerType* layer,ValueType lr)
{
  typename LayerType::WeightSetType::Pointer outputweightset;
  typename LayerType::WeightSetType::Pointer inputweightset;
  outputweightset = layer->GetOutputWeightSet();
  inputweightset = layer->GetInputWeightSet();
 
  typedef typename LayerType::InputVectorType InputVectorType;
  typedef typename LayerType::OutputVectorType OutputVectorType;

  typedef RBFLayer<InputVectorType,OutputVectorType>
                                                            RbfLayerType;
  typedef typename RbfLayerType::InternalVectorType ArrayType;
  typename LayerType::ValuePointer currentdeltavalues = inputweightset->GetTotalDeltaValues();
  vnl_matrix<ValueType> DW_temp(currentdeltavalues,inputweightset->GetNumberOfOutputNodes(),
                                           inputweightset->GetNumberOfInputNodes());
  typename LayerType::ValuePointer DBValues = inputweightset->GetDeltaBValues();
  vnl_vector<ValueType> DB;
  DB.set_size(inputweightset->GetNumberOfOutputNodes());
  DB.fill(0);
  DB.copy_in(DBValues);

  if(layer->GetLayerTypeCode()==LayerInterfaceType::OUTPUTLAYER) //If output layer do back propagation
    {
    DW_temp*=lr;
    inputweightset->SetDWValues(DW_temp.data_block());
    DB*=lr;
    inputweightset->SetDBValues(DB.data_block()); 
    }
  else //else update centers, widths using gradient descent
    { 
    DW_temp*=m_LearningRate2;
    DB*=m_LearningRate3;

    inputweightset->SetDWValues(DW_temp.data_block());
    inputweightset->SetDBValues(DB.data_block());
    }
}

template<class LayerType, class TTargetVector>
void
RBFBackPropagationLearningFunction<LayerType,TTargetVector>
::Learn(LayerType* itkNotUsed(layer), TTargetVector itkNotUsed(errors), ValueType itkNotUsed(lr))
{
}

/** Print the object */
template<class LayerType, class TTargetVector>
void  
RBFBackPropagationLearningFunction<LayerType,TTargetVector>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "RBFBackPropagationLearningFunction(" << this << ")" << std::endl; 
  os << indent << "m_LearningRate1 = " << m_LearningRate1 << std::endl;
  os << indent << "m_LearningRate2 = " << m_LearningRate2 << std::endl;
  os << indent << "m_LearningRate3 = " << m_LearningRate3 << std::endl;
  os << indent << "m_OutputErrors = " << m_OutputErrors << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 

} // end namespace Statistics
} // end namespace itk

#endif
