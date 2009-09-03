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
#ifndef __otbAttributesMapOpeningLabelMapFilter_h
#define __otbAttributesMapOpeningLabelMapFilter_h

#include "otbAttributesMapLabelObject.h"
#include "itkAttributeOpeningLabelMapFilter.h"

namespace otb 
{

/** \class AttributesMapOpeningLabelMapFilter
 * \brief Attributes opening for AttributesMapLabelObject
 *
 * This class extends the itk::AttributeOpeningLabelMapFilter to
 * handle AttributesMapLabelObject. Thus it allows to perform openings
 * for any kind of attributes in the attributes map.
 * 
 * \sa AttributeLabelObject, AttributeOpeningLabelMapFilter
 *
 * \ingroup ImageEnhancement MathematicalMorphologyImageFilters
 */
template<class TImage>
class ITK_EXPORT AttributesMapOpeningLabelMapFilter 
: public itk::AttributeOpeningLabelMapFilter<TImage,
	      Functor::AttributesMapLabelObjectAccessor<typename TImage::LabelObjectType> >
{
public:
  /** Standard class typedefs. */
  typedef AttributesMapOpeningLabelMapFilter                       Self;
  typedef itk::AttributeOpeningLabelMapFilter <TImage,
               Functor::AttributesMapLabelObjectAccessor<TImage> > Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AttributesMapOpeningLabelMapFilter,AttributeOpeningLabelMapFilter);

  /** Set the name of the attribute to perform opening on */
  void SetAttributeName(const char * name)
  {
    this->GetAccessor().SetAttributeName(name);
  }

  /** Get the name of the attribute to perform opening on */
  const char *  GetAttributeName()
  {
    return this->GetAccessor().GetAttributeName();
  }

protected:
  /** Constructor */
  AttributesMapOpeningLabelMapFilter()  {}
  /** Destructor */
  ~AttributesMapOpeningLabelMapFilter() {}

private:
  AttributesMapOpeningLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace otb

#endif


