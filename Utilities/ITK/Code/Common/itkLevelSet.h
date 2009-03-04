/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLevelSet.h,v $
  Language:  C++
  Date:      $Date: 2009-02-05 19:05:00 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLevelSet_h
#define __itkLevelSet_h

#include "itkIndex.h"
#include "itkImage.h"
#include "itkVectorContainer.h"
#include "itkVector.h"
#include "itkLevelSetNode.h"

namespace itk
{
/**  \class LevelSetTypeDefault
 * \brief Level set type information.
 *
 * LevelSetTypeDefault is a simple class that holds type information
 * useful for level set algorithms. This class is templated over the
 * level set image type.
 *
 * A NodeContainer contains a group or collection of level set 
 * node or grid points useful for representing a narrowband or
 * region of interest.
 *
 * \ingroup LevelSetSegmentation 
 */
template<class TLevelSet>
class ITK_EXPORT LevelSetTypeDefault
{
public:
  /** LevelSetType typedef support. */
  typedef LevelSetTypeDefault Self;
  typedef TLevelSet           LevelSetImageType;
  
  
  /** SetDimension enumeration. */
  itkStaticConstMacro(SetDimension, unsigned int, TLevelSet::ImageDimension);

  /** LevelSetPointer typedef support. */
  typedef typename TLevelSet::Pointer      LevelSetPointer;
  typedef typename TLevelSet::ConstPointer LevelSetConstPointer;

  /** PixelType typedef support. */
  typedef typename TLevelSet::PixelType PixelType;
  
  /** Node typdef support. */
  typedef
      LevelSetNode<PixelType, itkGetStaticConstMacro(SetDimension)> NodeType;

  /** NodeContainer typedef support. */
  typedef VectorContainer<unsigned int,NodeType> NodeContainer;

  /** NodeContainerPointer typedef support. */
  typedef typename NodeContainer::Pointer NodeContainerPointer;
};


/** \class AuxVarTypeDefault
 * \brief Level set auxiliary variables type information.
 *
 * \brief AuxVarTypeDefault is a simple class that holds type information
 * for auxiliary variables in some level set algorithms. This class is templated
 * over the auxiliary variable data type, the number of auxiliary variables
 * and the level set dimension.
 *
 * A AuxValueContainer contains a collection of auxiliary
 * values vectors. It is used in conjunction with 
 * LevelSetTypeDefault::NodeContainer to represent auxiliary variable values 
 * in a group or collection of level set nodes or grid positions.
 *
 * \ingroup LevelSetSegmentation 
 */
template < 
class TPixel,
unsigned int VAuxDimension = 1,
unsigned int VSetDimension = 2
>
class ITK_EXPORT AuxVarTypeDefault
{
public:
  /** Standard typedefs */
  typedef AuxVarTypeDefault Self;
  
  /** PixelType typedef support. */ 
  typedef TPixel AuxValueType;

  /** Auxiliary variable dimension. */
  itkStaticConstMacro(AuxDimension, unsigned int, VAuxDimension);

  /** Level set dimension. */
  itkStaticConstMacro(SetDimension, unsigned int, VSetDimension);

  /** AuxVector typedef support. */
  typedef Vector<TPixel,VAuxDimension> AuxValueVectorType;

  /** AuxContainer typdef support. */
  typedef VectorContainer<unsigned int,AuxValueVectorType> AuxValueContainer;

  /** AuxImage typdef support. */
  typedef Image<AuxValueType, VSetDimension> AuxImageType;

  /** AuxImagePointer typedef support. */
  typedef typename AuxImageType::Pointer      AuxImagePointer;
  typedef typename AuxImageType::ConstPointer AuxImageConstPointer;
};


} // end namespace itk

#endif
