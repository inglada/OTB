/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsRelabelLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-08-07 09:30:38 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsRelabelLabelMapFilter_txx
#define __itkStatisticsRelabelLabelMapFilter_txx

#include "itkStatisticsRelabelLabelMapFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage>
StatisticsRelabelLabelMapFilter<TImage>
::StatisticsRelabelLabelMapFilter()
{
  this->m_Attribute = LabelObjectType::MEAN;
}


template <class TImage>
void
StatisticsRelabelLabelMapFilter<TImage>
::GenerateData()
{
  switch( this->m_Attribute )
    {
    case LabelObjectType::MINIMUM:
      {
      typedef typename Functor::MinimumLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::MAXIMUM:
      {
      typedef typename Functor::MaximumLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::MEAN:
      {
      typedef typename Functor::MeanLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::SUM:
      {
      typedef typename Functor::SumLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::SIGMA:
      {
      typedef typename Functor::SigmaLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::VARIANCE:
      {
      typedef typename Functor::VarianceLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::MEDIAN:
      {
      typedef typename Functor::MedianLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::KURTOSIS:
      {
      typedef typename Functor::KurtosisLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::SKEWNESS:
      {
      typedef typename Functor::SkewnessLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::ELONGATION:
      {
      typedef typename Functor::ElongationLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    case LabelObjectType::FLATNESS:
      {
      typedef typename Functor::FlatnessLabelObjectAccessor< LabelObjectType > AccessorType;
      AccessorType accessor;
      this->TemplatedGenerateData(accessor);
      break;
      }
    default:
      Superclass::GenerateData();
      break;
    }
}

}// end namespace itk
#endif
