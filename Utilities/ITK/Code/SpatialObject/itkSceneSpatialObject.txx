/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSceneSpatialObject.txx,v $
  Language:  C++
  Date:      $Date: 2005/12/06 19:42:43 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __SceneSpatialObject_txx
#define __SceneSpatialObject_txx

#include "itkSceneSpatialObject.h"
#include <algorithm>

namespace itk
{
  
/** Constructor */
template <unsigned int SpaceDimension>
SceneSpatialObject<SpaceDimension>
::SceneSpatialObject()
{
}

/** Destructor */
template <unsigned int SpaceDimension>
SceneSpatialObject<SpaceDimension>
::~SceneSpatialObject()
{
}

/** Add a spatial object to the SceneSpatialObject */
template <unsigned int SpaceDimension>
void 
SceneSpatialObject<SpaceDimension>
::AddSpatialObject( SpatialObject<SpaceDimension> * pointer )
{
  m_Objects.push_back( pointer );
  this->Modified();
}

/** Remove a spatial object from the SceneSpatialObject */
template <unsigned int SpaceDimension>
void 
SceneSpatialObject<SpaceDimension>
::RemoveSpatialObject( SpatialObject<SpaceDimension> * pointer )
{
  typename ObjectListType::iterator it;    
  it = std::find(m_Objects.begin(),m_Objects.end(),pointer);

  if( it != m_Objects.end() )
    {
    if( *it == pointer )
      {
      m_Objects.erase( it );
      this->Modified();
      }
    }
  else
    { 
    //throw an exception object to let user know that he tried to remove an object
    // which is not in the list of the children.
    }
}


/** Return the modification time of the SceneSpatialObject */
template <unsigned int SpaceDimension>
unsigned long
SceneSpatialObject<SpaceDimension>
::GetMTime( void ) const
{
  typename ObjectListType::const_iterator it = m_Objects.begin();
  typename ObjectListType::const_iterator itEnd = m_Objects.end();
 
  unsigned long latestTime = Superclass::GetMTime();
  unsigned long localTime;
  while(it!=itEnd)
    {
    localTime = (*it)->GetMTime();
    if( localTime > latestTime )
      {
      latestTime = localTime;
      }
    it++;
    } 
  return latestTime;
}

/** Returns a new list of objects in the scene */
template <unsigned int SpaceDimension>
typename SceneSpatialObject<SpaceDimension>::ObjectListType *
SceneSpatialObject<SpaceDimension>
::GetObjects( unsigned int depth, char * name )
{
  ObjectListType * newList = new ObjectListType;

  typename ObjectListType::const_iterator it = m_Objects.begin();
  typename ObjectListType::const_iterator itEnd = m_Objects.end();

  while(it != itEnd)
    {
    if(name == NULL || strstr(typeid(**it).name(), name))
      {
      newList->push_back(*it);
      }
    if(depth>0)
      {
      typedef typename SpatialObject<SpaceDimension>::ChildrenListType
        ChildListType;
      ChildListType * childList = 
        dynamic_cast<SpatialObject<SpaceDimension> *>((*it).GetPointer())->
        GetChildren(depth-1, name);
      typename ChildListType::const_iterator cIt = childList->begin();
      typename ChildListType::const_iterator cItEnd = childList->end();

      while(cIt != cItEnd)
        {
        newList->push_back(dynamic_cast< ObjectType * >((*cIt).GetPointer()));
        cIt++;
        }

      delete childList;
      }
    it++;
    }

  return newList;
}

/** Set the children list */
template <unsigned int SpaceDimension>
void
SceneSpatialObject<SpaceDimension>
::SetObjects( ObjectListType & children )
{ 
  m_Objects = children;
}

/** Return the number of objects in the SceneSpatialObject */
template <unsigned int SpaceDimension>
unsigned int
SceneSpatialObject<SpaceDimension>
::GetNumberOfObjects( unsigned int depth, char * name )
{
  typename ObjectListType::const_iterator it = m_Objects.begin();
  typename ObjectListType::const_iterator itEnd = m_Objects.end();

  unsigned int cnt = 0;
  while(it != itEnd)
    {
    if(name == NULL || strstr(typeid(**it).name(), name))
      {
      cnt++;
      }
    it++;
    }

  it = m_Objects.begin();
  itEnd = m_Objects.end();
  if( depth > 0 )
    {
    while(it != itEnd)
      {
      cnt += (dynamic_cast<SpatialObject<SpaceDimension> * >((*it).GetPointer()))->
        GetNumberOfChildren( depth-1, name );
      it++;
      }
    }

  return cnt;
} 

/** Print the object */
template <unsigned int SpaceDimension>
void
SceneSpatialObject<SpaceDimension>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  os << indent << "Number of objects: " 
     << m_Objects.size() << std::endl;
  os << indent << "List of objects: ";

  typename ObjectListType::const_iterator it = m_Objects.begin();
  typename ObjectListType::const_iterator itEnd = m_Objects.end();

  while(it != itEnd)
    {
    os << "[" << (*it) << "] ";
    it++;
    }
  os << std::endl;

