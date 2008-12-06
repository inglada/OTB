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
#ifndef __otbSVMPointSetModelEstimator_h
#define __otbSVMPointSetModelEstimator_h

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModel.h"
#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMPointSetModelEstimator

 * \brief Class for SVM model
 * estimation from PointSets used for classification.
 *
 *
 * The basic functionality of the SVMPointSetModelEstimator is to
 * generate the models used in SVM classification. It
 * requires one input poinset and a training pointset to be provided by the
 * user.  This object supports data handling of multiband data. The
 * object accepts the input image in vector format only, where each
 * pixel is a vector and each element of the vector corresponds to an
 * entry from 1 particular band of a multiband dataset. A single band
 * data set is treated as a vector with a single element for every
 * vector. The classified data is treated as a single band scalar
 * data.
 *
 * EstimateModels() uses the libsvm library for SVM learning.
 *
 * \ingroup ClassificationFilters
 */
template <class TInputPointSet,
          class TTrainingPointSet>
class ITK_EXPORT SVMPointSetModelEstimator:
    public SVMModelEstimator<typename TInputPointSet::PixelType::value_type, typename TTrainingPointSet::PixelType>
{
public:
  /** Standard class typedefs. */
  typedef SVMPointSetModelEstimator   Self;
  typedef SVMModelEstimator<typename TInputPointSet::PixelType::value_type,
			    typename TTrainingPointSet::PixelType>
                            Superclass;

  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMPointSetModelEstimator, SVMModelEstimator);

  /** Type definition for the input image. */
    /** Type definitions for the training image. */
  typedef typename TInputPointSet::Pointer   InputPointSetPointer;

  /** Type definitions for the training image. */
  typedef typename TTrainingPointSet::Pointer TrainingPointSetPointer;

  /** Type definition for the vector associated with
   * input image pixel type. */
  typedef typename TInputPointSet::PixelType InputPointSetPixelType;
  typedef typename TInputPointSet::PointType InputPointType;

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingPointSet::PixelType TrainingPointSetPixelType;

  /** Type definitions for the iterators for the input and training images. */
  typedef typename
  TInputPointSet::PointsContainerConstIterator  InputPointSetIteratorType;
  typedef typename
  TTrainingPointSet::PointsContainerConstIterator TrainingPointSetIteratorType;

  /** Set the input image. */
  itkSetObjectMacro(InputPointSet,TInputPointSet);

  /** Get the input image. */
  itkGetObjectMacro(InputPointSet,TInputPointSet);


  /** Set the training image. */
  itkSetMacro(TrainingPointSet,TrainingPointSetPointer);

  /** Get the training image. */
  itkGetMacro(TrainingPointSet,TrainingPointSetPointer);



protected:
  SVMPointSetModelEstimator();
  ~SVMPointSetModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


  virtual void BuildProblem();

private:
  SVMPointSetModelEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



  TrainingPointSetPointer  m_TrainingPointSet;
  InputPointSetPointer  m_InputPointSet;



}; // class SVMPointSetModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMPointSetModelEstimator.txx"
#endif



#endif

