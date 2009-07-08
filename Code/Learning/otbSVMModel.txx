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
#ifndef __otbSVMModel_txx
#define __otbSVMModel_txx
#include "otbSVMModel.h"
#include "otbMacro.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

namespace otb
{
// TODO: Check memory allocation in this class
template <class TValue,class TLabel> 
SVMModel<TValue,TLabel>::SVMModel()
{
  // Initialize model
  m_Model = new struct svm_model;
  m_Model->param.kernel_generic = NULL;
  m_Model->param.kernel_composed = NULL;
  m_Model->param.nr_weight = 0;
  m_Model->param.weight_label = NULL;
  m_Model->param.weight = NULL;
  m_Model->delete_composed = false;
  m_Model->l = 0;
  m_Model->nr_class = 0;
  m_Model->SV = NULL;
  m_Model->sv_coef = NULL;
  m_Model->rho = NULL;
  m_Model->label = NULL;
  m_Model->probA = NULL;
  m_Model->probB = NULL;
  m_Model->nSV = NULL;


  m_ModelUpToDate = false;

  // Default parameters
  this->SetSVMType(C_SVC);
  this->SetKernelType(LINEAR);
  this->SetPolynomialKernelDegree(3);
  this->SetKernelGamma(1.);  // 1/k
  this->SetKernelCoef0(1.);
  this->SetKernelFunctor(NULL);
  this->SetNu(0.5);
  this->SetCacheSize(40);
  this->SetC(1);
  this->SetEpsilon(1e-3);
  this->SetP(0.1);
  this->DoShrinking(1);
  this->DoProbabilityEstimates(true);

  // Intialize problem
  m_Problem.l = 0;
  m_Problem.y = NULL;
  m_Problem.x = NULL;

  m_ProblemUpToDate = false;
}

template <class TValue,class TLabel> 
SVMModel<TValue,TLabel>::~SVMModel()
{
  this->DeleteModel();
  this->DeleteProblem();
}

template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::DeleteModel()
{ 
  // Here we do not use svm_destroy_model because it delete memory
  // without checking if actually allocated
if(m_Model->free_sv && m_Model->l > 0)
  {
  if(m_Model->SV[0])
    {
    delete [] (m_Model->SV[0]);
    }
  }
  for(int i=0;i<m_Model->nr_class-1;i++)
    {
    delete [](m_Model->sv_coef[i]);
    }
  if ( m_Model->delete_composed == true)
    {
    delete m_Model->param.kernel_composed;
    }
  if(m_Model->SV)
    {
    delete [](m_Model->SV);
    }
  if(m_Model->sv_coef)
    {
    delete [](m_Model->sv_coef);
    }
  if(m_Model->rho)
    {
    delete [](m_Model->rho);
    }
  if(m_Model->label)
    {
    delete [](m_Model->label);
    }
  if(m_Model->probA)
    {
    delete [](m_Model->probA);
    }
  if(m_Model->probB)
    {
    delete [](m_Model->probB);
    }
  if(m_Model->nSV)
    {
    delete [](m_Model->nSV);
    }
  delete m_Model;
}
template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::DeleteProblem()
{
// Deallocate any existing problem
  if(m_Problem.y)
    {
    delete [] m_Problem.y;
    m_Problem.y = NULL;
    }
  
  if(m_Problem.x)
    {
    for(int i = 0; i < m_Problem.l;++i)
      {
      if(m_Problem.x[i])
	{
	delete [] m_Problem.x[i];
	}
      }
    delete [] m_Problem.x;
    m_Problem.x = NULL;
    }
  m_Problem.l = 0;
  m_ProblemUpToDate = false;
}


template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::AddSample(const MeasurementType & measure, const LabelType & label)
{
  SampleType newSample(measure,label);
  m_Samples.push_back(newSample);
  m_ProblemUpToDate = false;
}

template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::ClearSamples()
{
  m_Samples.clear();
  m_ProblemUpToDate = false;
}

template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::SetSamples(const SamplesVectorType & samples)
{
  m_Samples = samples;
  m_ProblemUpToDate = false;
}

template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::BuildProblem()
{
  // Check if problem is up-to-date
  if(m_ProblemUpToDate)
    {
    return;
    }

  // Get number of samples
  int probl = m_Samples.size();

  if(probl < 1)
    {
    itkExceptionMacro(<<"No samples, can not build SVM problem.");
    }
  std::cout<<"Rebuilding problem ..."<<std::endl;

  // Get the size of the samples
  long int elements = m_Samples[0].first.size()+1;


  // Deallocate any previous problem
  this->DeleteProblem();

  // Allocate the problem 
  m_Problem.l = probl;
  m_Problem.y = new double[probl];
  m_Problem.x = new struct svm_node*[probl];

  for(int i = 0; i<probl;++i)
    {
    // Initialize labels to 0
    m_Problem.y[i] = 0;
    m_Problem.x[i] = new struct svm_node[elements];

    // Intialize elements (value = 0; index = -1)
    for(unsigned int j = 0; j<static_cast<unsigned int>(elements);++j)
      {
      m_Problem.x[i][j].index = -1;
      m_Problem.x[i][j].value = 0;
      }
    }

  // Iterate on the samples
  typename SamplesVectorType::const_iterator sIt = m_Samples.begin();
  int sampleIndex = 0;
  int maxElementIndex = 0;

  while(sIt != m_Samples.end())
    {

    // Get the sample measurement and label
    MeasurementType measure = sIt->first;
    LabelType label = sIt->second;
    
    // Set the label
    m_Problem.y[sampleIndex] = label;

     int elementIndex = 0;
     
     // Populate the svm nodes
     for(typename MeasurementType::const_iterator eIt = measure.begin();
	 eIt!=measure.end() && elementIndex < elements ;++eIt,++elementIndex)
       {
       m_Problem.x[sampleIndex][elementIndex].index = elementIndex+1;
       m_Problem.x[sampleIndex][elementIndex].value = (*eIt);
       }
     
     // Get the max index
     if(elementIndex > maxElementIndex)
       {
       maxElementIndex = elementIndex;
       }

      ++sampleIndex;
      ++sIt;
    }
  
  // Compute the kernel gamma from maxElementIndex if necessary
  if (this->GetKernelGamma() == 0 && this->GetParameters().kernel_type != COMPOSED && this->GetParameters().kernel_type != GENERIC)
    this->SetKernelGamma(1.0/static_cast<double>(maxElementIndex));

  // problem is up-to-date
  m_ProblemUpToDate = true;
}

template <class TValue, class TLabel>
double
SVMModel<TValue,TLabel>::CrossValidation(unsigned int nbFolders)
{
  // Build problem
  this->BuildProblem();

  // Check consistency
  this->ConsistencyCheck();

  // Get the length of the problem
  int length = m_Problem.l;
    
    // Temporary memory to store cross validation results
    double *target = new double[length];

    // Do cross validation
    svm_cross_validation(&m_Problem,&m_Model->param,nbFolders,target);

    // Evaluate accuracy
    int i;
    double total_correct = 0.;
    
    for(i=0;i<length;i++)
      { 
      if(target[i] == m_Problem.y[i])
	{
	++total_correct;
	}
      }
    double accuracy = total_correct/length;
    
    // Free temporary memory
    delete [] target;

    // return accuracy value
    return accuracy;
}

template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::ConsistencyCheck()
{
  if (m_Model->nr_class<2)
  {
  itkExceptionMacro(<<"Can not do SVM estimation with less than 2 classes");
  }
  
  if (m_Model->param.svm_type == ONE_CLASS)
    {
    if (m_Model->nr_class>2)
      {
      itkExceptionMacro(<<"Can not do ONE_CLASS SVM estimation with more than 2 classes");
      }
    if (this->GetDoProbabilityEstimates())
      {
      otbMsgDebugMacro(<<"Disabling SVM probability estimates for ONE_CLASS SVM type.");
      this->DoProbabilityEstimates(false);
      }
    }
  const char* error_msg = svm_check_parameter(&m_Problem,&m_Model->param);
  if (error_msg)
  {
    throw itk::ExceptionObject(__FILE__, __LINE__,error_msg,ITK_LOCATION);
  }
}

template <class TValue, class TLabel>
void 
SVMModel<TValue,TLabel>::Train()
{
  // If the model is already up-to-date, return
  if(m_ModelUpToDate)
    {
    return;
    }

  // Build problem
  this->BuildProblem();
  
  // Check consistency
  this->ConsistencyCheck();
  
  // retrieve parameters
  struct svm_parameter parameters = m_Model->param;
  
  // train the model
  m_Model = svm_train(&m_Problem,&parameters);
  
  // Reset the parameters
  m_Model->param = parameters;
    
  // Set the model as up-to-date
  m_ModelUpToDate = true;
}

template <class TValue, class TLabel>
typename SVMModel<TValue,TLabel>::LabelType
SVMModel<TValue,TLabel>::EvaluateLabel(const MeasurementType & measure) const
{
  // Check if model is up-to-date
  if(!m_ModelUpToDate)
    {
    itkExceptionMacro(<<"Model is not up-to-date, can not predict label");
    } 

  // Check probability prediction
  bool predict_probability = 1;

  if (svm_check_probability_model(m_Model)==0)
    {
    if (this->GetSVMType() == ONE_CLASS)
      {
      predict_probability = 0;
      }
    else
      {
      throw itk::ExceptionObject(__FILE__, __LINE__,
                                 "Model does not support probabiliy estimates",ITK_LOCATION);
      }
    }
  
  // Get type and number of classes
  int svm_type=svm_get_svm_type(m_Model);
  int nr_class=svm_get_nr_class(m_Model);
 
  // Allocate space for labels
  double *prob_estimates=NULL;

  // Eventually allocate space for probabilities
  if (predict_probability)
    {
    if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
      {
       otbMsgDevMacro(<<"Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma="<<svm_get_svr_probability(m_Model));
      }
    else
      {
      prob_estimates = new double[nr_class];
      }
    }
  
  // Allocate nodes (/TODO if performances problems are related to too
  // many allocations, a cache approach can be set)
  struct svm_node * x = new struct svm_node[measure.size()+1];
  
  int valueIndex = 0;
  
  // Fill the node
  for(typename MeasurementType::const_iterator mIt = measure.begin();mIt!=measure.end();++mIt,++valueIndex)
    {
    x[valueIndex].index=valueIndex+1;
    x[valueIndex].value=(*mIt);
    }
  
  // terminate node
  x[measure.size()].index = -1;
  x[measure.size()].value = 0;

  LabelType label = 0;

  if(predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
    {
    label = static_cast<LabelType>(svm_predict_probability(m_Model,x,prob_estimates));
    }
  else
    {
    label = static_cast<LabelType>(svm_predict(m_Model,x));
    }

  // Free allocated memory
  delete [] x;
  
  if(prob_estimates)
    {
    delete [] prob_estimates;
    }

  return label;
}

template <class TValue, class TLabel>
typename SVMModel<TValue,TLabel>::DistancesVectorType
SVMModel<TValue,TLabel>::EvaluateHyperplanesDistances(const MeasurementType & measure) const
{
  // Check if model is up-to-date
  if(!m_ModelUpToDate)
    {
    itkExceptionMacro(<<"Model is not up-to-date, can not predict label");
    }

  // Allocate nodes (/TODO if performances problems are related to too
  // many allocations, a cache approach can be set)
    struct svm_node * x  = new struct svm_node[measure.size()+1];

  int valueIndex = 0;
  
  // Fill the node
  for(typename MeasurementType::const_iterator mIt = measure.begin();mIt!=measure.end();++mIt,++valueIndex)
    {
    x[valueIndex].index=valueIndex+1;
    x[valueIndex].value=(*mIt);
    }
  
  // terminate node
  x[measure.size()].index = -1;
  x[measure.size()].value = 0;
  
  // Intialize distances vector
  DistancesVectorType distances(m_Model->nr_class*(m_Model->nr_class-1)/2);
  
  // predict distances vector
  svm_predict_values(m_Model,x, (double*)(distances.GetDataPointer()));
 
  // Free allocated memory
  delete [] x;
  
  return (distances);
}

template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::SetModel(struct svm_model* aModel)
{
  this->DeleteModel();
  m_Model = svm_copy_model(aModel);
  m_ModelUpToDate = true;
}


template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::SaveModel(const char* model_file_name) const
{
  if (svm_save_model(model_file_name, m_Model)!=0)
  {
    itkExceptionMacro( << "Problem while saving SVM model "
                       << std::string(model_file_name) );
  }
}

template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::LoadModel(const char* model_file_name)
{
  this->DeleteModel();
  m_Model = svm_load_model(model_file_name, m_Model->param.kernel_generic);
  if (m_Model == 0)
  {
    itkExceptionMacro( << "Problem while loading SVM model "
                       << std::string(model_file_name) );
  }
  m_ModelUpToDate = true;
}

template <class TValue,class TLabel> 
typename SVMModel<TValue,TLabel>::Pointer 
SVMModel<TValue,TLabel>::GetCopy() const
{
  Pointer modelCopy = New();
  modelCopy->SetModel( m_Model );
  // We do not copy the problem to avoid sharing allocated memory
  return modelCopy;
}

template <class TValue,class TLabel>
void
SVMModel<TValue,TLabel>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::SetSupportVectors(svm_node ** sv, int nbOfSupportVector)
{
  // TODO: rewrite this to check memory allocation


  // erase the old SV
  // delete just the first element, it destoyes the whole pointers (cf SV filling with x_space)
  delete [] (m_Model->SV[0]);

  for (int n = 0; n<m_Model->l; ++n)
  {
    m_Model->SV[n] = NULL;
  }
  delete[] (m_Model->SV);
  m_Model->SV = NULL;

  this->SetNumberOfSupportVectors(nbOfSupportVector);
  m_Model->SV = new struct svm_node*[m_Model->l];

  // copy new SV values
  svm_node **SV = m_Model->SV;

  // Compute the total number of SV elements.
  unsigned int elements = 0;
  for (int p=0; p<nbOfSupportVector; ++p)
  {
    std::cout<<p<<"  ";
    const svm_node *tempNode = sv[p];
    std::cout<<p<<"  ";
    while (tempNode->index != -1)
    {
      tempNode++;
      ++elements;
    }
    ++elements;// for -1 values
  }

  if (m_Model->l>0)
  {
    SV[0] = new struct svm_node[elements];
    memcpy( SV[0],sv[0],sizeof(svm_node*)*elements);
  }
  svm_node *x_space =  SV[0];

  int j = 0;
  for (int i=0; i<m_Model->l; ++i)
  {
    // SV
    SV[i] = &x_space[j];
    const svm_node *p = sv[i];
    svm_node *pCpy = SV[i];
    while (p->index != -1)
    {
      pCpy->index = p->index;
      pCpy->value = p->value;
      ++p;
      ++pCpy;
      ++j;
    }
    pCpy->index = -1;
    ++j;
  }

  if(m_Model->l>0)
    {
    delete [] SV[0];
    }
}

template <class TValue,class TLabel> 
void
SVMModel<TValue,TLabel>::SetAlpha( double ** alpha, int nbOfSupportVector )
{
  // TODO: Check memory allocation

  // Erase the old sv_coef
  for (int i=0; i<m_Model->nr_class-1; ++i)
  {
    delete[] m_Model->sv_coef[i];
  }
  delete [] m_Model->sv_coef;

  this->SetNumberOfSupportVectors(nbOfSupportVector);

  // copy new sv_coef values
  m_Model->sv_coef = new double*[m_Model->nr_class-1];
  for (int i=0; i<m_Model->nr_class-1; ++i)
    m_Model->sv_coef[i] = new double[m_Model->l];

  for (int i=0; i<m_Model->l; ++i)
  {
    // sv_coef
    for (int k=0; k<m_Model->nr_class-1; ++k)
    {
      m_Model->sv_coef[k][i] = alpha[k][i];
    }
  }
}

}// end namespace otb

#endif