  Superclass::PrintSelf(os, indent);
}

/** Return a SpatialObject in the SceneSpatialObject 
 *  given a parent ID */
template <unsigned int SpaceDimension>
SpatialObject<SpaceDimension> *
SceneSpatialObject<SpaceDimension>
::GetObjectById(int Id)
{
  typename ObjectListType::iterator it = m_Objects.begin();
  typename ObjectListType::iterator itEnd = m_Objects.end();

  typedef typename SpatialObjectType::ChildrenListType ChildListType;
  ChildListType * cList;
  typename ChildListType::iterator cIt;
  typename ChildListType::iterator cItEnd;
    
  while( it != itEnd )
    {
    if( (*it)->GetId() == Id )
      { 
      return *it;
      }
    else
      {
      //cList = (dynamic_cast<SpatialObject<SpaceDimension> *>(*it))->
      //  GetChildren(SpatialObjectType::MaximumDepth); 
      cList = (*it)->GetChildren(SpatialObjectType::MaximumDepth); 
      cIt = cList->begin();
      cItEnd = cList->end();
      while(cIt != cItEnd)
        {
        if( (*cIt)->GetId() == Id )
          {
          SpatialObject<SpaceDimension> * tmp;
          tmp = *cIt;
          delete cList;
          return tmp;
          }
        cIt++;
        }

      delete cList;
      }

    it++;
    }

  return NULL;
}
template <unsigned int SpaceDimension>
bool
SceneSpatialObject<SpaceDimension>
::FixHierarchy(void)
{
  typename ObjectListType::iterator it = m_Objects.begin();
  typename ObjectListType::iterator oldIt;
  typename ObjectListType::iterator itEnd = m_Objects.end();

  bool ret = true;
  while( it != itEnd)
    {
    int pID = (*it)->GetParentId();
    if(pID >= 0)
      {
      SpatialObject<SpaceDimension> * pObj =
        static_cast<SpatialObject<SpaceDimension> * >
        (this->GetObjectById(pID));
      if(pObj == NULL)
        {
        ret = false;
        it++;
        }
      else
        {
        pObj->AddSpatialObject(dynamic_cast<SpatialObject<SpaceDimension> *>
                               ((*it).GetPointer()));
        oldIt = it;
        it++;
        m_Objects.erase( oldIt );
        }
      }
    else
      {
      it++;
      }
    }

  return ret;
}



/** Check if the parent objects have a defined ID */
template <unsigned int SpaceDimension>
bool
SceneSpatialObject<SpaceDimension>
::CheckIdValidity(void)
{
  typename ObjectListType::iterator it = m_Objects.begin();
  typename ObjectListType::iterator itEnd = m_Objects.end();

  bool ret = true;
  while( it != itEnd)
    {
    // For every object in the scene we check the ID validity
    typename ObjectType::ChildrenListType* children = (*it)->GetChildren();
    typename ObjectType::ChildrenListType::const_iterator itChild = children->begin();
    
    while( itChild != children->end())
      {
      if( (*itChild)->HasParent())
        {
        if((*itChild)->GetParent()->GetId()<0)
          {
          delete children;
          return false;
          }
        }
      itChild++;
      }
    delete children;
    it++;
    }
  return ret;
}

template <unsigned int SpaceDimension>
void
SceneSpatialObject<SpaceDimension>
::FixIdValidity(void)
{
  typename ObjectListType::iterator it = m_Objects.begin();
  typename ObjectListType::iterator itEnd = m_Objects.end();

  while( it != itEnd)
    {
    // For every object in the scene we check the ID validity
    typename ObjectType::ChildrenListType* children = (*it)->GetChildren();
    typename ObjectType::ChildrenListType::iterator itChild = children->begin();
    
    while( itChild != children->end())
      {
      if( (*itChild)->HasParent())
        {
        if((*itChild)->GetParent()->GetId()<0)
          {
          (*itChild)->GetParent()->SetId(this->GetNextAvailableId());
          }
        }
      itChild++;
      }
    delete children;
    it++;
    }
}


/** Return the next available Id. For speed reason the MaxID+1 is returned*/
template <unsigned int SpaceDimension>
int
SceneSpatialObject<SpaceDimension>
::GetNextAvailableId()
{
  int Id = 0;

  typename ObjectListType::iterator it = m_Objects.begin();
  typename ObjectListType::iterator itEnd = m_Objects.end();

  while( it != itEnd)
    {
    typename ObjectType::ChildrenListType* children = (*it)->GetChildren();
    typename ObjectType::ChildrenListType::iterator itChild = children->begin();
    
    while( itChild != children->end())
      {
      if((*itChild)->GetId() >= Id)
        {
        Id = (*itChild)->GetId()+1;
        }
      itChild++;
      }
    delete children;
    it++;
    }
  return Id;
}

/** Clear function : Remove all the objects in the scene */
template <unsigned int SpaceDimension>
void
SceneSpatialObject<SpaceDimension>
::Clear()
{
  m_Objects.clear();
  this->Modified();
}


} // end of namespace itk 

#endif
