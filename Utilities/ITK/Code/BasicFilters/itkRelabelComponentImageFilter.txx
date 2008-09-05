/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRelabelComponentImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006-08-01 19:16:18 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkRelabelComponentImageFilter_txx
#define _itkRelabelComponentImageFilter_txx

#include "itkRelabelComponentImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itk_hash_map.h"
#include <map>

namespace itk
{

template< class TInputImage, class TOutputImage >
void
RelabelComponentImageFilter< TInputImage, TOutputImage >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template< class TInputImage, class TOutputImage >
void
RelabelComponentImageFilter< TInputImage, TOutputImage >
::GenerateData()
{
  unsigned long i;
  
  // Use a map to keep track of the size of each object.  Object
  // number -> ObjectType (which has Object number and the two sizes)
  typedef itk::hash_map<unsigned long, RelabelComponentObjectType> MapType;
  MapType sizeMap;
  typename MapType::iterator mapIt;
  typedef typename MapType::value_type MapValueType;

  // Get the input and the output
  typename TInputImage::ConstPointer input = this->GetInput();
  typename TOutputImage::Pointer output = this->GetOutput();

  // Setup a progress reporter.  We have 2 stages to the algorithm so
  // use the total number of pixels accessed. We walk the entire input
  // in the first pass, then walk just the output requested region in
  // the second pass.
  ProgressReporter progress(this, 0,
                            input->GetRequestedRegion().GetNumberOfPixels() +
                            output->GetRequestedRegion().GetNumberOfPixels());


  // Calculate the size of pixel
  float physicalPixelSize = 1.0;
  for (i=0; i < TInputImage::ImageDimension; ++i)
    {
    physicalPixelSize *= input->GetSpacing()[i];
    }

  RelabelComponentObjectType initialSize;
  initialSize.m_SizeInPixels = 1;
  initialSize.m_SizeInPhysicalUnits = physicalPixelSize;

  // First pass: walk the entire input image and determine what
  // labels are used and the number of pixels used in each label.
  //

  // walk the input
  InputPixelType inputValue;
  ImageRegionConstIterator<InputImageType> it;
  it = ImageRegionConstIterator<InputImageType>(input,
                                                input->GetRequestedRegion());
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    // Get the input pixel value
    inputValue = it.Get();

    // if the input pixel is not the background
    if (inputValue != NumericTraits<InputPixelType>::Zero)
      {
      // Does this label already exist
      mapIt = sizeMap.find( inputValue );
      if ( mapIt == sizeMap.end() )
        {
        // label is not currently in the map
        initialSize.m_ObjectNumber = inputValue;
        sizeMap.insert( MapValueType( inputValue, initialSize ) );
        }
      else
        {
        // label is already in the map, update the values
        (*mapIt).second.m_SizeInPixels++;
        (*mapIt).second.m_SizeInPhysicalUnits += physicalPixelSize;
        }
      }

    // increment the iterators
    ++it;
    progress.CompletedPixel();
    }

  // Now we need to reorder the labels. Use the m_ObjectSortingOrder
  // to determine how to sort the objects. Define a map for converting
  // input labels to output labels.
  //
  typedef std::vector<RelabelComponentObjectType> VectorType;
  VectorType sizeVector;
  typename VectorType::iterator vit;

  typedef std::map<unsigned long, unsigned long> RelabelMapType;
  RelabelMapType relabelMap;

  // copy the original object map to a vector so we can sort it
  for (mapIt = sizeMap.begin(); mapIt != sizeMap.end(); ++mapIt)
    {
    sizeVector.push_back( (*mapIt).second );
    }

  // sort the objects by size and define the map to use to relabel the image
  std::sort(sizeVector.begin(), sizeVector.end(), RelabelComponentSizeInPixelsComparator() );

