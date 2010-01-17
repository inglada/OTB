/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeOpeningLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-08-05 15:52:35 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeOpeningLabelMapFilter_h
#define __itkShapeOpeningLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkLabelObjectAccessors.h"
#include "itkShapeLabelObjectAccessors.h"
#include "itkProgressReporter.h"

namespace itk {
/** \class ShapeOpeningLabelMapFilter
 * \brief Remove objects according to the value of their shape attribute.
 *
 * ShapeOpeningLabelMapFilter removes objects in a label collection image
 * with an attribute value smaller or greater than a threshold called Lambda.
 * The attributes are those of the ShapeLabelObject.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ShapeLabelObject, BinaryShapeOpeningImageFilter, LabelStatisticsOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage>
class ITK_EXPORT ShapeOpeningLabelMapFilter : 
    public InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ShapeOpeningLabelMapFilter    Self;
  typedef InPlaceLabelMapFilter<TImage> Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage                              ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  
  typedef typename LabelObjectType::AttributeType AttributeType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ShapeOpeningLabelMapFilter, InPlaceLabelMapFilter);

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

  /**
   * Set/Get the threshold used to keep or remove the objects.
   */
  itkGetConstMacro(Lambda, double);
  itkSetMacro(Lambda, double);

  /**
   * Set/Get the ordering of the objects. By default, objects with
   * an attribute value smaller than Lamba are removed. Turning ReverseOrdering
   * to true makes this filter remove objects with an attribute value greater
   * than Lambda instead.
   */
  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );
  
  /**
   * Set/Get the attribute to use to select the object to remove. 
   * The default is "Size".
   */
  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );
  void SetAttribute( const std::string & s )
    {
    this->SetAttribute( LabelObjectType::GetAttributeFromName( s ) );
    }


protected:
  ShapeOpeningLabelMapFilter();
  ~ShapeOpeningLabelMapFilter() {};

  void GenerateData();

  template <class TAttributeAccessor> 
  void TemplatedGenerateData( const TAttributeAccessor & accessor )
    {
    // Allocate the output
    this->AllocateOutputs();

    ImageType * output = this->GetOutput();
    ImageType * output2 = this->GetOutput( 1 );
    assert( this->GetNumberOfOutputs() == 2 );
    assert( output2 != NULL );

    // set the background value for the second output - this is not done in the superclasses
    output2->SetBackgroundValue( output->GetBackgroundValue() );

    const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();

    ProgressReporter progress( this, 0, labelObjectContainer.size() );

    typename ImageType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
    while( it != labelObjectContainer.end() )
      {
      typename LabelObjectType::LabelType label = it->first;
      LabelObjectType * labelObject = it->second;

      if( ( !m_ReverseOrdering && accessor( labelObject ) < m_Lambda )
        || ( m_ReverseOrdering && accessor( labelObject ) > m_Lambda ) )
        {
        // must increment the iterator before removing the object to avoid invalidating the iterator
        it++;
        output2->AddLabelObject( labelObject );
        output->RemoveLabel( label );
        }
      else
        {
        it++;
        }

      progress.CompletedPixel();
      }
    }

  void PrintSelf(std::ostream& os, Indent indent) const;

  double        m_Lambda;
  bool          m_ReverseOrdering;
  AttributeType m_Attribute;

private:
  ShapeOpeningLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShapeOpeningLabelMapFilter.txx"
#endif

#endif
