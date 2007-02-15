/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOneHiddenLayerBackPropagationNeuralNetwork.h,v $
  Language:  C++
  Date:      $Date: 2006/06/28 18:05:24 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkOneHiddenLayerBackPropNeuralNetworkBase_h
#define __itkOneHiddenLayerBackPropNeuralNetworkBase_h


#include "itkMultilayerNeuralNetworkBase.h"
#include "itkBackPropagationLayer.h"
#include "itkCompletelyConnectedWeightSet.h"
#include "itkSigmoidTransferFunction.h"
#include "itkLogSigmoidTransferFunction.h"
#include "itkSymmetricSigmoidTransferFunction.h"
#include "itkTanSigmoidTransferFunction.h"
#include "itkHardLimitTransferFunction.h"
#include "itkSignedHardLimitTransferFunction.h"
#include "itkGaussianTransferFunction.h"
#include "itkTanHTransferFunction.h"
#include "itkIdentityTransferFunction.h"
#include "itkSumInputFunction.h"
#include "itkProductInputFunction.h"

namespace itk
{
namespace Statistics
{

template<class TVector, class TOutput>
class OneHiddenLayerBackPropagationNeuralNetwork : public MultilayerNeuralNetworkBase<TVector, TOutput>
{

public:
  
  typedef OneHiddenLayerBackPropagationNeuralNetwork Self;
  typedef MultilayerNeuralNetworkBase<TVector, TOutput> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::NetworkOutputType NetworkOutputType;
  
  typedef TransferFunctionBase<ValueType> TransferFunctionType;
  typedef InputFunctionBase<ValueType*, ValueType> InputFunctionType;
  
  /* Method for creation through the object factory. */
  itkTypeMacro(OneHiddenLayerBackPropagationNeuralNetwork,
               MultilayerNeuralNetworkBase);  
  itkNewMacro(Self) ;

  //Add the layers to the network.
  // 1 input, 1 hidden, 1 output 
  void Initialize();

  itkSetMacro(NumOfInputNodes, unsigned int);
  itkGetConstReferenceMacro(NumOfInputNodes, unsigned int);

  itkSetMacro(NumOfHiddenNodes, unsigned int);
  itkGetConstReferenceMacro(NumOfHiddenNodes, unsigned int);

  itkSetMacro(NumOfOutputNodes, unsigned int);
  itkGetConstReferenceMacro(NumOfOutputNodes, unsigned int);

  itkSetMacro(HiddenLayerBias, ValueType);
  itkGetConstReferenceMacro(HiddenLayerBias, ValueType);

  itkSetMacro(OutputLayerBias, ValueType);
  itkGetConstReferenceMacro(OutputLayerBias, ValueType);

  //ValueType* GenerateOutput(TVector samplevector);
  NetworkOutputType GenerateOutput(TVector samplevector);

  void SetInputTransferFunction(TransferFunctionType* f);
  void SetHiddenTransferFunction(TransferFunctionType* f);
  void SetOutputTransferFunction(TransferFunctionType* f);

  void SetInputFunction(InputFunctionType* f);

protected:

  OneHiddenLayerBackPropagationNeuralNetwork();
  ~OneHiddenLayerBackPropagationNeuralNetwork(){};

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  unsigned int       m_NumOfInputNodes;
  unsigned int       m_NumOfHiddenNodes;
  unsigned int       m_NumOfOutputNodes;
  ValueType m_HiddenLayerBias;
  ValueType m_OutputLayerBias;

  typename InputFunctionType::Pointer    m_InputFunction;
  typename TransferFunctionType::Pointer m_InputTransferFunction;
  typename TransferFunctionType::Pointer m_HiddenTransferFunction;
  typename TransferFunctionType::Pointer m_OutputTransferFunction;
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkOneHiddenLayerBackPropagationNeuralNetwork.txx"
#endif

#endif
