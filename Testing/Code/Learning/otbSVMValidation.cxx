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



#include "itkExceptionObject.h"
#include "itkListSample.h"
#include <iostream>

#include "otbSVMSampleListModelEstimator.h"
#include "otbSVMClassifier.h"
#include "otbSVMKernels.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "otbSVMClassifier.h"
#include "otbConfusionMatrixCalculator.h"

#include "otbMath.h"

#include <fstream>

int otbSVMValidation(int argc, char* argv[])
{
 if(argc != 13)
 {
   std::cerr<<"Usage: "<<argv[0]<<" nbTrainingSamples nbValidationSamples positiveCenterX positiveCenterY negativeCenterX negativeCenterY positiveRadiusMin positiveRadiusMax negativeRadiusMin negativeRadiusMax kernel probEstimate"<<std::endl;
   return EXIT_FAILURE;
 }
 unsigned int nbTrainingSamples = atoi(argv[1]);
 unsigned int nbValidationSamples = atoi(argv[2]);
 double cpx = atof(argv[3]);
 double cpy = atof(argv[4]);
 double cnx = atof(argv[5]);
 double cny = atof(argv[6]);
 double prmin = atof(argv[7]);
 double prmax = atof(argv[8]);
 double nrmin = atof(argv[9]);
 double nrmax = atof(argv[10]);
 unsigned int kernel = atoi(argv[11]);
 bool   probEstimate = atoi(argv[12]);

  typedef double                                          InputPixelType;
  typedef unsigned short                                  LabelType;

  typedef itk::VariableLengthVector<InputPixelType>       SampleType;
  typedef itk::Statistics::ListSample<SampleType>         ListSampleType;
  typedef itk::FixedArray<LabelType, 1>                   TrainingSampleType;
  typedef itk::Statistics::ListSample<TrainingSampleType> TrainingListSampleType;
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, TrainingListSampleType> EstimatorType;
  typedef otb::SVMClassifier<ListSampleType, LabelType>   ClassifierType;
  typedef ClassifierType::OutputType                      ClassifierOutputType;
  typedef otb::ConfusionMatrixCalculator
      <TrainingListSampleType, TrainingListSampleType> ConfusionMatrixCalculatorType;


  RandomGeneratorType::Pointer random = RandomGeneratorType::New();
  random->SetSeed((unsigned int)0);

  // First, generate training and validation sets
  ListSampleType::Pointer trainingSamples = ListSampleType::New();
  TrainingListSampleType::Pointer trainingLabels = TrainingListSampleType::New();
  ListSampleType::Pointer validationSamples = ListSampleType::New();
  TrainingListSampleType::Pointer validationLabels = TrainingListSampleType::New();

  // Generate training set
//  std::ofstream training("training.csv");
  for(unsigned int i =0; i < nbTrainingSamples; ++i)
  {
    // Generate a positive sample
    double angle = random->GetVariateWithOpenUpperRange( otb::CONST_2PI );
    double radius = random->GetUniformVariate(prmin, prmax);
    SampleType pSample(2);
    pSample[0] = cpx+radius*vcl_sin(angle);
    pSample[1] = cpy+radius*vcl_cos(angle);
    TrainingSampleType label;
    label[0]=1;
    trainingSamples->PushBack(pSample);
    trainingLabels->PushBack(label);

//    training<<"1 1:"<<pSample[0]<<" 2:"<<pSample[1]<<std::endl;

    // Generate a negative sample
    angle = random->GetVariateWithOpenUpperRange( otb::CONST_2PI );
    radius = random->GetUniformVariate(nrmin, nrmax);
    SampleType nSample(2);
    nSample[0] = cnx+radius*vcl_sin(angle);
    nSample[1] = cny+radius*vcl_cos(angle);
    label[0]=2;
    trainingSamples->PushBack(nSample);
    trainingLabels->PushBack(label);

//    training<<"2 1:"<<nSample[0]<<" 2:"<<nSample[1]<<std::endl;

  }
//  training.close();

  // Generate validation set

//  std::ofstream validation("validation.csv");
  for(unsigned int i =0; i < nbValidationSamples; ++i)
    {
      // Generate a positive sample
      double angle = random->GetVariateWithOpenUpperRange( otb::CONST_2PI );
      double radius = random->GetUniformVariate(prmin, prmax);
      SampleType pSample(2);
      pSample[0] = cpx+radius*vcl_sin(angle);
      pSample[1] = cpy+radius*vcl_cos(angle);
      TrainingSampleType label;
      label[0]=1;
      validationSamples->PushBack(pSample);
      validationLabels->PushBack(label);
//    validation<<"1 1:"<<pSample[0]<<" 2:"<<pSample[1]<<std::endl;

      // Generate a negative sample
      angle = random->GetVariateWithOpenUpperRange( otb::CONST_2PI );
      radius = random->GetUniformVariate(nrmin, nrmax);
      SampleType nSample(2);
      nSample[0] = cnx+radius*vcl_sin(angle);
      nSample[1] = cny+radius*vcl_cos(angle);
      label[0]=2;
      validationSamples->PushBack(nSample);
      validationLabels->PushBack(label);
//    validation<<"2 1:"<<nSample[0]<<" 2:"<<nSample[1]<<std::endl;

    }
//  validation.close();

  // Learn
  EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetInputSampleList(trainingSamples);
  estimator->SetTrainingSampleList(trainingLabels);

  estimator->SetKernelType(kernel);
  estimator->DoProbabilityEstimates(probEstimate);
//  estimator->SetParametersOptimization(true);
  estimator->Update();

//  estimator->SaveModel("model.svm");

  // Classify
  ClassifierType::Pointer validationClassifier = ClassifierType::New();
  validationClassifier->SetSample(validationSamples);
  validationClassifier->SetNumberOfClasses(2);
  validationClassifier->SetModel(estimator->GetModel());
  validationClassifier->Update();

  // Confusion
  ClassifierOutputType::ConstIterator it = validationClassifier->GetOutput()->Begin();
  ClassifierOutputType::ConstIterator itEnd = validationClassifier->GetOutput()->End();

  TrainingListSampleType::Pointer classifierListLabel = TrainingListSampleType::New();

  while (it != itEnd)
  {
    classifierListLabel->PushBack(it.GetClassLabel());
    ++it;
  }

  ConfusionMatrixCalculatorType::Pointer confMatCalc =  ConfusionMatrixCalculatorType::New();

  confMatCalc->SetReferenceLabels(validationLabels);
  confMatCalc->SetProducedLabels(classifierListLabel);

  confMatCalc->Update();

  std::cout<<std::endl;
  std::cout<<"Confusion matrix: "<<std::endl<< confMatCalc->GetConfusionMatrix()<<std::endl<<std::endl;
  std::cout<<"Kappa Index: "<<std::endl<< confMatCalc->GetKappaIndex()<<std::endl<<std::endl;

  if(confMatCalc->GetKappaIndex()!=1)
  {
    std::cerr<<"Kappa index should be 1."<<std::endl;
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}

