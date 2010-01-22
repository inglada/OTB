/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMembershipFunctionBase.h,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:24:02 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMembershipFunctionBase_h
#define __itkMembershipFunctionBase_h

#include "itkFunctionBase.h"
#include "itkMeasurementVectorTraits.h"

namespace itk { 
namespace Statistics {

/** \class MembershipFunctionBase
 * \brief MembershipFunctionBase class declares common interfaces 
 * for membership functions.
 *
 * As a function derived from FunctionBase, users use Evaluate method
 * get result. However, the return value type of the method is fixed
 * as double. Any function derived from this class returns quantitative
 * measure for how well the vector x belong to the class ( or group)
 * represented by the function.
 */

template< class TVector >
class ITK_EXPORT MembershipFunctionBase :
    public FunctionBase< TVector, double >
{
public:
  /** Standard class typedefs */
  typedef MembershipFunctionBase          Self;
  typedef FunctionBase< TVector, double > Superclass;
  typedef SmartPointer< Self >            Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Strandard macros */
  itkTypeMacro(MembershipFunctionBase, FunctionBase);

  /** Typedef for the length of each measurement vector */
  typedef unsigned int  MeasurementVectorSizeType;

  /** Method to get membership score (discriminant score) of an entity. */
  virtual double Evaluate(const TVector &x) const = 0;
  
  /** Set/Get Macros to set the length of each measurement vector. */
  itkSetMacro( MeasurementVectorSize, MeasurementVectorSizeType );
  itkGetConstMacro( MeasurementVectorSize, MeasurementVectorSizeType ); 
 
protected:
  MembershipFunctionBase() : m_MeasurementVectorSize( 
      MeasurementVectorTraits::GetLength( TVector() ) ) {}
  virtual ~MembershipFunctionBase(void) {}

  void PrintSelf(std::ostream& os, Indent indent) const
    { 
    Superclass::PrintSelf(os,indent); 
    os << indent << "Length of measurement vectors: " 
                << m_MeasurementVectorSize << std::endl;
    }

  MeasurementVectorSizeType m_MeasurementVectorSize;
}; // end of class

} // end of namespace Statistics
} // end namespace itk

#endif
