/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTreeIteratorBase.txx,v $
  Language:  C++
  Date:      $Date: 2009-06-14 11:55:05 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTreeIteratorBase_txx
#define __itkTreeIteratorBase_txx

#include <itkTreeIteratorBase.h>
#include "itkTreeChangeEvent.h"

namespace itk
{

/** Constructor */
template <class TTreeType>
TreeIteratorBase<TTreeType>::TreeIteratorBase( TTreeType* tree, const TreeNodeType* start)
{ 
  if(start)
    {
    m_Root = start;
    } 
  else
    {
    m_Root = dynamic_cast<const TreeNodeType*>(tree->GetRoot());
    }

  m_Position = const_cast<TreeNodeType*>(m_Root);
  m_Tree = tree;
  m_Begin = m_Position;
  m_End = NULL;
}

/** Constructor */
template <class TTreeType>
TreeIteratorBase<TTreeType>::TreeIteratorBase( const TTreeType* tree, const TreeNodeType* start)
{
  if(start)
    {
    m_Root = start;
    } 
  else
    {
    m_Root = const_cast<TreeNodeType*>(dynamic_cast<const TreeNodeType*>(tree->GetRoot()));
    }
  m_Position = const_cast<TreeNodeType*>(m_Root);
  m_Tree = const_cast<TTreeType*>(tree);
  m_Begin = m_Position;
  m_End = NULL;
}

/** Return the current value of the node */
template <class TTreeType>
const typename TreeIteratorBase<TTreeType>::ValueType& 
TreeIteratorBase<TTreeType>::Get() const 
{
  return m_Position->Get();
}

/** Set the current value of the node */
template <class TTreeType>
void
TreeIteratorBase<TTreeType>::Set(ValueType element) 
{
//  assert(m_Position);
  m_Position->Set(element);
  m_Tree->Modified();
  m_Tree->InvokeEvent( TreeNodeChangeEvent<TTreeType>(*this) );  
}

/** Add a value to the node. This creates a new child node */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::Add(ValueType element)
{
  if ( m_Position == NULL && m_Root == NULL ) 
    {
    bool returnValue = false;  
    if ( m_Tree ) 
      {
      returnValue = const_cast<TTreeType*>(m_Tree)->SetRoot( element );
      }
    // signal AddEvent for self
    m_Root = dynamic_cast<const TreeNodeType*>(const_cast<TTreeType*>(m_Tree)->GetRoot());
    m_Position =  const_cast<TreeNodeType*>(m_Root);
    m_Tree->Modified();
    m_Tree->InvokeEvent( TreeAddEvent<TTreeType>(*this) );
    return returnValue;
    } 
  else if ( m_Position == NULL )
    {
    return false;
    }
  
  typename TreeNodeType::Pointer node = TreeNodeType::New();
  node->Set(element);
  m_Position->AddChild( node );
  m_Tree->Modified();

  // signal AddEvent for new child
  TreeIteratorBase<TTreeType>* childIterator = Clone();
  childIterator->m_Position = dynamic_cast<TreeNodeType*>( m_Position->GetChild( m_Position->ChildPosition(node)) );
  m_Tree->InvokeEvent( TreeAddEvent<TTreeType>(*childIterator) );         // signal "child has been added deleted"
  delete childIterator;

  return true;
}

/** Add a new element at a given position */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::Add( int itkNotUsed(childPosition), ValueType element )
{
  if ( m_Position ) 
    {
    typename TreeNodeType::Pointer node = TreeNodeType::New();
    node->Set(element);
    m_Position->AddChild(node);
    m_Tree->Modified();
    
    // signal AddEvent
    TreeIteratorBase<TTreeType>* childIterator = Clone();
    childIterator->m_Position = dynamic_cast<TreeNodeType*>( m_Position->GetChild( m_Position->ChildPosition(node)) );
    m_Tree->InvokeEvent( TreeAddEvent<TTreeType>(*childIterator) );         // signal "child has been added deleted"
    delete childIterator;
    
    return true;
    }
  return false;
}

/** Return true if the current pointed node is a leaf */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::IsLeaf() const 
{
  return !(m_Position->HasChildren());
} 

/** Return true if the current pointed node is a root */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::IsRoot( ) const 
{
  if ( m_Root == NULL )
    {
    return false;
    }
    
  if ( m_Position == m_Root )
    {
    return true;
    }
  return false;
}

/** Add a subtree  */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::Add( TTreeType& subTree ) 
{
  if ( subTree.Count() == 0 )
    {
    return false;
    }

  if(!subTree.GetRoot())
    {
    return false;
    }
  
  if ( m_Root == NULL ) 
    {
    m_Root = static_cast<const TreeNodeType*>(subTree.GetRoot());
    } 
  else 
    {
    if ( m_Position == NULL )
      {
      return false;
      }
    m_Position->AddChild( const_cast<TreeNodeType*>(static_cast<const TreeNodeType*>(subTree.GetRoot())) );
    }
  return true;
}

/** Return the subtree */
template <class TTreeType>
TTreeType*
TreeIteratorBase<TTreeType>::GetSubTree() const 
{
  typename TTreeType::Pointer tree = TTreeType::New();
  tree->SetRoot(m_Position);
  tree->SetSubtree(true);
  return tree;
}

/** Return true of the current node has a child */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::HasChild( int number ) const
{
  if ( m_Position == NULL )
    {
    return false;
    }
  if ( m_Position->GetChild( number ) != NULL )
    {
    return true;
    }
  return false;
}

/** Return the current position of the child */
template <class TTreeType>
int 
TreeIteratorBase<TTreeType>::ChildPosition( ValueType element ) const 
{
  if ( !m_Position )
    {
    return -1;
    }
  return m_Position->ChildPosition( element );
}

/** Remove a child */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::RemoveChild( int number ) 
{
  if( !HasChild( number ) )
    {
    return false;
    }
  TreeNodeType* child = dynamic_cast<TreeNodeType*>(m_Position->GetChild( number ));

  if( child != NULL ) 
    {
    // signal PruneEvent (node plus all children are removed)
    TreeIteratorBase<TTreeType>* childIterator = Clone();
    childIterator->m_Position = child;
    m_Tree->InvokeEvent( TreePruneEvent<TTreeType>(*childIterator) );         // signal "child has been added deleted"
    delete childIterator;
      
    const_cast<TreeNodeType*>(m_Position)->Remove( child );     // and really remove child (and subitems)
    m_Tree->Modified();
    return true;
    }
  return false;
}

/** Count the number of children */
template <class TTreeType>
int 
TreeIteratorBase<TTreeType>::CountChildren() const 
{
  if ( m_Position == NULL )
    {
    return -1;
    }
  return m_Position->CountChildren( );
}

/** Return true of the pointed node has a parent */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::HasParent( ) const 
{
  return ( m_Position != NULL && m_Position->GetParent() != NULL);
}

/** Disconnect the tree */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::Disconnect()
{
  if ( m_Position == NULL )
    {
    return false;
    }

  if( m_Position->HasParent() == false )
    {
    return false;
    }
  
  //keep node alive just a bit longer
  typename TreeNodeType::Pointer position = m_Position;
 
  TreeNodeType* parent = dynamic_cast<TreeNodeType*>(m_Position->GetParent());
  parent->Remove( const_cast<TreeNodeType*>(m_Position) );
  m_Tree->Modified();

  while (m_Position->CountChildren() > 0) 
    {
    TreeNodeType* child = dynamic_cast<TreeNodeType*>(m_Position->GetChild(0));     // always add first child in list, because AddChild() removes the added node from its former parent (== m_position)
    parent->AddChild( child );
    }
  
  m_Tree->InvokeEvent( TreeRemoveEvent<TTreeType>( *this ) );

  m_Position = NULL;
  return true;
}

/** Return the children list */
template <class TTreeType>
TreeIteratorBase<TTreeType>* 
TreeIteratorBase<TTreeType>::Children() 
{
  itkGenericOutputMacro("Not implemented yet");
  ::itk::ExceptionObject e_(__FILE__, __LINE__, "Not implemented yet", ITK_LOCATION);
  throw e_; /* Explicit naming to work around Intel compiler bug.  */
  return 0;
}


/** Return the first parent found */
template <class TTreeType>
const typename TreeIteratorBase<TTreeType>::TreeNodeType* 
TreeIteratorBase<TTreeType>::GetParent() const
{
  if(m_Position == NULL)
    {
    return NULL;
    }
 
  return m_Position->GetParent();
}

/** Return the list of parents */
template <class TTreeType>
TreeIteratorBase<TTreeType>* TreeIteratorBase<TTreeType>::Parents() 
{
  itkGenericOutputMacro("Not implemented yet");
  ::itk::ExceptionObject e_(__FILE__, __LINE__, "Not implemented yet",ITK_LOCATION);
  throw e_; /* Explicit naming to work around Intel compiler bug.  */ 
  return 0;
}

/** Go to a child */
template <class TTreeType>
bool TreeIteratorBase<TTreeType>::GoToChild( int number ) 
{
  if ( m_Position == NULL )
    {
    return false;
    }

  TreeNodeType* next = dynamic_cast<TreeNodeType*>(m_Position->GetChild( number ));

  if ( next == NULL )
    {
    return false;
    }
  m_Position = next;
  return true;
}

/** Go to a parent */
template <class TTreeType>
bool TreeIteratorBase<TTreeType>::GoToParent( ) 
{
  if ( m_Position == NULL )
    {
    return false;
    }

  if( !m_Position->HasParent() ) 
    {
    return false;
    }

  m_Position = dynamic_cast<TreeNodeType*>(m_Position->GetParent());
  return true;
}

/** Get a child given a number */
template <class TTreeType>
TreeIteratorBase<TTreeType>* TreeIteratorBase<TTreeType>::GetChild( int number ) const 
{
  if ( !m_Position )
    {
    return NULL;
    }
    
  TreeNodeType* child = dynamic_cast<TreeNodeType*>(m_Position->GetChild( number ));

  if ( !child )
    {
    return NULL;
    }
//    return new WalkTreeIterator<ValueType,P>( child, m_Root, m_Tree, getType() );
  return NULL;
}

/** Count the number of nodes from the beginning */
template <class TTreeType>
int TreeIteratorBase<TTreeType>::Count()
{
  int size = 0;
  this->GoToBegin();
  if ( !m_Position->HasChildren() )
    {
    return 0;
    }
  while (this->Next() ) 
    {
    size++;
    }
  return size;
}

/** Get the node pointed by the iterator */
template <class TTreeType>
typename TreeIteratorBase<TTreeType>::TreeNodeType* 
TreeIteratorBase<TTreeType>::GetNode() 
{
  return const_cast<TreeNodeType*>(m_Position);
}

/** Get the node pointed by the iterator */
template <class TTreeType>
const typename TreeIteratorBase<TTreeType>::TreeNodeType* 
TreeIteratorBase<TTreeType>::GetNode() const
{
  return m_Position;
}

/** Get the root */
template <class TTreeType>
typename TreeIteratorBase<TTreeType>::TreeNodeType*
TreeIteratorBase<TTreeType>::GetRoot() 
{
  return const_cast<TreeNodeType*>(m_Root);
}

/** Get the root (const) */
template <class TTreeType>
const typename TreeIteratorBase<TTreeType>::TreeNodeType*
TreeIteratorBase<TTreeType>::GetRoot() const
{
  return m_Root;
}

/** Remove a specific node (and its child nodes!) */
template <class TTreeType>
bool 
TreeIteratorBase<TTreeType>::Remove() 
{
  if ( m_Position == NULL )
    {
    return false;
    }
    
  //keep node alive just a bit longer (for the notification)
  typename TreeNodeType::Pointer position = m_Position;

  if ( m_Position->HasParent() )
    {
    TreeNodeType* parent = m_Position->GetParent();
    parent->Remove( m_Position );                        // removes this node (and implicitly all children, too)
    }
  else if (m_Root == m_Position)
    {
    m_Root = NULL;
    m_Tree->SetRoot((TreeNodeType*)NULL); // this won't do anything if root is already != NULL  ==> root cannot be removed
    }
    
  m_Position->SetParent(NULL); // we don't have a parent anymore
  m_Tree->InvokeEvent( TreePruneEvent<TTreeType>(*this) );
  while (m_Position->CountChildren() > 0)  // remove all children
    {
    //always remove first child (id 0)
    TreeNodeType* child = dynamic_cast<TreeNodeType*>(m_Position->GetChild(0));
    m_Position->Remove( child );
    }

  position = NULL;
  m_Position = NULL;  // Smart pointer, deletes *m_Position
  
  m_Tree->Modified();

  return true;
}

/** Return the tree */
template <class TTreeType>
TTreeType* 
TreeIteratorBase<TTreeType>::GetTree() const
{
  return m_Tree;
}


} // namespace itk

#endif
