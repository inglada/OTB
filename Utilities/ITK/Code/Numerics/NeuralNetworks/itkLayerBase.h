/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLayerBase.h,v $
  Language:  C++
  Date:      $Date: 2007/08/17 13:10:57 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLayerBase_h
#define __itkLayerBase_h

#include <iostream>
#include "itkLightProcessObject.h"
#include "itkArray.h"
#include "itkVector.h"
#include "itkTransferFunctionBase.h"
#include "itkInputFunctionBase.h"

#include "itkWeightSetBase.h"
#include "itkMacro.h"

namespace itk
{
namespace Statistics
{

  template<class TMeasurementVector, class TTargetVector>
class LayerBase : public LightProcessObject
{

public:
  typedef LayerBase Self;
  typedef LightProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkTypeMacro(LayerBase, LightProcessObject);

  typedef TMeasurementVector InputVectorType;
  typedef TTargetVector OutputVectorType;

  typedef typename TMeasurementVector::ValueType ValueType;
  typedef ValueType* ValuePointer;
  typedef const ValueType* ValueConstPointer;
  typedef vnl_vector<ValueType> NodeVectorType;
  typedef Array<ValueType> InternalVectorType;

  typedef LayerBase  LayerInterfaceType;
  typedef WeightSetBase<TMeasurementVector,TTargetVector> WeightSetType;
  typedef WeightSetBase<TMeasurementVector,TTargetVector> WeightSetInterfaceType;
  typedef InputFunctionBase<ValueType*, ValueType> InputFunctionInterfaceType;
  typedef TransferFunctionBase<ValueType>  TransferFunctionInterfaceType;

  //The only valid layer types
  typedef enum {  INVALIDLAYER=0, INPUTLAYER=1, HIDDENLAYER=2, OUTPUTLAYER=3 } LayerTypeCode;

  virtual void SetNumberOfNodes(unsigned int);
  unsigned int GetNumberOfNodes() const;

  virtual ValueType GetInputValue(unsigned int) const = 0;
  virtual ValueType GetOutputValue(unsigned int) const = 0;
  virtual ValuePointer GetOutputVector() = 0;

  virtual void ForwardPropagate(){};
  virtual void ForwardPropagate(TMeasurementVector){};

  virtual void BackwardPropagate(){};
  virtual void BackwardPropagate(InternalVectorType){};

  virtual ValueType GetOutputErrorValue(unsigned int) const = 0;
  virtual void SetOutputErrorValues(TTargetVector) {};

  virtual ValueType GetInputErrorValue(unsigned int) const = 0;
  virtual ValuePointer GetInputErrorVector() = 0;
  virtual void SetInputErrorValue(ValueType, unsigned int) {};

  //itkSetObjectMacro(InputWeightSet, WeightSetInterfaceType);
  void SetInputWeightSet(WeightSetInterfaceType*);
  itkGetObjectMacro(InputWeightSet, WeightSetInterfaceType);
  itkGetConstObjectMacro(InputWeightSet, WeightSetInterfaceType);

  //itkSetObjectMacro(OutputWeightSet, WeightSetInterfaceType);
  void SetOutputWeightSet(WeightSetInterfaceType*);
  itkGetObjectMacro(OutputWeightSet, WeightSetInterfaceType);
  itkGetConstObjectMacro(OutputWeightSet, WeightSetInterfaceType);

  void SetNodeInputFunction(InputFunctionInterfaceType* f);
  itkGetObjectMacro(NodeInputFunction, InputFunctionInterfaceType);
  itkGetConstObjectMacro(NodeInputFunction, InputFunctionInterfaceType);

  void SetTransferFunction(TransferFunctionInterfaceType* f);
  itkGetObjectMacro(ActivationFunction, TransferFunctionInterfaceType);
  itkGetConstObjectMacro(ActivationFunction, TransferFunctionInterfaceType);

  virtual ValueType Activation(ValueType) = 0;
  virtual ValueType DActivation(ValueType) = 0;

  itkSetEnumMacro(LayerTypeCode, LayerTypeCode);
  itkGetEnumMacro(LayerTypeCode, LayerTypeCode);

//#define __USE_OLD_INTERFACE  Comment out to ensure that new interface works
#ifdef __USE_OLD_INTERFACE
  void SetLayerType(const LayerTypeCode value) { SetLayerTypeCode(value); }
  LayerTypeCode GetLayerType(void) { return GetLayerTypeCode(); }
  //For backwards compatibility
  void SetLayerType(const unsigned int value)
    {
    switch(value)
      {
    case 0:
      SetLayerType(INVALIDLAYER);
      break;
    case 1:
      SetLayerType(INPUTLAYER);
      break;
    case 2:
      SetLayerType(HIDDENLAYER);
      break;
    case 3:
      SetLayerType(OUTPUTLAYER);
      break;
    default:
      //Throw Exception Here
      break;
      }
    }
#endif
  itkSetMacro(LayerId,unsigned int);
  itkGetConstReferenceMacro(LayerId,unsigned int);

  //virtual void SetBias(const ValueType) = 0;
  //virtual const ValueType & GetBias() const = 0;

protected:
  LayerBase();
  ~LayerBase();

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  LayerTypeCode m_LayerTypeCode; //input, hidden, output
  unsigned int m_LayerId;
  unsigned int m_NumberOfNodes;

  typename WeightSetInterfaceType::Pointer m_InputWeightSet;
  typename WeightSetInterfaceType::Pointer m_OutputWeightSet;

  typename TransferFunctionInterfaceType::Pointer m_ActivationFunction;
  typename InputFunctionInterfaceType::Pointer    m_NodeInputFunction;

}; //class layer base

} //namespace itk
} //namespace statistics

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLayerBase.txx"
#endif

#endif
