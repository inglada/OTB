/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCompletelyConnectedWeightSet.h,v $
  Language:  C++
  Date:      $Date: 2009-01-24 21:33:49 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkCompletelyConnectedWeightSet_h
#define __itkCompletelyConnectedWeightSet_h

#include "itkWeightSetBase.h"

namespace itk
{
namespace Statistics
{
template<class TMeasurementVector, class TTargetVector>
class CompletelyConnectedWeightSet : public WeightSetBase<TMeasurementVector, TTargetVector>
{
public:
  #define MAX_SIZE 1000

  typedef CompletelyConnectedWeightSet                     Self;
  typedef WeightSetBase<TMeasurementVector, TTargetVector> Superclass;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;

  itkTypeMacro(CompletelyConnectedWeightSet, WeightSetBase);
  itkNewMacro(Self);

  void SetCompleteConnectivity();
  void SetRandomConnectivity(int[][MAX_SIZE]);

protected:

  CompletelyConnectedWeightSet();
  virtual ~CompletelyConnectedWeightSet(){};

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCompletelyConnectedWeightSet.txx"
#endif

#endif
