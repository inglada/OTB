/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelSelectionLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelSelectionLabelMapFilter_h
#define __itkLabelSelectionLabelMapFilter_h

#include "itkAttributeSelectionLabelMapFilter.h"
#include "itkLabelObject.h"
#include <set>


namespace itk {
/** \class LabelSelectionLabelMapFilter
 * \brief remove the objects according to the value of their attribute
 *
 * LabelSelectionLabelMapFilter removes the objects in a label collection image
 * with an attribute value inside or outside a set of attribute values passed by
 * the user.
 * The attribute is provide by an attribute accessor given in template parameter.
 * Contrary to the other filters made to remove some object of a LabelMap, no
 * ordering relation for the attribute is needed in that filter.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa AttributeLabelObject
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage>
class ITK_EXPORT LabelSelectionLabelMapFilter : 
    public AttributeSelectionLabelMapFilter<TImage, typename Functor::LabelLabelObjectAccessor< typename TImage::LabelObjectType > >
{
public:
  /** Standard class typedefs. */
  typedef LabelSelectionLabelMapFilter Self;
  typedef AttributeSelectionLabelMapFilter<TImage, typename Functor::LabelLabelObjectAccessor< typename TImage::LabelObjectType > > Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  
  typedef typename Superclass::AttributeAccessorType AttributeAccessorType;
  typedef typename Superclass::AttributeValueType AttributeValueType;
  
  typedef typename Superclass::AttributeSetType AttributeSetType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelSelectionLabelMapFilter, 
               AttributeSelectionLabelMapFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
  /** End concept checking */
#endif

  const AttributeSetType & GetLabelSet() const
    {
    return this->GetAttributeSet();
    }
    
  void SetLabelSet( const AttributeSetType & set )
    {
    this->SetAttributeSet( set );
    }
    
  /** Clear the attribute set, and add the attribute passed in parameter */
  void SetLabel( const AttributeValueType & attr )
    {
    this->SetAttribute( attr );
    }
    
  void ClearLabelSet()
    {
    this->ClearAttributeSet();
    }
    
  void AddLabel( const AttributeValueType & attr )
    {
    this->AddAttribute( attr );
    }

protected:
  LabelSelectionLabelMapFilter() {};
  ~LabelSelectionLabelMapFilter() {};

private:
  LabelSelectionLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#endif


