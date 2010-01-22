/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRBFNetwork.txx,v $
  Language:  C++
  Date:      $Date: 2007-08-17 13:10:57 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRBFNetwork_txx
#define __itkRBFNetwork_txx

#include "itkRBFNetwork.h"

namespace itk
{
namespace Statistics
{

/** Constructor */
template<class TMeasurementVector, class TTargetVector>
RBFNetwork<TMeasurementVector,TTargetVector>
::RBFNetwork()
{
  typedef IdentityTransferFunction<ValueType> tfType1;
  m_InputTransferFunction=tfType1::New();

  typedef GaussianRadialBasisFunction<ValueType> tfType2;
  m_FirstHiddenTransferFunction = tfType2::New();

  typedef IdentityTransferFunction<ValueType> tfType3;
  m_OutputTransferFunction= tfType3::New();

  typedef SumInputFunction<ValueType*, ValueType> InputFcnType;
  m_InputFunction=InputFcnType::New();

  m_FirstHiddenLayerBias = 1.0;
  m_OutputLayerBias = 1.0;
  m_NumOfInputNodes = 0;
  m_NumOfFirstHiddenNodes = 0;

  m_NumOfOutputNodes = 0;
  m_Classes = 0;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::InitializeWeights()
{
  Superclass::InitializeWeights();
  vnl_matrix<ValueType> rbf_weights(m_NumOfFirstHiddenNodes,m_NumOfInputNodes+1);
  rbf_weights.fill(0.0);
  this->m_Weights[0]->SetWeightValues(rbf_weights.data_block());

  std::cout << "Setting rbf weights to zero" << std::endl;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::Initialize()
{
  if(m_NumOfInputNodes == 0 )
    {
    itkExceptionMacro("ERROR:  Number of Input Nodes must be greater than 0!");
    }
  if(m_NumOfFirstHiddenNodes == 0 )
    {
    itkExceptionMacro("ERROR:  Number of Hidden Layer 1 Nodes must be greater than 0!");
    }
  if(m_NumOfOutputNodes == 0 )
    {
    itkExceptionMacro("ERROR:  Number of Output Nodes must be greater than 0!");
    }

  //Define weights of Nodes
  typename LearningLayerType::WeightSetType::Pointer InputLayerOutputWeights = LearningLayerType::WeightSetType::New();
  InputLayerOutputWeights->SetNumberOfInputNodes(m_NumOfInputNodes);
  InputLayerOutputWeights->SetNumberOfOutputNodes(m_NumOfFirstHiddenNodes);
  InputLayerOutputWeights->SetCompleteConnectivity();
  InputLayerOutputWeights->SetBias(m_FirstHiddenLayerBias);
  InputLayerOutputWeights->SetRange(1.0);  //0.5
  InputLayerOutputWeights->Initialize();

  typename HiddenLayerType::WeightSetType::Pointer HiddenLayer1OutputWeights =  HiddenLayerType::WeightSetType::New();
  HiddenLayer1OutputWeights->SetNumberOfInputNodes(m_NumOfFirstHiddenNodes);
  HiddenLayer1OutputWeights->SetNumberOfOutputNodes(m_NumOfOutputNodes);
  HiddenLayer1OutputWeights->SetCompleteConnectivity();
  HiddenLayer1OutputWeights->SetBias(m_OutputLayerBias);
  HiddenLayer1OutputWeights->SetRange(1.0); //0.5
  HiddenLayer1OutputWeights->Initialize();

  //Define layers
  typename LearningLayerType::Pointer inputlayer = LearningLayerType::New();
  inputlayer->SetLayerTypeCode(LearningLayerType::INPUTLAYER);
  inputlayer->SetNumberOfNodes(m_NumOfInputNodes);
  inputlayer->SetTransferFunction(m_InputTransferFunction);
  inputlayer->SetNodeInputFunction(m_InputFunction);

  typename HiddenLayerType::Pointer hiddenlayer1 = HiddenLayerType::New();
  hiddenlayer1->SetLayerTypeCode(HiddenLayerType::HIDDENLAYER);
  hiddenlayer1->SetNumberOfNodes(m_NumOfFirstHiddenNodes);
  hiddenlayer1->SetRBF(m_FirstHiddenTransferFunction);
  hiddenlayer1->SetNodeInputFunction(m_InputFunction);
  hiddenlayer1->SetRBF_Dim(m_NumOfInputNodes);
  hiddenlayer1->SetNumClasses(m_Classes);

  typename LearningLayerType::Pointer outputlayer = LearningLayerType::New();
  outputlayer->SetLayerTypeCode(LearningLayerType::OUTPUTLAYER);
  outputlayer->SetNumberOfNodes(m_NumOfOutputNodes);
  outputlayer->SetTransferFunction(m_OutputTransferFunction);
  outputlayer->SetNodeInputFunction(m_InputFunction);

  Superclass::AddLayer(inputlayer);
  Superclass::AddLayer(hiddenlayer1);
  Superclass::AddLayer(outputlayer);

  Superclass::AddWeightSet(InputLayerOutputWeights);
  Superclass::AddWeightSet(HiddenLayer1OutputWeights);

  //HACK:  NOTE:  You can not set the WeightSets until after the layers are added to the network because
  //       the LayerId's must have been set prior to the Weights being added to the layers.
  //       The ordering of putting together the networks is crucial.  Layers must be added to network
  //       prior to weights being added to layers.
  inputlayer->SetOutputWeightSet(InputLayerOutputWeights);
  hiddenlayer1->SetInputWeightSet(InputLayerOutputWeights);
  hiddenlayer1->SetOutputWeightSet(HiddenLayer1OutputWeights);
  outputlayer->SetInputWeightSet(HiddenLayer1OutputWeights);

  /*
  TMeasurementVector temp1;
  TMeasurementVector temp2;
  temp1[0]=110;
  temp1[1]=250;
  temp1[2]=50;
  hiddenlayer1->SetCenter(temp1,0);

  temp2[0]=99;
  temp2[1]=199;
  temp2[2]=300;

  hiddenlayer1->SetCenter(temp2,1);
  DistanceMetric=DistanceMetricType::New();
  double width = DistanceMetric->Evaluate(temp1,temp2);

  hiddenlayer1->SetRadii(2*width,0);
  hiddenlayer1->SetRadii(2*width,1);
   */
  /*  A better test should be written to ensure that bounds checking is done at initializaiton.
  if (m_Centers.size() != m_Radii.size()
    ||  m_Centers.size() != m_NumOfInputNodes)
    {
    itkExceptionMacro("ERROR:  Centers and Radii size must equal number of input nodes");
    }
    */
  for(unsigned int j=0; j<m_Centers.size(); j++)
    {
    hiddenlayer1->SetCenter(m_Centers[j],j);
    hiddenlayer1->SetRadii(m_Radii[j],j);
    }
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetInputTransferFunction(TransferFunctionInterfaceType* f)
{
  m_InputTransferFunction=f;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetDistanceMetric(DistanceMetricType* f)
{
  m_DistanceMetric=f;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetFirstHiddenTransferFunction(TransferFunctionInterfaceType* f)
{
  m_FirstHiddenTransferFunction=f;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetOutputTransferFunction(TransferFunctionInterfaceType* f)
{
  m_OutputTransferFunction=f;
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetInputFunction(InputFunctionInterfaceType* f)
{
  m_InputFunction=f;
}

template<class TMeasurementVector, class TTargetVector>
typename RBFNetwork<TMeasurementVector, TTargetVector>::NetworkOutputType
RBFNetwork<TMeasurementVector,TTargetVector>
::GenerateOutput(TMeasurementVector samplevector)
{
  return Superclass::GenerateOutput(samplevector);
}

template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::SetCenter(TMeasurementVector c)
{
  m_Centers.push_back(c);
}

template<class TMeasurementVector, class TTargetVector>
void
  RBFNetwork<TMeasurementVector,TTargetVector>
::SetRadius(ValueType r)
{
  m_Radii.push_back(r);
}

/** Print the object */
template<class TMeasurementVector, class TTargetVector>
void
RBFNetwork<TMeasurementVector,TTargetVector>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  os << indent << "IdentityTransferFunction(" << this << ")" << std::endl;
  os << indent << "m_NumOfInputNodes = " << m_NumOfInputNodes << std::endl;
  os << indent << "m_NumOfFirstHiddenNodes = " << m_NumOfFirstHiddenNodes << std::endl;
  os << indent << "m_NumOfOutputNodes = " << m_NumOfOutputNodes << std::endl;
  os << indent << "m_Classes = " << m_Classes << std::endl;
  os << indent << "m_FirstHiddenLayerBias = " << m_FirstHiddenLayerBias << std::endl;
  os << indent << "m_OutputLayerBias = " << m_OutputLayerBias << std::endl;
  Superclass::PrintSelf( os, indent );
}

} // end namespace Statistics
} // end namespace itk


#endif
