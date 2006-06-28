/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRBFLayer.h,v $
  Language:  C++
  Date:      $Date: 2006/04/17 21:34:31 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRBFLayerBase_h
#define __itkRBFLayerBase_h

#include "itkLayerBase.h"
#include "itkObject.h"
#include "itkMacro.h"
#include "itkRadialBasisFunctionBase.h"
#include "itkEuclideanDistance.h"

namespace itk
{
namespace Statistics
{

template<class TVector, class TOutput>
class RBFLayer : public LayerBase<TVector, TOutput>
{
public:       
  
  typedef RBFLayer Self;
  typedef LayerBase<TVector, TOutput> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkTypeMacro(RBFLayer, LayerBase);  
  itkNewMacro(Self) ;

  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::ValuePointer ValuePointer;
  typedef vnl_vector<ValueType> NodeVectorType;
  //typedef Array<ValueType> NodeArrayType;
  typedef typename Superclass::InternalVectorType InternalVectorType;

  typedef typename Superclass::OutputVectorType OutputVectorType;
  
  typedef RadialBasisFunctionBase<ValueType> RBFType;

  //Distance Metric
  typedef EuclideanDistance<InternalVectorType> DistanceMetricType; 
  typedef typename DistanceMetricType::Pointer DistanceMetricPointer;
  //Member Functions
  void SetNumberOfNodes(unsigned int numNodes);
  //void SetMeasurementVectorSize(unsigned int size);
  itkGetMacro(RBF_Dim, unsigned int);
  void SetRBF_Dim(unsigned int size);
 

  ValueType GetInputValue(unsigned int i);
  void SetInputValue(unsigned int i,ValueType value);

  itkGetMacro(LayerType, unsigned int);

  ValueType GetOutputValue(int);
  void SetOutputValue(int, ValueType);

  ValuePointer GetOutputVector();
  void SetOutputVector(TVector value);

  void ForwardPropagate();
  void ForwardPropagate(TVector input);

  void BackwardPropagate();
  void BackwardPropagate(TOutput itkNotUsed(errors)){};

  void SetOutputErrorValues(TOutput);
  ValueType GetOutputErrorValue(unsigned int node_id);

  
  ValueType GetInputErrorValue(int node_id);
  ValuePointer GetInputErrorVector();
  void SetInputErrorValue(ValueType, int node_id);

  //TVector GetCenter(int i);
  InternalVectorType GetCenter(int i);
  void SetCenter(TVector c,int i);

  ValueType GetRadii(int i);
  void SetRadii(ValueType c,int i);


  ValueType Activation(ValueType);
  ValueType DActivation(ValueType);

  void SetBias(ValueType b);

  ValueType GetBias();

  void SetDistanceMetric(DistanceMetricType* f);
  DistanceMetricPointer GetDistanceMetric(){return m_DistanceMetric;}

  itkSetMacro(NumClasses, int);
  itkGetConstReferenceMacro(NumClasses,int);

  void SetRBF(RBFType* f);
  itkGetObjectMacro(RBF, RBFType);

protected:                

   RBFLayer();
  ~RBFLayer();
  
  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:
  
  typename DistanceMetricType::Pointer  m_DistanceMetric;
  NodeVectorType                        m_NodeInputValues;
  NodeVectorType                        m_NodeOutputValues;
  NodeVectorType                        m_InputErrorValues;
  NodeVectorType                        m_OutputErrorValues;
  //std::vector<TVector>                  m_Centers;  // ui....uc
  std::vector<InternalVectorType>       m_Centers;  // ui....uc
  InternalVectorType                    m_Radii;
  int                                   m_NumClasses;
  ValueType                             m_Bias;
  int                                   m_RBF_Dim;
  typename RBFType::Pointer             m_RBF;
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkRBFLayer.txx"
#endif

#endif
