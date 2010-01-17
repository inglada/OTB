/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0QuadraticTriangular.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:44:35 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0QuadraticTriangular_h
#define __itkFEMElement2DC0QuadraticTriangular_h

#include "itkFEMElementStd.h"

namespace itk {
namespace fem {

/**
 * \class Element2DC0QuadraticTriangular
 * \brief 3-noded, quadratic, C0 continuous finite element in 2D space.
 */
class Element2DC0QuadraticTriangular : public ElementStd<6,2>
{
  typedef ElementStd<3,2> TemplatedParentClass;
  FEM_ABSTRACT_CLASS( Element2DC0QuadraticTriangular, TemplatedParentClass )
public:

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  enum { DefaultIntegrationOrder = 2 };

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType& pt, Float& w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions( const VectorType& pt ) const;

  virtual void ShapeFunctionDerivatives( const VectorType& pt, MatrixType& shapeD ) const;

  // FIXME: Write a proper implementation
  virtual bool GetLocalFromGlobalCoordinates( const VectorType&, VectorType& ) const
    { 
    throw;
    return false;
    }

  // Since the Jacobian is not quadratic, we need to provide our
  // own implementation of calculating the determinant and inverse.
  virtual Float JacobianDeterminant( const VectorType& pt, const MatrixType* pJ = 0 ) const;
  virtual void JacobianInverse( const VectorType& pt, MatrixType& invJ, const MatrixType* pJ = 0 ) const;

  /**
   * Draw the element on the specified device context
   */
#ifdef FEM_BUILD_VISUALIZATION
  void Draw(CDC* pDC, Solution::ConstPointer sol) const;
#endif

};

}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0QuadraticTriangular_h
