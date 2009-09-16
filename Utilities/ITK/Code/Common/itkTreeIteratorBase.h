/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTreeIteratorBase.h,v $
  Language:  C++
  Date:      $Date: 2009-06-14 11:55:05 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTreeIteratorBase_h
#define __itkTreeIteratorBase_h

#include <itkTreeNode.h>

namespace itk {

/** \class TreeIteratorBase
 *  \brief TreeIteratorBase class
 * 
 * This class provides the base implementation for tree iterators
 *
 * Events will notify interested observers about tree changes. These events all derive from TreeChangeEvent. They are:
 *
 *  - TreeNodeChangeEvent: invoked when Set() is called, i.e. exactly one node changes
 *  - TreeAddEvent: invoked when Add() is called.
 *  - TreeRemoveEvent: when a single node has been removed, i.e. Disconnect() has been called.
 *  - TreePruneEvent: when a node and all its children were removed, i.e. Remove() has been called.
 *
 *  All those events have a member GetChangePosition(), which returns an iterator to the position that has changd. Please
 *  note that this iterator may not be fully functional, but you should always be able to use its Get() method to retrieve
 *  the thing it points to.
 *
 */
template <class TTreeType>
class TreeIteratorBase
{
public: 
  
  /** Typedefs */
  typedef TreeIteratorBase                    Self;
  typedef typename TTreeType::ValueType       ValueType;
  typedef typename TTreeType::TreeNodeType    TreeNodeType;

  /** Add an element to the tree */
  virtual bool Add(ValueType element);

  /** Add an element at a given position */
  virtual bool Add(int position, ValueType element);

  /** Add a subtree */
  virtual bool Add(TTreeType& subTree);

  /** Get a value */
  virtual const ValueType& Get() const;

  /** Get the subtree */
  virtual TTreeType* GetSubTree() const;

  /** Return true if the current node is a leaf */
  virtual bool IsLeaf() const;
  
  /** Return true if the current node is a root */
  virtual bool IsRoot() const;

  /** Get the type of iterator */
  virtual int GetType() const = 0;

  /** Go to the specified child */
  virtual bool GoToChild(int number = 0);
  
  /** Go to the parent */
  virtual bool GoToParent( );

  /** Set the current value of the node */
  void Set(ValueType element);

  /** Return true if the current node has a child */
  virtual bool HasChild(int number = 0) const;

  /** Return the current ChildPosition of an element */
  virtual int ChildPosition(ValueType element) const;

  /** Remove a child */
  virtual bool RemoveChild(int number);

  /** Count the number of children */
  virtual int CountChildren() const;

  /** Return true if the current node has a parent */
  virtual bool HasParent() const;

  /** Disconnect the tree */
  virtual bool Disconnect();

  /** Return a list of children */
  virtual TreeIteratorBase<TTreeType>* Children();

  /** Return a list of parents */
  virtual TreeIteratorBase<TTreeType>* Parents();

  /** Return a list of child */
  virtual TreeIteratorBase<TTreeType>* GetChild(int number) const;

  /** Count the number of nodes */
  virtual int Count();

  /** Remove the current node from the tree */
  bool Remove();

  /** Get the current node */
  virtual TreeNodeType* GetNode();
  virtual const TreeNodeType* GetNode() const;

  /** Get the root */
  TreeNodeType* GetRoot();
  const TreeNodeType* GetRoot() const;
  
  /** Get the tree */
  TTreeType* GetTree() const;

  /** Return the first parent found */
  const TreeNodeType* GetParent() const;

  /** Move an iterator to the beginning of the tree */
  void GoToBegin()
    {
    m_Position = m_Begin;
    }

  /** Move an iterator to the end of the tree. */
  void GoToEnd()
    {
    m_Position = m_End;
    }

  /** Is the iterator at the beginning of the tree? */
  bool IsAtBegin(void) const
    {
    return (m_Position == m_Begin);
    }

  /** Is the iterator at the end of the tree?. The iterator is at the
   * end if it points to NULL */
  bool IsAtEnd(void) const
    {
    return (m_Position == m_End);
    }

  /** Clone the iterator */
  virtual TreeIteratorBase<TTreeType>* Clone() = 0;

  /** Enumerations */
  enum{
    UNDEFIND   = 0,
    PREORDER   = 1,
    INORDER    = 2,
    POSTORDER  = 3,
    LEVELORDER = 4,
    CHILD   = 5,
    ROOT     = 6,
    LEAF     = 7
  };

  /** operator++ */
  Self &
  operator++()
    {
    this->Next();
    return *this;
    }
  /** operator++ */
  void
  operator++(int)
    {
    // assert( !IsAtEnd() );
    this->Next();
    }
  /** operator = */
  const Self & operator=(const Self& iterator) 
    {
    m_Position = iterator.m_Position; 
    m_Begin  = iterator.m_Begin;
    m_End = iterator.m_End;
    m_Root = iterator.m_Root;
    m_Tree = iterator.m_Tree;
    return *this;
    }

  virtual ~TreeIteratorBase() {}
  
protected:

  /** Constructors */
  TreeIteratorBase( TTreeType* tree, const TreeNodeType* start);
  TreeIteratorBase( const TTreeType* tree, const TreeNodeType* start);

  mutable TreeNodeType* m_Position; // Current position of the iterator
  mutable TreeNodeType* m_Begin;
  mutable TreeNodeType* m_End;
  const TreeNodeType*   m_Root;
  TTreeType*            m_Tree;

  virtual bool HasNext() const = 0;
  virtual const ValueType& Next() = 0;
};

} //end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_TreeIteratorBase(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT TreeIteratorBase< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef TreeIteratorBase< ITK_TEMPLATE_1 x > \
                                                  TreeIteratorBase##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkTreeIteratorBase+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkTreeIteratorBase.txx"
#endif

#endif
