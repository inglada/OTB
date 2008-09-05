/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuaternionOrientationAdapter.h,v $
  Language:  C++
  Date:      $Date: 2005-10-31 17:52:09 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuaternionOrientationAdapter_h
#define __itkQuaternionOrientationAdapter_h
#include "itkOrientationAdapter.h"
#include "itkQuaternionRigidTransform.h"
#include "itkConceptChecking.h"

namespace itk
{
/** \class QuaternionOrientationAdapter
 *  \brief converts QuaternionOrientation flags to/from direction cosines
 */
namespace QuaternionOrientationAdapterClasses
  {
    typedef QuaternionRigidTransform<double> TransformType;
    typedef TransformType::Pointer TransformPointerType;
  }
template <int Dimension>
class QuaternionOrientationAdapter : 
    public OrientationAdapterBase<QuaternionOrientationAdapterClasses::TransformPointerType,Dimension>
{
public:
  /** typedef for superclass */
  typedef QuaternionOrientationAdapter Self;

  typedef OrientationAdapterBase<QuaternionOrientationAdapterClasses::TransformPointerType,Dimension>
  SuperClass;
  typedef QuaternionRigidTransform<double> OrientationRootType;
  typedef QuaternionOrientationAdapterClasses::TransformPointerType OrientationType;

  /** The dimension of the input image must be 3. */
  itkConceptMacro(DimensionShouldBe3,
    (Concept::SameDimension<Dimension,3>));

  /** typedef for direction cosines */
  typedef typename SuperClass::DirectionType DirectionType;

  /** convert from direction cosines. */
  virtual OrientationType FromDirectionCosines(const DirectionType &Dir)
  {
    OrientationType q = OrientationRootType::New();
    q->SetMatrix(Dir);
    return q;
  }

  /** convert to direction cosines. */
  virtual DirectionType ToDirectionCosines(const OrientationType &Or)
  {
    return Or->GetMatrix();
  }

};

} // namespace itk


#endif // __itkQuaternionOrientationAdapter_h
