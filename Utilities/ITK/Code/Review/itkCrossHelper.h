/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCrossHelper.h,v $
  Language:  C++
  Date:      $Date: 2008-10-06 00:02:03 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCrossHelper_h
#define __itkCrossHelper_h

namespace itk
{
/** \class Cross
 * \brief Compute the cross product of two vectors of dimension 3,
 *        independently of the type of the values of vector's elements.
 *
 *  FIXME: Strictly speaking, the Cross product should not return a Vector, but
 *  a CovariantVector, since it behaves differently under Affine
 *  Transformations.
 *
 */
template< typename TVector >
class CrossHelper
{
public:
  typedef TVector                           VectorType;
  typedef typename VectorType::ValueType    ValueType;

  itkStaticConstMacro ( Dimension, unsigned int, VectorType::Dimension );

  /**
   * \param[in] iU
   * \param[in] iV
   * \return \f$ \boldsymbol{iU} \cdot \boldsymbol{iV} \f$
   */
  VectorType operator ( ) ( const VectorType& iU,
                            const VectorType& iV ) const
    {
    assert ( Dimension > 2 );

    VectorType oCross;

    // *** Arnaud : InputVDimension == 3
    oCross[0] = iU[1] * iV[2] - iV[1] * iU[2];
    oCross[1] = iV[0] * iU[2] - iU[0] * iV[2];
    oCross[2] = iU[0] * iV[1] - iV[0] * iU[1];

    for( unsigned int dim = 3; dim < Dimension; dim++ )
      {
      oCross[dim] = 0.0;
      }

    return oCross;
    }
};
}

#endif
