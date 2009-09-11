/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelObject.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-23 23:52:55 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelObject_txx
#define __itkLabelObject_txx

#include "itkLabelObject.h"
#include "itkLabelObjectLineComparator.h"
#include "vnl/vnl_math.h"
#include <algorithm>

namespace itk
{

template < class TLabel, unsigned int VImageDimension >
LabelObject<TLabel, VImageDimension>::LabelObject()
{
  m_Label = NumericTraits<LabelType>::Zero;
  m_LineContainer.clear();
}

template < class TLabel, unsigned int VImageDimension >
typename LabelObject<TLabel, VImageDimension>::AttributeType 
LabelObject<TLabel, VImageDimension>::GetAttributeFromName( const std::string & s )
{
  if( s == "Label" )
    {
    return LABEL;
    }
  // can't recognize the name
  throw std::runtime_error("Unknown attribute.");
}

template < class TLabel, unsigned int VImageDimension >
std::string 
LabelObject<TLabel, VImageDimension>
::GetNameFromAttribute( const AttributeType & a )
{
  switch( a )
    {
    case LABEL:
    return "Label";
    break;
    }
  // can't recognize the namespace
  throw std::runtime_error("Unknown attribute.");
}

/**
 * Set/Get the label associated with that object.
 */
template < class TLabel, unsigned int VImageDimension >
const typename LabelObject<TLabel, VImageDimension>::LabelType & 
LabelObject<TLabel, VImageDimension>::GetLabel() const
{
  return m_Label;
}

template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::SetLabel( const LabelType & label )
{
  m_Label = label;
}

/**
 * Return true if the object contain the given index and false otherwise.
 * Worst case complexity is O(L) where L is the number of lines in the object.
 */
template < class TLabel, unsigned int VImageDimension >
bool 
LabelObject<TLabel, VImageDimension>::
HasIndex( const IndexType & idx ) const
{
  for( typename LineContainerType::const_iterator it=m_LineContainer.begin();
    it != m_LineContainer.end();
    it++ )
    {
    if( it->HasIndex( idx ) )
      {
      return true;
      }
    }
  return false;
}

/**
 * Add an index to the object. If the index is already in the object, the index can
 * be found several time in the object.
 */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::
AddIndex( const IndexType & idx ) 
{
  itkAssertOrThrowMacro( (!this->HasIndex( idx )), "Index out of range" );

  if( !m_LineContainer.empty() )
    {
    // can we use the last line to add that index ?
    LineType & lastLine = * m_LineContainer.rbegin();
    if( lastLine.IsNextIndex( idx ) )
      {
      lastLine.SetLength( lastLine.GetLength() + 1 );
      return;
      }
    }
  // create a new line
  this->AddLine( idx, 1 );
}

/**
 * Add a new line to the object, without any check.
 */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::
AddLine( const IndexType & idx, const LengthType & length )
{
  LineType line( idx, length );
  this->AddLine( line );
}

/**
 * Add a new line to the object, without any check.
 */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::
AddLine( const LineType & line )
{
  // TODO: add an assert to be sure that some indexes in the line are not already stored here
  m_LineContainer.push_back( line );
}
  
/** Return the line container of this object */
template < class TLabel, unsigned int VImageDimension >
const 
typename LabelObject<TLabel, VImageDimension>::LineContainerType & 
LabelObject<TLabel, VImageDimension>::GetLineContainer() const
{
  return m_LineContainer;
}

template < class TLabel, unsigned int VImageDimension >
typename LabelObject<TLabel, VImageDimension>::LineContainerType & 
LabelObject<TLabel, VImageDimension>::GetLineContainer()
{
  return m_LineContainer;
}

template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::SetLineContainer( const LineContainerType & lineContainer )
{
  m_LineContainer = lineContainer;
}

template < class TLabel, unsigned int VImageDimension >
int 
LabelObject<TLabel, VImageDimension>::GetNumberOfLines() const
{
  return m_LineContainer.size();
}

template < class TLabel, unsigned int VImageDimension >
const 
typename LabelObject<TLabel, VImageDimension>::LineType & 
LabelObject<TLabel, VImageDimension>::GetLine( int i ) const
{
  return m_LineContainer[i];
}
  
template < class TLabel, unsigned int VImageDimension >
typename LabelObject<TLabel, VImageDimension>::LineType & 
LabelObject<TLabel, VImageDimension>::GetLine( int i )
{
  return m_LineContainer[i];
}

template < class TLabel, unsigned int VImageDimension >
int 
LabelObject<TLabel, VImageDimension>::Size() const
{
  int size = 0;
  for( typename LineContainerType::const_iterator it=m_LineContainer.begin();
    it != m_LineContainer.end();
    it++ )
    {
    size += it->GetLength();
    }
  return size;
}
  
template < class TLabel, unsigned int VImageDimension >
typename LabelObject<TLabel, VImageDimension>::IndexType 
LabelObject<TLabel, VImageDimension>::GetIndex( int offset ) const
{
  int o = offset;
  for( typename LineContainerType::const_iterator it=m_LineContainer.begin();
    it != m_LineContainer.end();
    it++ )
    {
    int size = it->GetLength();
    if( o > size)
      {
      o -= size;
      }
    else
      {
      IndexType idx = it->GetIndex();
      idx[0] += o;
      return idx;
      }
    }
  itkGenericExceptionMacro(<< "Invalid offset: " << offset);
}
  
/** Copy the attributes of another node to this one */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::CopyAttributesFrom( const Self * src )
{
  itkAssertOrThrowMacro ( (src != NULL), "Null Pointer" );
  // nothing to do here - this class has no attribute
}
   
/** Copy the lines, the label and the attributes from another node. */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::CopyAllFrom( const Self * src )
{
  itkAssertOrThrowMacro ( (src != NULL), "Null Pointer" );
  m_LineContainer = src->m_LineContainer;
  m_Label = src->m_Label;
  // also copy the attributes
  this->CopyAttributesFrom( src );
}
    
/** Reorder the lines, merge the touching lines and ensure that no
 * pixel is covered by two lines
 */
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::Optimize()
{
  if( ! m_LineContainer.empty() )
    {
    // first copy the lines in another container and clear the current one
    LineContainerType lineContainer = m_LineContainer;
    m_LineContainer.clear();
    
    // reorder the lines
    typename Functor::LabelObjectLineComparator< LineType > comparator;
    std::sort( lineContainer.begin(), lineContainer.end(), comparator );
    
    // then check the lines consistancy
    // we'll proceed line index by line index
    IndexType currentIdx = lineContainer.begin()->GetIndex();
    long int currentLength = lineContainer.begin()->GetLength();
    
    for( typename LineContainerType::const_iterator it=lineContainer.begin(); 
         it != lineContainer.end();
         it++ )
      {
      const LineType & line = *it;
      IndexType idx = line.GetIndex();
      unsigned long length = line.GetLength();
      
      // check the index to be sure that we are still in the same line idx
      bool sameIdx = true;
      for( int i=1; i<ImageDimension; i++ )
        {
        if( currentIdx[i] != idx[i] )
          {
          sameIdx = false;
          }
        }
      
      // try to extend the current line idx, or create a new line
      if( sameIdx && currentIdx[0] + currentLength >= idx[0] )
        {
        // we may expand the line
        long int newLength = idx[0] + length - currentIdx[0];
        currentLength = vnl_math_max( newLength, currentLength );
        }
      else
        {
        // add the previous line to the new line container and use the new line index and size
        // std::cout << currentIdx << "  " << currentLength << std::endl;
        this->AddLine( currentIdx, currentLength );
        currentIdx = idx;
        currentLength = length;
        }
      // std::cout << line.GetIndex() << "  " << line.GetLength() << std::endl;
      }
    // complete the last line
    // std::cout << currentIdx << "  " << currentLength << std::endl;
    this->AddLine( currentIdx, currentLength );
    }
}
  
template < class TLabel, unsigned int VImageDimension >
void 
LabelObject<TLabel, VImageDimension>::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "LineContainer: " << & m_LineContainer << std::endl;
  os << indent << "Label: " << static_cast<typename NumericTraits<LabelType>::PrintType>(m_Label) << std::endl; 
}

} // end namespace itk

#endif
