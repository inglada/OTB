/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSceneSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2006/08/29 23:28:47 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
 
#ifndef __SceneSpatialObject_h 
#define __SceneSpatialObject_h 

#include "itkSpatialObject.h"  
 
#include <list>

namespace itk 
{ 

/** \class SceneSpatialObject
* \brief a SceneSpatialObject has a list of SpatialObjects
*
* This class represent a SceneSpatialObject object into which one can
* plug any kind of spatial object.
*
* \sa SpatialObject
*/ 
 
template <unsigned int SpaceDimension = 3>
class SceneSpatialObject 
  : public Object
{ 
 
public: 

  typedef SceneSpatialObject<SpaceDimension> Self; 
  typedef Object Superclass; 
  typedef Superclass::Pointer SuperclassPointer; 
  typedef SmartPointer< Self > Pointer; 
  typedef SmartPointer< const Self > ConstPointer; 

  typedef SpatialObject<SpaceDimension> ObjectType;
  typedef SpatialObject<SpaceDimension> SpatialObjectType;
  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  typedef std::list< SpatialObjectPointer > 
  ObjectListType; 
     
  itkStaticConstMacro(MaximumDepth, unsigned int, 9999999);

  /** Method for creation through the object factory */ 
  itkNewMacro(Self); 
  itkTypeMacro(SceneSpatialObject, Object); 

  /** Add an object to the list of children. */ 
  void AddSpatialObject( SpatialObject<SpaceDimension> * pointer ); 
     
  /** Remove the object passed as arguments from the list of 
   *  children. May this function 
   *  should return a false value if the object to remove is 
   *  not found in the list. */ 
  void RemoveSpatialObject( SpatialObject<SpaceDimension> * object ); 

  /** Returns a list of pointer to the objects in the scene.
   *  \warning This function creates a new list therefore the user is responsible
   *  for the deletion of the list. */ 
  ObjectListType * GetObjects( unsigned int depth=MaximumDepth,
                               char * name=NULL );

  /** Returns the number of children currently assigned to the SceneSpatialObject object.*/ 
  unsigned int GetNumberOfObjects( unsigned int depth=MaximumDepth,
                                   char * name=NULL ); 

  /** Set the list of pointers to children to the list passed as argument.*/ 
  void SetObjects( ObjectListType & children ); 

  /** Returns the latest modified time of all the objects contained in this SceneSpatialObject
   *  object. */
  unsigned long GetMTime( void ) const;

  /** Get/Set the ParentID */
  void SetParentId(int parentid) {m_ParentId=parentid;}
  int  GetParentId(void) {return m_ParentId;}

  /** Return a SpatialObject in the SceneSpatialObject given its ID */
  SpatialObject<SpaceDimension> * GetObjectById(int Id);

  bool FixHierarchy(void);
  bool CheckIdValidity(void);
  void FixIdValidity(void);
  int GetNextAvailableId();

  /** Clear function : Remove all the objects in the scene */
  void Clear();

protected: 
  SceneSpatialObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** List of the children object plug to the SceneSpatialObject spatial object. */
  ObjectListType m_Objects; 

  /** constructor */ 
  SceneSpatialObject(); 

  /** destructor */ 
  virtual ~SceneSpatialObject();

  /** Print the object informations in a stream. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const; 

  /** Parent ID : default = -1 */
  int m_ParentId;

}; 

} // end of namespace itk
 

#ifndef ITK_MANUAL_INSTANTIATION 
#include "itkSceneSpatialObject.txx" 
#endif 

#endif // __SceneSpatialObject_h 
