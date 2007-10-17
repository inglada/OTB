/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshEulerOperatorJoinFacetFunction.h,v $
  Language:  C++
  Date:      $Date: 2007/08/02 23:38:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadEdgeMeshEulerOperatorJoinFacetFunction_h
#define __itkQuadEdgeMeshEulerOperatorJoinFacetFunction_h

#include "itkQuadEdgeMeshFunctionBase.h"

namespace itk
{
/**
 * \class EulerOperatorJoinFacetFunction
 * \ingroup QEMeshModifierFunctions
 *
 * \brief Join the two facets which are on both sides of a given internal edge.
 */
template < class TMesh, class TQEType >
class ITK_EXPORT QuadEdgeMeshEulerOperatorJoinFacetFunction :
   public QuadEdgeMeshFunctionBase< TMesh, TQEType* >
{
public:
  /** Standard class typedefs. */
  typedef QuadEdgeMeshEulerOperatorJoinFacetFunction      Self;
  typedef QuadEdgeMeshFunctionBase< TMesh, TQEType* >     Superclass;
  typedef itk::SmartPointer< Self >                       Pointer;
  typedef itk::SmartPointer< const Self >                 ConstPointer;
  
  itkNewMacro( Self );
  /** Run-time type information (and related methods). */
  itkTypeMacro( QuadEdgeMeshEulerOperatorJoinFacetFunction, QuadEdgeMeshFunctionBase );

  /** Type of QuadEdge with which to apply slicing. */
  typedef TQEType QEType;

  typedef typename Superclass::MeshType   MeshType;
  typedef typename Superclass::OutputType OutputType;

  /** Evaluate at the specified input position */
  virtual OutputType Evaluate( QEType* e );

protected:
  QuadEdgeMeshEulerOperatorJoinFacetFunction(){};
  ~QuadEdgeMeshEulerOperatorJoinFacetFunction(){};

private:
  QuadEdgeMeshEulerOperatorJoinFacetFunction(const Self& ); //purposely not implemented
  void operator=(const Self& );        //purposely not implemented

};

} // namespace itk

#include "itkQuadEdgeMeshEulerOperatorJoinFacetFunction.txx"

#endif

// eof - $RCSfile: itkQuadEdgeMeshEulerOperatorJoinFacetFunction.h,v $
