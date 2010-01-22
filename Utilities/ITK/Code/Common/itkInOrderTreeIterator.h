/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkInOrderTreeIterator.h,v $
  Language:  C++
  Date:      $Date: 2009-02-05 19:04:57 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkInOrderTreeIterator_h
#define __itkInOrderTreeIterator_h

#include <itkTreeIteratorBase.h>

namespace itk {

template <class TTreeType>
class InOrderTreeIterator : public TreeIteratorBase<TTreeType> 
{
public:

  /** Typedefs */
  typedef InOrderTreeIterator                 Self;
  typedef TreeIteratorBase<TTreeType>         Superclass;
  typedef TTreeType                           TreeType;
  typedef typename TTreeType::ValueType       ValueType;
  typedef typename Superclass::TreeNodeType   TreeNodeType;

  /** Constructors */
  InOrderTreeIterator( TreeType& start );
  InOrderTreeIterator( TreeType* tree, TreeNodeType* start=NULL);

  /** Get the type of iterator */
  int GetType() const;
  
  /** Clone function */
  TreeIteratorBase<TTreeType>* Clone();

protected:

  /** Return the next node */
  const ValueType& Next();

  /** Return true if the next node exists */
  bool HasNext() const;

private:

  /** Find the next node */
  const TreeNodeType* FindNextNode() const;

};


/** Constructor */
template <class TTreeType>
InOrderTreeIterator<TTreeType>::InOrderTreeIterator( TTreeType& start )
    :TreeIteratorBase<TTreeType>(start) 
{
}


/** Constructor */
template <class TTreeType>
InOrderTreeIterator<TTreeType>::InOrderTreeIterator( TTreeType* tree, TreeNodeType* start)
    :TreeIteratorBase<TTreeType>(tree,start)
{
}

/** Get the type of the iterator */
template <class TTreeType>
int 
InOrderTreeIterator<TTreeType>::GetType() const 
{
  return TreeIteratorBase<TTreeType>::INORDER;
}


/** Return true if the next node exists */
template <class TTreeType>
bool InOrderTreeIterator<TTreeType>::HasNext() const
{
  if ( const_cast<TreeNodeType*>(FindNextNode()) != NULL )
    {
    return true;
    }
  return false;
}


/** Return the next node */
template <class TTreeType>
const typename InOrderTreeIterator<TTreeType>::ValueType&
InOrderTreeIterator<TTreeType>::Next() 
{
  this->m_Position =  const_cast<TreeNodeType* >(FindNextNode());
  return this->m_Position->Get();
}


/** Find the next node */
template <class TTreeType>
const typename InOrderTreeIterator<TTreeType>::TreeNodeType* 
InOrderTreeIterator<TTreeType>::FindNextNode() const 
{
  if ( this->m_Position == NULL )
    {
    return NULL;
    }
  
  if ( this->m_Position->HasChildren() )
    {
    return this->m_Position->GetChild(0);
    }
    
  if ( !this->m_Position->HasParent() )
    {
    return NULL;
    }
  
  TreeNodeType* child = this->m_Position;
  TreeNodeType* parent = this->m_Position->GetParent();

  int childPosition = parent->ChildPosition( child );
  int lastChildPosition = parent->CountChildren() - 1;

  while ( childPosition < lastChildPosition ) 
    {
    TreeNodeType* help = parent->GetChild( childPosition + 1 );
    if ( help != NULL )
      {
      return help;
      }
    childPosition++;
    }

  while ( parent->HasParent() )
    {
    child = parent;
    parent = parent->GetParent();

    // Subtree
    if( parent->ChildPosition( this->m_Root ) >= 0 )
      {
      return NULL;
      }
    childPosition = parent->ChildPosition(child);
    lastChildPosition = parent->CountChildren() - 1;

    while ( childPosition < lastChildPosition ) 
      {
      TreeNodeType* help = parent->GetChild( childPosition + 1 );
      if ( help != NULL )
        {
        return help;
        }
      }
    }
  return NULL;
}

/** Clone function */
template <class TTreeType>
TreeIteratorBase<TTreeType>* InOrderTreeIterator<TTreeType>::Clone() 
{
  InOrderTreeIterator* clone = new InOrderTreeIterator( const_cast<TTreeType*>(this->m_Tree) );
  *clone = *this;
  return clone;
}

} // end namespace itk

#endif
