/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRBFLayer.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/10 19:53:43 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkRBFLayerBase_txx
#define __itkRBFLayerBase_txx

#include "itkRBFLayer.h"

namespace itk
{
namespace Statistics
{

template<class TVector, class TOutput>
RBFLayer<TVector,TOutput>
::RBFLayer()
{
  m_Bias = 1;
  m_NumClasses = 0;
  typedef GaussianRadialBasisFunction<ValueType> GRBFType;
  m_RBF=GRBFType::New();
  m_DistanceMetric = DistanceMetricType::New();
  TVector origin;
  
  m_DistanceMetric->SetMeasurementVectorSize(origin.GetVectorDimension());
  m_RBF_Dim = 0;
}

template<class TVector, class TOutput>
RBFLayer<TVector,TOutput>
::~RBFLayer()
{
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetRBF(RBFType* f)
{
  m_RBF = f;
  this->Modified();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetNumberOfNodes(unsigned int c)
{
  TVector sampleinputvector;
  m_RBF_Dim= sampleinputvector.GetVectorDimension();
  Superclass::SetNumberOfNodes(c);
  this->m_NodeInputValues.set_size(m_RBF_Dim); //c);
  this->m_NodeOutputValues.set_size(c);
  m_InputErrorValues.set_size(c);
  m_OutputErrorValues.set_size(c);
 
  if(this->GetLayerType()!=3)
    {
    TVector temp;
    for(unsigned int i=0; i<c; i++)
      {
      m_Centers.push_back(temp);
      }
    this->m_NodeOutputValues.set_size(c);
    m_Radii.SetSize(c);
    m_Radii.fill(1.0);
    }
  this->Modified();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetInputValue(unsigned int i, ValueType value)
{
  this->m_NodeInputValues[i] = value;
  this->Modified();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetInputValue(unsigned int i)
{
  return m_NodeInputValues[i];
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetOutputValue(int i, ValueType value)
{
  m_NodeOutputValues(i) = value;
  this->Modified();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetOutputValue(int i)
{
  return m_NodeOutputValues(i);
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetOutputVector(TVector value)
{
  m_NodeOutputValues = value.GetVnlVector();
  this->Modified();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValuePointer
RBFLayer<TVector,TOutput>
::GetOutputVector()
{
  return m_NodeOutputValues.data_block();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetRadii(ValueType c,int i)
{
  m_Radii.SetElement(i,c);
  this->Modified();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetRadii(int i)
{
  return m_Radii.GetElement(i);
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetCenter(TVector c,int i)
{
  m_Centers[i]=c;
  this->Modified();
}

template<class TVector, class TOutput>
TVector
RBFLayer<TVector,TOutput>
::GetCenter(int i)
{
  if(m_Centers.size()!=0)
    {
    return m_Centers[i];
    }
  else
    {
    return 0;
    }
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetInputErrorValue(int n)
{
  return m_InputErrorValues[n];
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValuePointer
RBFLayer<TVector,TOutput>
::GetInputErrorVector()
{
  return m_InputErrorValues.data_block();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetInputErrorValue(ValueType v, int i)
{
  m_InputErrorValues[i] = v;
  this->Modified();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::ForwardPropagate()
{
  if(this->GetLayerType()==3)
    {
    typename Superclass::WeightSetType::Pointer inputweightset;
    typename Superclass::InputFunctionType::Pointer inputfunction;
    typename Superclass::TransferFunctionType::Pointer transferfunction;

    inputfunction = this->GetNodeInputFunction();
    transferfunction = this->GetActivationFunction();
    inputweightset = this->GetInputWeightSet();
    ValuePointer inputvalues = inputweightset->GetOutputValues();

    int rows = this->m_NumberOfNodes;
    int cols = this->m_InputWeightSet->GetNumberOfInputNodes();
    vnl_matrix<ValueType> inputmatrix;
    inputmatrix.set_size(rows, cols);
    inputmatrix.copy_in(inputvalues);
  
    inputfunction->SetSize(cols); //include bias
    for (int j = 0; j < rows; j++)
      {
      vnl_vector<ValueType> temp_vnl;
      temp_vnl.set_size(inputmatrix.cols());
      temp_vnl=inputmatrix.get_row(j);
      m_NodeInputValues.put(j, inputfunction->Evaluate(temp_vnl.data_block()));
      m_NodeOutputValues.put(j, transferfunction->Evaluate(m_NodeInputValues[j]));
      }
    }
  else
    {
    typename Superclass::WeightSetType::Pointer inputweightset;
    typename Superclass::InputFunctionType::Pointer inputfunction;
 
    inputweightset = this->GetInputWeightSet();
 
    inputfunction = this->GetNodeInputFunction();
  
    vnl_vector<ValueType> temp;
    ValuePointer inputvalues = inputweightset->GetInputValues();

    int cols = this->m_InputWeightSet->GetNumberOfInputNodes();
    vnl_matrix<ValueType> inputmatrix;
    inputmatrix.set_size(1, cols-1);
    inputmatrix.copy_in(inputvalues);
    inputfunction->SetSize(cols-1); //include bias
    m_NodeInputValues = inputmatrix.get_row(0);
    ValuePointer cdeltavalues = inputweightset->GetTotalDeltaValues();
    vnl_matrix<ValueType> center_increment(cdeltavalues,inputweightset->GetNumberOfOutputNodes(),
                                             inputweightset->GetNumberOfInputNodes());
    vnl_vector<ValueType> width_increment;
    width_increment.set_size(inputweightset->GetNumberOfOutputNodes());
    width_increment.fill(0);
    width_increment= center_increment.get_column(inputweightset->GetNumberOfInputNodes()-1);
    ValueType temp_radius;
    NodeArrayType temp_center;
    temp_center.SetSize(m_RBF_Dim);
    TVector tempvector1;
    TVector tempvector2;
    TVector tempcenter;
 
    for (int i = 0; i < m_NumClasses; i++)
      {
      tempcenter = m_Centers[i];
      for(int j=0;j<m_RBF_Dim;j++)
        {
        ValueType val =tempcenter[j];
        val += center_increment[i][j];
        tempcenter[j]=val;
        }
        
      m_Centers[i]=tempcenter;   
      temp_radius = m_Radii.GetElement(i);
      temp_radius += width_increment[i];
      m_Radii.SetElement(i,temp_radius);
      NodeArrayType array1(m_NodeInputValues.size());
   
      array1= m_NodeInputValues;
 
      tempvector1.Set_vnl_vector(m_NodeInputValues);
      tempvector2=m_Centers[i];
      tempcenter= m_Centers[i];
      //double dt= m_DistanceMetric->Evaluate(tempvector1,tempvector2);
      //std::cout<<"Euclidean in layer ="<<dt<<std::endl;
      m_RBF->SetRadius(m_Radii.GetElement(i));
      NodeArrayType temp_array(m_RBF_Dim);
      TVector temp_vector=  m_Centers[i];
      for(int ii=0; ii<m_RBF_Dim; ii++)
         temp_array.SetElement(ii,temp_vector[ii]);
      m_RBF->SetCenter(temp_array);
      m_NodeOutputValues.put(i,m_RBF->Evaluate(m_DistanceMetric->Evaluate(tempvector1,tempvector2)));
      }
    }
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetDistanceMetric(DistanceMetricType* f)
{ 
  m_DistanceMetric=f;
  this->Modified();
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::ForwardPropagate(TVector samplevector)
{ 
  typename Superclass::TransferFunctionType::Pointer transferfunction;
  transferfunction = this->GetActivationFunction();

  for (unsigned int i = 0; i < samplevector.Size(); i++)
    {
    samplevector[i] = transferfunction->Evaluate(samplevector[i]);      
    m_NodeOutputValues.put(i, samplevector[i]);
    }
}


template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetOutputErrorValues(TOutput errors)
{
  m_OutputErrorValues = errors.GetVnlVector();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetOutputErrorValue(unsigned int i)
{
  return m_OutputErrorValues[i];
}


template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::BackwardPropagate()
{
  int num_nodes = this->GetNumberOfNodes();

  typename Superclass::WeightSetType::Pointer outputweightset;
  typename Superclass::WeightSetType::Pointer inputweightset;
  outputweightset = Superclass::GetOutputWeightSet();
  inputweightset = Superclass::GetInputWeightSet();

  vnl_vector<ValueType> OutputLayerInput(outputweightset->GetInputValues(),num_nodes);
  

  ValuePointer deltavalues = outputweightset->GetDeltaValues();
  ValuePointer weightvalues = outputweightset->GetWeightValues();

  int cols = num_nodes;
  int rows = outputweightset->GetNumberOfOutputNodes();
  
  vnl_matrix<ValueType> weightmatrix(weightvalues, rows, cols);
 
  vnl_matrix<ValueType> deltamatrix(deltavalues, rows, cols);
  vnl_vector<ValueType> deltaww;
  deltaww.set_size(cols);
  deltaww.fill(0);
  TVector tempvector1;
  TVector tempvector2;
 
  for(int c1=0; c1<rows; c1++)
    {
    for(int c2=0; c2<cols; c2++)
      {
      deltamatrix[c1][c2]=deltamatrix[c1][c2]/OutputLayerInput[c2];
      }
    }
  for (int i = 0; i < cols; i++)
    {
    deltaww[i] = dot_product(deltamatrix.get_column(i),
                               weightmatrix.get_column(i));
    }
  
  //compute gradient for centers 
  NodeArrayType array1(m_NodeInputValues.size());
  array1= m_NodeInputValues;
  vnl_matrix<ValueType> DW_temp(inputweightset->GetNumberOfOutputNodes(),
                                           inputweightset->GetNumberOfInputNodes());
  DW_temp.fill(0.0);
  tempvector1.Set_vnl_vector(array1);
    
  for(int k=0; k<num_nodes; k++)
    {  
    for (int i = 0; i < m_RBF_Dim; i++)
      {
      tempvector2=m_Centers[k];
      double dist=m_DistanceMetric->Evaluate(tempvector1,tempvector2); 
      m_RBF->SetRadius(m_Radii.GetElement(k));
      TVector temp_vector=  m_Centers[k];
      NodeArrayType temp_array(m_RBF_Dim);
      for(int ii=0; ii<m_RBF_Dim; ii++)
         temp_array.SetElement(ii,temp_vector[ii]);      
      m_RBF->SetCenter(temp_array);
   
      DW_temp[k][i]=deltaww[k] * m_RBF->EvaluateDerivative
                                          (dist,array1,'u',i);                                     
      }
    }

  //compute gradient for widths
  NodeVectorType width_gradient;
  width_gradient.set_size(num_nodes);
  width_gradient.fill(0.0);
  
  for (int i=0;i<num_nodes;i++)
    {
    tempvector2=m_Centers[i];
    double dist=m_DistanceMetric->Evaluate(tempvector1,tempvector2);
    width_gradient[i]=deltaww[i] * m_RBF->EvaluateDerivative
                                        (dist,array1,'s');
    }
  inputweightset->SetDeltaValues(DW_temp.data_block());
  inputweightset->SetDeltaBValues(width_gradient.data_block());
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::Activation(ValueType n)
{
  return this->m_ActivationFunction->Evaluate(n);
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::DActivation(ValueType n)
{
  return this->m_ActivationFunction->EvaluateDerivative(n);
}

template<class TVector, class TOutput>
void
RBFLayer<TVector,TOutput>
::SetBias(ValueType b)
{
  m_Bias = b;
  this->Modified();
}

template<class TVector, class TOutput>
typename RBFLayer<TVector,TOutput>::ValueType
RBFLayer<TVector,TOutput>
::GetBias()
{
  return m_Bias;
}


/** Print the object */
template<class TVector, class TOutput>
void  
RBFLayer<TVector,TOutput>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "RBFLayer(" << this << ")" << std::endl;
  os << indent << "m_DistanceMetric = " << m_DistanceMetric << std::endl;
  os << indent << "m_NodeInputValues = " << m_NodeInputValues << std::endl;
  os << indent << "m_NodeOutputValues = " << m_NodeOutputValues << std::endl;
  os << indent << "m_InputErrorValues = " << m_InputErrorValues << std::endl;
  os << indent << "m_OutputErrorValues = " << m_OutputErrorValues << std::endl;
  //os << indent << "m_Centers = " << m_Centers << std::endl;
  os << indent << "m_Radii = " << m_Radii << std::endl;
  os << indent << "m_Bias = " << m_Bias << std::endl;
  os << indent << "m_RBF_Dim = " << m_RBF_Dim << std::endl;
  os << indent << "m_RBF = " << m_RBF << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 

} // end namespace Statistics
} // end namespace itk

#endif
