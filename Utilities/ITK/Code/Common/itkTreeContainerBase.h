/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTreeContainerBase.h,v $
  Language:  C++
  Date:      $Date: 2004-11-01 16:31:24 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTreeContainerBase_h
#define __itkTreeContainerBase_h

#include "itkMacro.h"
#include <itkObject.h>
#include <iostream>
#include <itkObjectFactory.h>
#include <itkTreeNode.h>

namespace itk
{


template <class TValueType>
class TreeContainerBase : public Object
{
 
public:

  typedef Object Superclass;
  typedef TreeContainerBase Self;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef TValueType ValueType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(TreeContainerBase, Object);

  /** Set the root element. 
   * A new node is created and the element is added to the node */
  virtual bool SetRoot( TValueType element ) = 0;

  /** Set the root has a node */
  virtual bool SetRoot( TreeNode<TValueType>* node) = 0;

  /** Return true if the tree contains the element */
  virtual bool Contains( const TValueType element ) = 0;

  /** Return the number of nodes in the tree */
  virtual int Count() const = 0;

  /** Return if the element is a leaf */
  virtual bool IsLeaf( const TValueType element ) = 0;
  
  /** Return if the element is root */
  virtual bool IsRoot( const TValueType element ) = 0;

  /** Clear the tree */
  virtual bool Clear() = 0;

  /** Get the root as a node */
  virtual const TreeNode<TValueType>* GetRoot() const = 0;
  
  /** Set if the tree is a subtree */
  void SetSubtree(bool val) {m_SubTree = val;}

protected:

  TreeContainerBase() {}; 
  virtual ~TreeContainerBase() {}; 
  bool  m_SubTree;

};



} // namespace itk

#endif

