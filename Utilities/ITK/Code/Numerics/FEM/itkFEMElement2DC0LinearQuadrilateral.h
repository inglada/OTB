/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearQuadrilateral.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:25:49 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0LinearQuadrilateral_h
#define __itkFEMElement2DC0LinearQuadrilateral_h

#include "itkFEMElementStd.h"

namespace itk {
namespace fem {

/**
 * \class Element2DC0LinearQuadrilateral
 * \brief 4-noded, linear, C0 continuous finite element in 2D space.
 */
class Element2DC0LinearQuadrilateral : public ElementStd<4,2>
{
  typedef ElementStd<4,2> TemplatedParentClass;
  FEM_ABSTRACT_CLASS( Element2DC0LinearQuadrilateral, TemplatedParentClass )
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

  virtual bool GetLocalFromGlobalCoordinates( const VectorType& globalPt, VectorType& localPt) const;

  /**
   * Draw the element on the specified device context
   */
#ifdef FEM_BUILD_VISUALIZATION
  void Draw(CDC* pDC, Solution::ConstPointer sol) const;
#endif

};

}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearQuadrilateral_h