  // create a lookup table to map the input label to the output label.
  // cache the object sizes for later access by the user
  m_NumberOfObjects = sizeVector.size();
  m_OriginalNumberOfObjects = sizeVector.size();
  m_SizeOfObjectsInPixels.clear();
  m_SizeOfObjectsInPixels.resize(m_NumberOfObjects);
  m_SizeOfObjectsInPhysicalUnits.clear();
  m_SizeOfObjectsInPhysicalUnits.resize(m_NumberOfObjects);
  int NumberOfObjectsRemoved = 0;
  for (i=0, vit = sizeVector.begin(); vit != sizeVector.end(); ++vit, ++i)
    {

    // if we find an object smaller than the minimum size, we
    // terminate the loop.
    if (m_MinimumObjectSize > 0 && (*vit).m_SizeInPixels < m_MinimumObjectSize)
      {
      // map small objects to the background
      NumberOfObjectsRemoved++;
      relabelMap.insert(RelabelMapType::value_type( (*vit).m_ObjectNumber, 0));
      }
    else
      {
      // map for input labels to output labels (Note we use i+1 in the
      // map since index 0 is the background)
      relabelMap.insert(RelabelMapType::value_type( (*vit).m_ObjectNumber, i+1));
      
      // cache object sizes for later access by the user
      m_SizeOfObjectsInPixels[i] = (*vit).m_SizeInPixels;
      m_SizeOfObjectsInPhysicalUnits[i] = (*vit).m_SizeInPhysicalUnits;
      }

    }

  // update number of objects and resize cache vectors if we have removed small objects
  m_NumberOfObjects -= NumberOfObjectsRemoved;
  if (NumberOfObjectsRemoved > 0)
    {
    m_SizeOfObjectsInPixels.resize(m_NumberOfObjects);
    m_SizeOfObjectsInPhysicalUnits.resize(m_NumberOfObjects);
    }

  // Second pass: walk just the output requested region and relabel
  // the necessary pixels.
  //

  // Allocate the output
  this->AllocateOutputs();

  // Remap the labels.  Note we only walk the region of the output
  // that was requested.  This may be a subset of the input image.
  OutputPixelType outputValue;
  ImageRegionIterator<OutputImageType> oit;
  oit = ImageRegionIterator<OutputImageType>(output,
                                             output->GetRequestedRegion());
  it = ImageRegionConstIterator<InputImageType>(input,
                                                output->GetRequestedRegion());

  it.GoToBegin();
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    inputValue = it.Get();
    // std::cerr << std::endl << "inputValue: " << inputValue << " m_SizeOfObjectsInPixels[inputValue]: " <<
    //  m_SizeOfObjectsInPixels[inputValue] << std::endl;
    if (inputValue != NumericTraits<InputPixelType>::Zero)
      {

      // lookup the mapped label
      outputValue = static_cast<OutputPixelType>(relabelMap[inputValue]); 
      oit.Set( outputValue );
      }
    else
      {
      oit.Set( inputValue );
      }

    // increment the iterators
    ++it;
    ++oit;
    progress.CompletedPixel();
    }
}


template< class TInputImage, class TOutputImage >
void
RelabelComponentImageFilter< TInputImage, TOutputImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
  os << indent << "NumberOfObjects: " << m_NumberOfObjects << std::endl;
  os << indent << "OriginalNumberOfObjects: " << m_OriginalNumberOfObjects << std::endl;
  os << indent << "NumberOfObjectsToPrint: "
     << m_NumberOfObjectsToPrint << std::endl;
  os << indent << "MinimumObjectSizez: " << m_MinimumObjectSize << std::endl;

  std::vector<unsigned long>::const_iterator it;
  std::vector<float>::const_iterator fit;
  unsigned long i;

  // limit the number of objects to print
  unsigned long numPrint = m_NumberOfObjectsToPrint;
  if (numPrint > m_SizeOfObjectsInPixels.size())
    {
    numPrint = m_SizeOfObjectsInPixels.size();
    }


  for (i=0, it = m_SizeOfObjectsInPixels.begin(),
         fit = m_SizeOfObjectsInPhysicalUnits.begin();
       i < numPrint; ++it, ++fit, ++i)
    {
    os << indent << "Object #" << i+1 << ": " << *it << " pixels, "
       << *fit << " physical units" << std::endl;
    }
  if (numPrint < m_SizeOfObjectsInPixels.size())
    {
    os << indent << "..." << std::endl;
    }
}

} // end namespace itk

#endif
