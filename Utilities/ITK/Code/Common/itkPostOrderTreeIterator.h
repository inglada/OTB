/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPostOrderTreeIterator.h,v $
  Language:  C++
  Date:      $Date: 2004/12/11 20:29:19 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPostOrderTreeIterator_h
#define __itkPostOrderTreeIterator_h

#include <itkTreeIteratorBase.h>

namespace itk{

template <class TTreeType>
class PostOrderTreeIterator : public TreeIteratorBase<TTreeType> 
{
public:
   
  /** Typedefs */
  typedef TreeIteratorBase<TTreeType>  Superclass;
  typedef TTreeType TreeType;
  typedef typename TTreeType::ValueType ValueType;
  typedef typename Superclass::TreeNodeType TreeNodeType;

  /** Constructor */
  PostOrderTreeIterator(TreeType* tree);

  /** Get the type of the iterator */
  int GetType() const;
  /** Clone function */
  TreeIteratorBase<TTreeType>* Clone();

protected:
  /** Return the next node */
  const ValueType& Next();
  /** Return true if the next node exists */
  bool HasNext() const;
 
protected:

  const TreeNodeType* FindNextNode() const;
  const TreeNodeType* FindMostRightLeaf(TreeNodeType* node) const;
  const TreeNodeType* FindSister(TreeNodeType* node) const;

};

/** Constructor */
template <class TTreeType>
PostOrderTreeIterator<TTreeType>::PostOrderTreeIterator( TTreeType* tree)
    :TreeIteratorBase<TTreeType>(tree,NULL) 
{
  this->m_Position = const_cast<TreeNode<ValueType>*>(tree->GetRoot());

  if ( this->m_Position == NULL )
  {
    this->m_Begin = NULL;
  }
  else
  {
    this->m_Position =const_cast<TreeNodeType* >(FindMostRightLeaf(this->m_Position));
    this->m_Begin = this->m_Position;
  }  
}


/** Return the type of the iterator */
template <class TTreeType>
int 
PostOrderTreeIterator<TTreeType>::GetType() const
{
  return TreeIteratorBase<TTreeType>::POSTORDER;
}


/** Return true if the next node exists */
template <class TTreeType>
bool 
PostOrderTreeIterator<TTreeType>::HasNext() const
{
  if(const_cast<TreeNodeType* >(FindNextNode()) != NULL)
    {
    return true;
    }
  return false;
}

/** Go to the next node */
template <class TTreeType>
const typename PostOrderTreeIterator<TTreeType>::ValueType&
PostOrderTreeIterator<TTreeType>::Next()
{ 
  this->m_Position = const_cast<TreeNodeType*>(FindNextNode());
  return this->m_Position->Get();
}

/** Find the next node */
template <class TTreeType>
const typename PostOrderTreeIterator<TTreeType>::TreeNodeType* 
PostOrderTreeIterator<TTreeType>::FindNextNode() const
{
  if ( this->m_Position == NULL || this->m_Position == this->m_Root )
    {
    return NULL;
    }
  TreeNodeType* sister = const_cast<TreeNodeType*>(FindSister( this->m_Position ));

  if ( sister != NULL )
    {
    return FindMostRightLeaf( sister );
    }

  return this->m_Position->GetParent();
}


/** Find the sister node */
template <class TTreeType>
const typename PostOrderTreeIterator<TTreeType>::TreeNodeType* 
PostOrderTreeIterator<TTreeType>::FindSister( TreeNodeType* node ) const
{
  if ( !node->HasParent() )
    {
    return NULL;
    }

  TreeNodeType* parent = node->GetParent();
  int childPosition = parent->ChildPosition( node );
  int lastChildPosition = parent->CountChildren() - 1;

  while ( childPosition < lastChildPosition ) 
    {
    TreeNodeType* sister = parent->GetChild( childPosition + 1);

    if ( sister != NULL )
      {
      return sister;
      }
    childPosition++;
    }
  return NULL;
}

/** Find the most right leaf */
template <class TTreeType>
const typename PostOrderTreeIterator<TTreeType>::TreeNodeType* 
PostOrderTreeIterator<TTreeType>::FindMostRightLeaf(TreeNodeType* node) const
{
 while ( node->HasChildren() ) 
   {
   TreeNodeType* helpNode;
   int childCount = node->CountChildren();
   int i = 0;

   do 
     {
     helpNode = node->GetChild( i );
     i++;
     } while ( helpNode == NULL && i < childCount );

   if ( helpNode == NULL )
     {
     return node;
     }
   node = helpNode;
   }
  return node;
}

/** Clone function */
template <class TTreeType>
TreeIteratorBase<TTreeType>* PostOrderTreeIterator<TTreeType>::Clone() 
{
  PostOrderTreeIterator<TTreeType>* clone = new PostOrderTreeIterator<TTreeType>( const_cast<TTreeType*>(this->m_Tree) );
  *clone = *this;
  return clone;
}

} // end namespace itk

#endif

