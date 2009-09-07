/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeKeepNObjectsLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeKeepNObjectsLabelMapFilter_txx
#define __itkShapeKeepNObjectsLabelMapFilter_txx

#include "itkShapeKeepNObjectsLabelMapFilter.h"
#include "itkProgressReporter.h"
#include "itkLabelObjectUtils.h"


namespace itk {

template <class TImage>
ShapeKeepNObjectsLabelMapFilter<TImage>
::ShapeKeepNObjectsLabelMapFilter()
{
  m_ReverseOrdering = false;
  m_NumberOfObjects = 1;
  m_Attribute = LabelObjectType::SIZE;
}


template <class TImage>
void
ShapeKeepNObjectsLabelMapFilter<TImage>
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
    default:
      itkExceptionMacro(<< "Unknown attribute type");
      break;
    }
}


template <class TImage>
template <class TAttributeAccessor>
void
ShapeKeepNObjectsLabelMapFilter<TImage>
::TemplatedGenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  typedef typename ImageType::LabelObjectContainerType LabelObjectContainerType;
  const LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
  typedef typename std::vector< typename LabelObjectType::Pointer > VectorType;

  ProgressReporter progress( this, 0, 2 * labelObjectContainer.size() );

  // get the label objects in a vector, so they can be sorted
  VectorType labelObjects;
  labelObjects.reserve( labelObjectContainer.size() );
  for( typename LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
    it != labelObjectContainer.end();
    it++ )
    {
    labelObjects.push_back( it->second );
    progress.CompletedPixel();
    }

  // instantiate the comparator and sort the vector
  if( m_NumberOfObjects < labelObjectContainer.size() )
    {
    typename VectorType::iterator end = labelObjects.begin() + m_NumberOfObjects;
    if( m_ReverseOrdering )
      {
      Functor::LabelObjectReverseComparator< LabelObjectType, TAttributeAccessor > comparator;
      std::nth_element( labelObjects.begin(), end, labelObjects.end(), comparator );
      }
    else
      {
      Functor::LabelObjectComparator< LabelObjectType, TAttributeAccessor > comparator;
      std::nth_element( labelObjects.begin(), end, labelObjects.end(), comparator );
      }
//   progress.CompletedPixel();
  
    // and remove the last objects of the map
    for( typename VectorType::const_iterator it = end;
      it != labelObjects.end();
      it++ )
      {
      output->RemoveLabelObject( *it );
      progress.CompletedPixel();
      }
    }
}


template <class TImage>
void
ShapeKeepNObjectsLabelMapFilter<TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "NumberOfObjects: "  << m_NumberOfObjects << std::endl;
  os << indent << "Attribute: "  << LabelObjectType::GetNameFromAttribute(m_Attribute) << " (" << m_Attribute << ")" << std::endl;
}

}// end namespace itk
#endif
