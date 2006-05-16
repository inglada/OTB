/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Classifier
  $Id$


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

  // FIXME :  typedef TLabel ClassLabelType ;
  typedef unsigned int ClassLabelType ;
  typedef std::vector< ClassLabelType > ClassLabelVectorType ;

//   /** Sets the target data that will be classified by this */
//  void SetSample(const TSample* sample) ;

//   /** Returns the target data */
//   const TSample* GetSample() const;

//   /** Sets the user given class labels for membership functions.
//    * If users do not provide class labels for membership functions by calling
//    * this function, then the index of the membership function vector for a
//    * membership function will be used as class label of measurement vectors
//    * belong to the membership function */ 
//   void SetMembershipFunctionClassLabels( ClassLabelVectorType& labels) ;

//   /** Gets the user given class labels */
//   ClassLabelVectorType& GetMembershipFunctionClassLabels() 
//   { return m_ClassLabels ; }

//   /** Returns the classification result */
  OutputType* GetOutput() ;


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
  void DoClassification() ;

private:
  /** Target data sample pointer*/
  const TSample* m_Sample ;

  /** Output pointer (MembershipSample) */
  typename OutputType::Pointer m_Output ;

  /** User given class labels for membership functions */
  ClassLabelVectorType m_ClassLabels ;

  SVMModelPointer m_Model;
} ; // end of class


} // end of namespace otb


#ifndef ITK_MANUAL_INSTANTIATION
#include "otbSVMClassifier.txx"
#endif

#endif







