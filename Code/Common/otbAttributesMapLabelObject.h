/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbAttributesMapLabelObject_h
#define __otbAttributesMapLabelObject_h

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include <map>

namespace otb
{

namespace Functor
{

/** \class AttributesMapLabelObjectAccessor
 *  \brief Allows to acces a given field of an AttributesMapLabelObject
 *
 * The name of the attribute to retrieve can be set by using the
 * SetAttributeName method.
 *
 * \sa AttributesMapLabelObject
 */
template< class TLabelObject >
class ITK_EXPORT AttributesMapLabelObjectAccessor
{
public:
  // The Label Object typedef
  typedef TLabelObject                                  LabelObjectType;
  typedef typename LabelObjectType::AttributesValueType AttributeValueType;

  /**
   * Please note that no exception catching is done here.
   * \param labelObject The pointer to the label object
   * \return The attribute value.
   */
  inline const AttributeValueType operator()( LabelObjectType * labelObject ) const
    {
    return labelObject->GetAttribute(m_AttributeName.c_str());
    }
  
  /// Set the name of the attribute to retrieve
  void SetAttributeName(const char * name)
  {
    m_AttributeName = name;
  }
  /// Get the the name of the attribute to retrieve
  const char * GetAttributeName()
  {
    return m_AttributeName.c_str();
  }

  /// Constructor
  AttributesMapLabelObjectAccessor() : m_AttributeName("") {}

  /// Destructor
  ~AttributesMapLabelObjectAccessor() {}

private:
  /// Name of the attribute to retrieve
  std::string m_AttributeName;
};

} // end namespace Functor

/** \class AttributesMapLabelObject
 *  \brief A LabelObject with a generic attributes map
 *
 *  This class derives from itk::LabelObject and extends it to
 *  store pairs of key,value (of type TAttributesValue) in an internal
 *  map container.
 *
 * As such it allows to store any custom attributes as necessary.
 *
 *\sa LabelObject, ShapeLabelObject, StatisticsLabelObject
 *
 * \ingroup DataRepresentation
 */
template < class TLabel, unsigned int VImageDimension, class TAttributesValue >
class ITK_EXPORT AttributesMapLabelObject
: public itk::LabelObject< TLabel, VImageDimension >
{
public:
  /** Standard class typedefs */
  typedef AttributesMapLabelObject                    Self;
  typedef itk::LabelObject< TLabel, VImageDimension > Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;
  typedef itk::WeakPointer <const Self>               ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AttributesMapLabelObject, LabelObject);

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /// Type of a label map using an AttributesMapLabelObject
  typedef itk::LabelMap< Self >                      LabelMapType;

  /// Template parameters typedef
  typedef TLabel                                     LabelType;
  typedef TAttributesValue                           AttributesValueType;
  
  // Convenient inherited typedefs
  typedef typename Superclass::IndexType             IndexType;
  typedef typename Superclass::LineType              LineType;
  typedef typename Superclass::LengthType            LengthType;
  typedef typename Superclass::LineContainerType     LineContainerType;

  /// Map container typedefs
  typedef std::map<std::string,AttributesValueType>  AttributesMapType;
  typedef typename AttributesMapType::iterator       AttributesMapIteratorType;
  typedef typename AttributesMapType::const_iterator AttributesMapConstIteratorType;

  /**
   * Set an attribute value.
   * If the key name already exists in the map, the value is overwritten.
   */
  void SetAttribute(const char * name, AttributesValueType value)
  {
    m_Attributes[name]=value;
  }

  /**
   * Returns the attribute corresponding to name
   */
  AttributesValueType GetAttribute(const char * name) const
  {
    AttributesMapConstIteratorType it = m_Attributes.find(name);
    if (it!=m_Attributes.end())
      {
      return it->second;
      }
    else
      {
      itkExceptionMacro(<<"Could not find attribute named "<<name<<".");
      }
  }

  /**
   * Returns the total number of attributes
   */
  unsigned int GetNumberOfAttributes() const
  {
    return m_Attributes.size();
  }

  /**
   * Returns the list of available attributes
   */
  std::vector<std::string> GetAvailableAttributes() const
  {
    std::vector<std::string> attributesNames;
    
    AttributesMapConstIteratorType it = m_Attributes.begin();

    while(it!=m_Attributes.end())
      {
      attributesNames.push_back(it->first);
      ++it;
      }
    return attributesNames;
  }
  
  /**
  * This method is overloaded to add the copy of the attributes map.
  */
  virtual void CopyAttributesFrom( const Self * src)
    {
    Superclass::CopyAttributesFrom( src );
      m_Attributes = src->m_Attributes;
   }

protected:
  /** Constructor */
  AttributesMapLabelObject() : m_Attributes()
    {}
  /** Destructor */
  virtual ~AttributesMapLabelObject() {}
  
  /** The printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
    Superclass::PrintSelf( os, indent );
    os << indent << "Attributes: "<< std::endl;
    for(AttributesMapConstIteratorType it = m_Attributes.begin();
	it!=m_Attributes.end();++it)
      {
      os<<indent<<indent<<it->first<<" = "<<it->second<<std::endl;
      }
    }
private:
  AttributesMapLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The attributes map */
  AttributesMapType m_Attributes;
};

} // end namespace otb
#endif
