/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeOpeningLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeOpeningLabelMapFilter_txx
#define __itkShapeOpeningLabelMapFilter_txx

#include "itkShapeOpeningLabelMapFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage>
ShapeOpeningLabelMapFilter<TImage>
::ShapeOpeningLabelMapFilter()
{
  m_Lambda = NumericTraits< double >::Zero;
  m_ReverseOrdering = false;
  m_Attribute = LabelObjectType::SIZE;
}


template <class TImage>
void
ShapeOpeningLabelMapFilter<TImage>
::GenerateData()
{
  switch( m_Attribute )
    {
    case LabelObjectType::SIZE:
      TemplatedGenerateData< typename Functor::SizeLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::PHYSICAL_SIZE:
      TemplatedGenerateData< typename Functor::PhysicalSizeLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::SIZE_REGION_RATIO:
      TemplatedGenerateData< typename Functor::SizeRegionRatioLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::REGION_ELONGATION:
      TemplatedGenerateData< typename Functor::RegionElongationLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::SIZE_ON_BORDER:
      TemplatedGenerateData< typename Functor::SizeOnBorderLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::PHYSICAL_SIZE_ON_BORDER:
      TemplatedGenerateData< typename Functor::PhysicalSizeOnBorderLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::FERET_DIAMETER:
      TemplatedGenerateData< typename Functor::FeretDiameterLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::BINARY_ELONGATION:
      TemplatedGenerateData< typename Functor::BinaryElongationLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::PERIMETER:
      TemplatedGenerateData< typename Functor::PerimeterLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::ROUNDNESS:
      TemplatedGenerateData< typename Functor::RoundnessLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::EQUIVALENT_RADIUS:
      TemplatedGenerateData< typename Functor::EquivalentRadiusLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::EQUIVALENT_PERIMETER:
      TemplatedGenerateData< typename Functor::EquivalentPerimeterLabelObjectAccessor< LabelObjectType > >();
      break;
    default:
      itkExceptionMacro(<< "Unknown attribute type");
      break;
    }
}

template <class TImage>
template <class TAttributeAccessor>
void
ShapeOpeningLabelMapFilter<TImage>
::TemplatedGenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  TAttributeAccessor accessor;

  const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();

  ProgressReporter progress( this, 0, labelObjectContainer.size() );

  typename ImageType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
  while( it != labelObjectContainer.end() )
    {
    typedef typename ImageType::LabelObjectType LabelObjectType;
    typename LabelObjectType::LabelType label = it->first;
    LabelObjectType * labelObject = it->second;

    if( ( !m_ReverseOrdering && accessor( labelObject ) < m_Lambda )
      || ( m_ReverseOrdering && accessor( labelObject ) > m_Lambda ) )
      {
      // must increment the iterator before removing the object to avoid invalidating the iterator
      it++;
      output->RemoveLabel( label );
      }
    else
      {
      it++;
      }

    progress.CompletedPixel();
    }
}


template <class TImage>
void
ShapeOpeningLabelMapFilter<TImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "Lambda: "  << m_Lambda << std::endl;
  os << indent << "Attribute: "  << LabelObjectType::GetNameFromAttribute(m_Attribute) << " (" << m_Attribute << ")" << std::endl;
}

}// end namespace itk
#endif
