/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodSampler.h,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:24:04 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNeighborhoodSampler_h
#define __itkNeighborhoodSampler_h

#include "itkMacro.h"
#include "itkObject.h"
#include "itkSample.h"
#include "itkSubsample.h"
#include "itkArray.h"
#include "itkSampleAlgorithmBase.h"

namespace itk { 
namespace Statistics {

/** \class NeighborhoodSampler 
 *  \brief generates a Subsample that is sampled from the input sample
 *   using a spherical kernel.
 *
 *  The resulting Subsample has measurement vectors that falls in a
 * hyper-sphere that is defined by a center and a radius. To set
 * the center, use SetCenter method, and to set radius, use SetRadius
 * method. The distance metric is Euclidean one.
 * 
 * <b>Recent API changes:</b>
 * The static const macro to get the length of a measurement vector,
 * \c MeasurementVectorSize  has been removed to allow the length of a measurement
 * vector to be specified at run time. Please use the function 
 * GetMeasurementVectorSize() to get the length. The typedef for \c CenterType
 * has changed from FixedArray to Array
 */

template < class TSample >
class ITK_EXPORT NeighborhoodSampler : public SampleAlgorithmBase< TSample >
{
public:
  /** Standard class typedefs */
  typedef NeighborhoodSampler            Self;
  typedef SampleAlgorithmBase< TSample > Superclass;
  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(NeighborhoodSampler, SampleAlgorithmBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** MeasurementVector typedef support */ 
  typedef TSample SampleType;


  /** Enums and typedefs from the TSample */
  typedef typename TSample::MeasurementVectorType MeasurementVectorType;
  typedef typename TSample::MeasurementType       MeasurementType;
  typedef typename TSample::FrequencyType         FrequencyType;
  typedef typename TSample::InstanceIdentifier    InstanceIdentifier;

  /** typedefs from the superclass */
  typedef typename Superclass::InputSampleType InputSampleType;

  /** Type of the output subsample object */
  typedef Subsample< TSample > SubsampleType;

  /** Type of the array of the radii */ 
  typedef double RadiusType;

  /** Type of the array of the radii */ 
  typedef Array< double > CenterType;

  /** Sets the center of the spherical kernel */
  void SetCenter(CenterType* center)
    {
    if( this->GetMeasurementVectorSize() && 
      ( center->Size() != this->GetMeasurementVectorSize() ) )
      {
      itkExceptionMacro( << "Size of measurement vectors in the sample is: " <<
         this->GetMeasurementVectorSize() << " but size of center is: " <<
         center->Size() );
      }
    
    if ( m_Center != center )
      {
      m_Center = center;
      this->Modified();
      }
    }

  /** Gets the center */
  CenterType* GetCenter() 
    { return m_Center; }

  /** Sets the radius of the kernel */
  void SetRadius(RadiusType* radius)
    { 
    if ( m_Radius != radius )
      {
      m_Radius = radius;
      this->Modified();
      }
    }

  /** Gets the radius */
  RadiusType* GetRadius()
    { return m_Radius; } 

  /** Output of this algorithm */
  typedef SubsampleType OutputType;

  /** Output of this algorithm */
  typedef typename SubsampleType::Pointer OutputPointer;

  /** Gets the Subsample */
  OutputPointer GetOutput();

protected:
  NeighborhoodSampler();
  virtual ~NeighborhoodSampler() {}
  virtual void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();

private:
  NeighborhoodSampler(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  CenterType*                     m_Center;
  RadiusType*                     m_Radius;
  typename SubsampleType::Pointer m_Subsample;
}; // end of class

} // end of namespace Statistics 
} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNeighborhoodSampler.txx"
#endif

#endif
