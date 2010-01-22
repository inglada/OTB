/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWatershedRelabeler.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-27 19:30:18 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWatershedRelabeler_txx
#define __itkWatershedRelabeler_txx

#include "itkImageRegionIterator.h"

namespace itk
{
namespace watershed
{

template <class TScalarType, unsigned int TImageDimension>
Relabeler<TScalarType, TImageDimension>::
Relabeler() : m_FloodLevel(0.0)
{
  typename ImageType::Pointer img
    = static_cast<ImageType*>(this->MakeOutput(0).GetPointer());
  this->SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, img.GetPointer());
}
  
template <class TScalarType, unsigned int TImageDimension>
typename Relabeler<TScalarType, TImageDimension>::DataObjectPointer
Relabeler<TScalarType, TImageDimension>
::MakeOutput(unsigned int itkNotUsed(idx))
{
  return static_cast<DataObject*>(ImageType::New().GetPointer());
}
  
template <class TScalarType, unsigned int TImageDimension>
void Relabeler<TScalarType, TImageDimension>
::GenerateData()
{
  this->UpdateProgress(0.0);
  typename ImageType::Pointer input  = this->GetInputImage();
  typename ImageType::Pointer output  = this->GetOutputImage();

  typename SegmentTreeType::Pointer tree = this->GetInputSegmentTree();
  typename SegmentTreeType::Iterator it;
  EquivalencyTable::Pointer eqT = EquivalencyTable::New();

  output->SetBufferedRegion(output->GetRequestedRegion());
  output->Allocate();
  //
  // Copy input to output
  //
  ImageRegionIterator<ImageType> it_a(input, output->GetRequestedRegion());
  ImageRegionIterator<ImageType> it_b(output, output->GetRequestedRegion());
  it_a = it_a.Begin();
  it_b = it_b.Begin();
  while (! it_a.IsAtEnd() )
    {
    it_b.Set(it_a.Get());
    ++it_a;
    ++it_b;
    }

  this->UpdateProgress(0.1);
  //
  // Extract the merges up the requested level
  //
  if (tree->Empty() == true )
    {
    // itkWarningMacro("Empty input.  No relabeling was done.");
    return;
    }
  ScalarType max = tree->Back().saliency;
  ScalarType mergeLimit = static_cast<ScalarType>(m_FloodLevel * max);

  this->UpdateProgress(0.5);
  
  it = tree->Begin();
  while ( it != tree->End() && (*it).saliency <= mergeLimit )
    {
    eqT->Add((*it).from, (*it).to);
    it++;
    } 

  SegmenterType::RelabelImage(output, output->GetRequestedRegion(), eqT);
  this->UpdateProgress(1.0);
}

template <class TScalarType, unsigned int VImageDimension>
void Relabeler<TScalarType, VImageDimension>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename ImageType::Pointer  inputPtr  = this->GetInputImage();
  typename ImageType::Pointer outputPtr = this->GetOutputImage();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // 
  // FOR NOW WE'LL JUST SET THE INPUT REGION TO THE OUTPUT REGION
  //
  inputPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
}
  
template <class TScalarType, unsigned int TImageDimension>
void Relabeler<TScalarType, TImageDimension>
::GenerateOutputRequestedRegion(DataObject *output)
{
  // Only the Image output need to be propagated through.
  // No choice but to use RTTI here.
  // All Image outputs set to the same RequestedRegion  other
  // outputs ignored.
  ImageBase<ImageDimension> *imgData;
  ImageBase<ImageDimension> *op;
  imgData = dynamic_cast<ImageBase<ImageDimension> * >(output);

  if (imgData)
    {
    std::vector<ProcessObject::DataObjectPointer>::size_type idx;
    for (idx = 0; idx < this->GetOutputs().size(); ++idx)
      {
      if (this->GetOutputs()[idx] && this->GetOutputs()[idx] != output)
        {
        op = dynamic_cast<ImageBase<ImageDimension> *>(this->GetOutputs()[idx].GetPointer());
        if (op) this->GetOutputs()[idx]->SetRequestedRegion(output);
        }
      }
    }
}

template <class TScalarType, unsigned int TImageDimension>
void Relabeler<TScalarType, TImageDimension>
::GraftOutput(ImageType *graft)
{
  this->GraftNthOutput(0, graft);
}

template <class TScalarType, unsigned int TImageDimension>
void Relabeler<TScalarType, TImageDimension>
::GraftNthOutput(unsigned int idx, ImageType *graft)
{
  typedef typename ImageType::Pointer OutputImagePointer;
  
  if (idx < this->GetNumberOfOutputs())
    {
    OutputImagePointer output = this->GetOutputImage();

    if (output && graft)
      {
      // grab a handle to the bulk data of the specified data object
      output->SetPixelContainer( graft->GetPixelContainer() );
      
      // copy the region ivars of the specified data object
      output->SetRequestedRegion( graft->GetRequestedRegion() );
      output->SetLargestPossibleRegion( graft->GetLargestPossibleRegion() );
      output->SetBufferedRegion( graft->GetBufferedRegion() );
      
      // copy the meta-information
      output->CopyInformation( graft );
      }
    }
}

template<class TScalarType, unsigned int TImageDimension>
void
Relabeler<TScalarType, TImageDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "FloodLevel: " << m_FloodLevel << std::endl;
}

}// end namespace watershed
}// end namespace itk

#endif
