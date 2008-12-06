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
#ifndef __otbSVMClassifier_h
#define __otbSVMClassifier_h

#include "itkSampleClassifier.h"
#include "otbSVMModel.h"
#include "itkVectorImage.h"

namespace otb{

/** \class SVMClassifier
 *  \brief SVM-based classifier
 *
 * The first template argument is the type of the target sample data
 * that this classifier will assign a class label for each measurement
 * vector. The second one is the pixel type of the labels to be
 * produced by the classifier.
 *
 * Before you call the GenerateData method to start the classification
 * process, you should plug in all necessary parts ( a SVM model and a
 * target sample data).
 *
 * The classification result is stored in a vector of Subsample object.
 * Each class has its own class sample (Subsample object) that has
 * InstanceIdentifiers for all measurement vectors belong to the class.
 * The InstanceIdentifiers come from the target sample data. Therefore,
 * the Subsample objects act as separate class masks.
 *
 *
 */

template< class TSample, class TLabel >
class ITK_EXPORT SVMClassifier :
      public itk::Statistics::SampleClassifier< TSample >
{
public:
  /** Standard class typedef*/
  typedef SVMClassifier Self;
  typedef itk::Statistics::SampleClassifier< TSample > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

 /** Standard macros */
  itkTypeMacro(SVMClassifier, itk::Statistics::SampleClassifier);
  itkNewMacro(Self) ;

  /** Output type for GetClassSample method */
  typedef itk::Statistics::MembershipSample< TSample > OutputType ;

  /** typedefs from TSample object */
  typedef typename TSample::MeasurementType MeasurementType ;
  typedef typename TSample::MeasurementVectorType MeasurementVectorType ;
  /*typedef typename itk::NumericTraits<TSample::ValueType>::RealType
   InputPixelType ;*/
  typedef typename TSample::MeasurementVectorType::ValueType InputPixelType ;

  /** typedefs from Superclass */
  typedef typename Superclass::MembershipFunctionPointerVector
    MembershipFunctionPointerVector ;

  /** typedef for label type */
  typedef TLabel ClassLabelType ;


   /** Returns the classification result */
  OutputType* GetOutput() ;
  void SetOutput(OutputType* output);

  /** Type definitions for the SVM Model. */
  typedef SVMModel< InputPixelType, TLabel >   SVMModelType;
  typedef typename SVMModelType::Pointer     SVMModelPointer;

  /** Set the model */
  itkSetMacro(Model, SVMModelPointer);

  /** Get the number of classes. */
  itkGetConstReferenceMacro(Model, SVMModelPointer);

  void Update() ;

protected:
  SVMClassifier() ;
  virtual ~SVMClassifier() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Starts the classification process */
  void GenerateData() ;
  virtual void DoClassification() ;

private:

  /** Output pointer (MembershipSample) */
  typename OutputType::Pointer m_Output ;

  SVMModelPointer m_Model;
} ; // end of class


} // end of namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMClassifier.txx"
#endif

#endif







