/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDenseFrequencyContainer2.h,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:55 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDenseFrequencyContainer2_h
#define __itkDenseFrequencyContainer2_h

#include <map>
#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkValarrayImageContainer.h"
#include "itkNumericTraits.h"
#include "itkMeasurementVectorTraits.h"

namespace itk { 
namespace Statistics {

/** \class DenseFrequencyContainer2 
 *  \brief his class is a container for frequencies of bins in an histogram.
 *
 * This class uses the ValarrayImageContainer class to store
 * frequencies. If the histogram has many zero frequency bins. 
 * use SparseFrequencyContainer.  You should access each bin 
 * by (InstanceIdentifier)index or measurement vector.
 *
 * \sa Histogram, SparseFrequencyContainer
 */
    
class ITK_EXPORT DenseFrequencyContainer2 
  : public Object
{
public:
  /** Standard class typedefs */
  typedef DenseFrequencyContainer2                          Self;
  typedef Object                                            Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DenseFrequencyContainer2, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InstanceIdenfitifer type alias */
  typedef MeasurementVectorTraits::InstanceIdentifier         InstanceIdentifier;

  /** Absoluate Frequency type alias */
  typedef MeasurementVectorTraits::AbsoluteFrequencyType      AbsoluteFrequencyType;
  
  /** Absolute Total frequency type */
  typedef MeasurementVectorTraits::TotalAbsoluteFrequencyType TotalAbsoluteFrequencyType;

  /** Relative Frequency type alias */
  typedef MeasurementVectorTraits::RelativeFrequencyType      RelativeFrequencyType;
  
  /** Relative Total frequency type */
  typedef MeasurementVectorTraits::TotalRelativeFrequencyType TotalRelativeFrequencyType;


  /** Internal storage class typedefs */
  typedef ValarrayImageContainer< InstanceIdentifier, AbsoluteFrequencyType > 
  FrequencyContainerType;

  typedef FrequencyContainerType::Pointer FrequencyContainerPointer;

  /** Calls the Initialize method of superclass to generate the offset table
   * and prepare the frequency container */
  void Initialize(unsigned long length);

  /** Calls the SetToZero method of superclass to initialize all the bins to Zero.
   *  This should be done before starting to call the IncreaseFrequency method. */
  void SetToZero();

  /** Sets the frequency of histogram using instance identifier. It returns
   * false when the Id is out of bounds. */
  bool SetFrequency(const InstanceIdentifier id, const AbsoluteFrequencyType value);

  /** Increases the frequency of a bin specified by the InstanceIdentifier by
   * one.  This function is convinient to create a histogram. It returns false
   * when the bin id is out of bounds. */
  bool IncreaseFrequency(const InstanceIdentifier id, 
                         const AbsoluteFrequencyType value);

  /** Method to get the frequency of a bin from the histogram. It returns zero
   * when the Id is out of bounds. */
  AbsoluteFrequencyType GetFrequency(const InstanceIdentifier id) const;

  /** Gets the sum of the frequencies */
  TotalAbsoluteFrequencyType GetTotalFrequency()
    {
    return m_TotalFrequency;
    }

protected:
  DenseFrequencyContainer2();
  virtual ~DenseFrequencyContainer2() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  DenseFrequencyContainer2(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Internal storage */
  FrequencyContainerPointer         m_FrequencyContainer;
  TotalAbsoluteFrequencyType        m_TotalFrequency;
}; // end of class

} // end of namespace Statistics
} // end of namespace itk

#endif
