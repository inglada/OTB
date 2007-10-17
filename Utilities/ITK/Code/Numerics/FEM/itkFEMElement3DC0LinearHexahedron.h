/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement3DC0LinearHexahedron.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:40 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement3DC0LinearHexahedron_h
#define __itkFEMElement3DC0LinearHexahedron_h

#include "itkFEMElementStd.h"

namespace itk {
namespace fem {




/**
 * \class Element3DC0LinearHexahedron
 * \brief 8-noded, linear, C0 continuous finite element in 3D space.
 */
class Element3DC0LinearHexahedron : public ElementStd<8,3>
{
typedef ElementStd<8,3> TemplatedParentClass;
FEM_ABSTRACT_CLASS( Element3DC0LinearHexahedron, TemplatedParentClass )
public:


//////////////////////////////////////////////////////////////////////////
  /*
   * Methods related to numeric integration
   */

  virtual void GetIntegrationPointAndWeight(unsigned int i, VectorType& pt, Float& w, unsigned int order) const;

  virtual unsigned int GetNumberOfIntegrationPoints(unsigned int order) const;



//////////////////////////////////////////////////////////////////////////
  /*
   * Methods related to the geometry of an element
   */

  virtual VectorType ShapeFunctions( const VectorType& pt ) const;

  virtual void ShapeFunctionDerivatives( const VectorType& pt, MatrixType& shapeD ) const;

  virtual bool GetLocalFromGlobalCoordinates( const VectorType& globalPt , VectorType& localPt) const;

  /**
   * Draw the element on the specified device context
   */
#ifdef FEM_BUILD_VISUALIZATION
  void Draw(CDC* pDC, Solution::ConstPointer sol) const;
#endif

};




}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearHexahedron_h
