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
#ifndef __otbVectorDataToDSValidatedVectorDataFilter_txx
#define __otbVectorDataToDSValidatedVectorDataFilter_txx

#include "otbVectorDataToDSValidatedVectorDataFilter.h"
#include "otbMassOfBelief.h"
#include <iostream>

namespace otb
{

// Constructor
template <class TVectorData, class TPrecision>
  VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::VectorDataToDSValidatedVectorDataFilter() :
  m_CriterionFormula("((Belief + Plausibility)/2) >= 0.5"),
  m_CurrentID(0)
{
  this->SetNumberOfRequiredInputs(1);
  //Default road descriptors
  std::vector<double> ndvi, radiom, lsd, shadow;
  ndvi.push_back(0.25); ndvi.push_back(0.5); ndvi.push_back(0.75); ndvi.push_back(0.99);
  radiom.push_back(0.25); radiom.push_back(0.5); radiom.push_back(0.75); radiom.push_back(0.90);
  //lsd.push_back(0.25); lsd.push_back(0.5); lsd.push_back(0.75); lsd.push_back(0.96);
  //shadow.push_back(0.25); shadow.push_back(0.5); shadow.push_back(0.75); shadow.push_back(0.98);
 
  AddDescriptor("NDVI", ndvi);
  AddDescriptor("RADIOM", radiom);
  //AddDescriptor("LSD", lsd);
  //AddDescriptor("SHADOW", shadow);
  
  m_Parser =  ParserType::New();
  //m_Hypothesis.insert("NDVI");
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::AddDescriptor(const std::string& key, std::vector<double>  model)
{
  m_DescriptorModels.push_back(std::pair<std::string, std::vector<double> >(key, model));
  this->Modified();
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::ClearDescriptors()
{
  m_DescriptorModels.clear();
}


template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::GenerateData()
{
  //Initialize Dempster Shafer tools
  if( m_DescriptorModels.size() == 0 )
     itkExceptionMacro(<< "No defined Descriptor. Please add a descriptor model.");
  for (unsigned int i=0; i<m_DescriptorModels.size(); i++)
    {
    typename FuzzyVarType::Pointer fuz = FuzzyVarType::New();
    std::string fuzName, fuzName_;
    fuzName  = m_DescriptorModels[i].first;
    fuzName_ = m_DescriptorModels[i].first + "_";

    fuz->SetMembership(fuzName,
                       0.0,
                       0.0,
                       m_DescriptorModels[i].second[0],
                       m_DescriptorModels[i].second[1],
                       0.0,
                       m_DescriptorModels[i].second[3]);
    fuz->SetMembership(fuzName_,
                       m_DescriptorModels[i].second[1],
                       m_DescriptorModels[i].second[2],
                       1.0,
                       1.0,
                       0.0,
                       m_DescriptorModels[i].second[3]);
    
    m_FuzzyVars.push_back(fuz);

    m_Universe.insert(fuzName);
    m_Universe.insert(fuzName_);
    }

  //Initialize parser
  m_Parser->SetExpr(m_CriterionFormula);
  m_Parser->DefineVar("Belief", &m_Bel);
  m_Parser->DefineVar("Plausibility", &m_Plau);

  // Output
  this->GetOutput(0)->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);
  // Create the folder node
  typename DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(folder, document);
  this->GetOutput(0)->SetProjectionRef(this->GetInput()->GetProjectionRef());

  TreeIteratorType itVector(this->GetInput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {
      typename DataNodeType::Pointer currentGeometry = itVector.Get();
      typename JointMassOfBeliefFilterType::Pointer jointMassFilter = JointMassOfBeliefFilterType::New();

      for (unsigned int i=0; i<m_DescriptorModels.size(); i++)
        {
        if (currentGeometry->HasField(m_DescriptorModels[i].first))
          {
          LabelSetType H, H_;
          std::string fuzName, fuzName_;
          fuzName  = m_DescriptorModels[i].first;
          fuzName_ = m_DescriptorModels[i].first + "_";

          typename MassOfBeliefFunctionType::Pointer mass = MassOfBeliefFunctionType::New();
          mass->InitializePowerSetMasses(m_Universe);
          
          H.insert(fuzName);
          H_.insert(fuzName_);
          
          mass->SetMass(H, m_FuzzyVars[i]->GetMembership(fuzName, currentGeometry->GetFieldAsDouble(fuzName)));
          mass->SetMass(H_, m_FuzzyVars[i]->GetMembership(fuzName_, currentGeometry->GetFieldAsDouble(fuzName)));

          mass->EstimateUncertainty();
          
          jointMassFilter->PushBackInput(mass);
          }
        }
      jointMassFilter->Update();
      m_Bel  = jointMassFilter->GetOutput()->GetBelief(m_Hypothesis);
      m_Plau = jointMassFilter->GetOutput()->GetPlausibility(m_Hypothesis);

     if (m_Parser->Eval())
        {
        currentGeometry->SetNodeId(this->GetNextID());
        currentGeometry->SetFieldAsDouble("Belief", m_Bel);
        currentGeometry->SetFieldAsDouble("Plausi", m_Plau);
        this->GetOutput(0)->GetDataTree()->Add(currentGeometry, folder);
        }
      }
    ++itVector;
    }
}


// PrintSelf Method
template <class TVectorData, class TPrecision>
void
VectorDataToDSValidatedVectorDataFilter<TVectorData, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
