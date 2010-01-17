/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearLine.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:19:16 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0LinearLine_h
#define __itkFEMElement2DC0LinearLine_h

#include "itkFEMElementStd.h"

namespace itk {
namespace fem {

/**
 * \class Element2DC0LinearLine
 * \brief 2-noded, linear, C0 continuous line element in 2D space.
 */
class Element2DC0LinearLine : public ElementStd<2,2>
{
  typedef ElementStd<2,2> TemplatedParentClass;
  FEM_ABSTRACT_CLASS( Element2DC0LinearLine, TemplatedParentClass )
public:

  
  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to numeric integration
   */

  enum { DefaultIntegrationOrder = 1 };
  
  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType& pt, Float& w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;

  //////////////////////////////////////////////////////////////////////////
  /**
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions( const VectorType& pt ) const;

  virtual void ShapeFunctionDerivatives( const VectorType& pt, MatrixType& shapeD ) const;

  // FIXME: Write a proper implementation
  virtual bool GetLocalFromGlobalCoordinates( const VectorType& globalPt, VectorType& localPt ) const;

  /**
   * We need to provide our own implementation of calculating Jacobian,
   * because the element lives in 2D space and has only one dimension.
   * The default implementation of Jacobian in the Element base class
   * is not correct since it assumes that the number of element dimensions
   * is equal to the number of spatial dimensions.
   *
   * Jacobian is a scalar for this element.
   */
  virtual void Jacobian( const VectorType& pt, MatrixType& J, const MatrixType* pshapeD = 0 ) const;

  /**
   * Draw the element on the specified device context
   */
#ifdef FEM_BUILD_VISUALIZATION
  void Draw(CDC* pDC, Solution::ConstPointer sol) const;
#endif

};

}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearLine_h
