/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-08-24 13:34:42 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryImageToLabelMapFilter_txx
#define __itkBinaryImageToLabelMapFilter_txx

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkNumericTraits.h"

// don't think we need the indexed version as we only compute the
// index at the start of each run, but there isn't a choice
#include "itkImageLinearConstIteratorWithIndex.h"  
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConnectedComponentAlgorithm.h"

namespace itk
{

template< class TInputImage, class TOutputImage >
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::BinaryImageToLabelMapFilter()
{
  this->m_FullyConnected = false;
  this->m_NumberOfObjects = 0;
  this->m_OutputBackgroundValue = NumericTraits<OutputPixelType>::NonpositiveMin();
  this->m_InputForegroundValue = NumericTraits<InputPixelType>::max();
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( !input )
    {
    return;
    }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}

template< class TInputImage, class TOutputImage >
void 
BinaryImageToLabelMapFilter< TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::BeforeThreadedGenerateData()
{
  typename TOutputImage::Pointer output = this->GetOutput();
  typename TInputImage::ConstPointer input = this->GetInput();

  output->SetBackgroundValue( this->m_OutputBackgroundValue );

  long nbOfThreads = this->GetNumberOfThreads();
  if( itk::MultiThreader::GetGlobalMaximumNumberOfThreads() != 0 )
    {
    nbOfThreads = vnl_math_min( this->GetNumberOfThreads(), itk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
    }

  // number of threads can be constrained by the region size, so call the SplitRequestedRegion
  // to get the real number of threads which will be used
  typename TOutputImage::RegionType splitRegion;  // dummy region - just to call the following method
  nbOfThreads = this->SplitRequestedRegion(0, nbOfThreads, splitRegion);

  // set up the vars used in the threads
  this->m_NumberOfLabels.clear();
  this->m_NumberOfLabels.resize( nbOfThreads, 0 );
  this->m_Barrier = Barrier::New();
  this->m_Barrier->Initialize( nbOfThreads );

  SizeValueType pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  SizeValueType xsize = output->GetRequestedRegion().GetSize()[0];
  SizeValueType linecount = pixelcount/xsize;
  m_LineMap.resize( linecount );
  m_FirstLineIdToJoin.resize( nbOfThreads - 1 );
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const RegionType& outputRegionForThread,
         int threadId) 
{
  typename TOutputImage::Pointer output = this->GetOutput();
  typename TInputImage::ConstPointer input = this->GetInput();

  long nbOfThreads = this->m_NumberOfLabels.size();

  // create a line iterator
  typedef itk::ImageLinearConstIteratorWithIndex<InputImageType>
    InputLineIteratorType;
  InputLineIteratorType inLineIt(input, outputRegionForThread);
  inLineIt.SetDirection(0);

  // set the progress reporter to deal with the number of lines
  SizeValueType pixelcountForThread = outputRegionForThread.GetNumberOfPixels();
  SizeValueType xsizeForThread = outputRegionForThread.GetSize()[0];
  SizeValueType linecountForThread = pixelcountForThread/xsizeForThread;
  ProgressReporter progress(this, threadId, linecountForThread);

  // find the split axis
  IndexType outputRegionIdx = output->GetRequestedRegion().GetIndex();
  IndexType outputRegionForThreadIdx = outputRegionForThread.GetIndex();
  SizeType outputRegionSize = output->GetRequestedRegion().GetSize();
  SizeType outputRegionForThreadSize = outputRegionForThread.GetSize();
  int splitAxis = 0;
  for( int i=0; i<ImageDimension; i++ )
    {
    if( outputRegionSize[i] != outputRegionForThreadSize[i] )
      {
      splitAxis = i;
      }
    }

  // compute the number of pixels before that threads
  outputRegionSize[splitAxis] = outputRegionForThreadIdx[splitAxis] - outputRegionIdx[splitAxis];
  SizeValueType firstLineIdForThread = RegionType( outputRegionIdx, outputRegionSize ).GetNumberOfPixels() / xsizeForThread;
  SizeValueType lineId = firstLineIdForThread;

  OffsetVectorType LineOffsets;
  this->SetupLineOffsets(LineOffsets);

  long nbOfLabels = 0;
  for( inLineIt.GoToBegin();
    !inLineIt.IsAtEnd();
    inLineIt.NextLine() )
    {
    inLineIt.GoToBeginOfLine();
    lineEncoding ThisLine;
    while (! inLineIt.IsAtEndOfLine())
      {
      InputPixelType PVal = inLineIt.Get();
      //std::cout << inLineIt.GetIndex() << std::endl;
      if (PVal == this->m_InputForegroundValue)
        {
        // We've hit the start of a run
        runLength thisRun;
        long length=0;
        IndexType thisIndex;
        thisIndex = inLineIt.GetIndex();
        //std::cout << thisIndex << std::endl;
        ++length;
        ++inLineIt;
        while( !inLineIt.IsAtEndOfLine()
          && inLineIt.Get() == this->m_InputForegroundValue )
          {
          ++length;
          ++inLineIt;
          }
        // create the run length object to go in the vector
        thisRun.length=length;
        thisRun.label=0; // will give a real label later
        thisRun.where = thisIndex;
        ThisLine.push_back(thisRun);
        nbOfLabels++;
        }
      else 
        {
        ++inLineIt;
        }
      }
    m_LineMap[lineId] = ThisLine;
    lineId++;
    progress.CompletedPixel();
    }

  this->m_NumberOfLabels[threadId] = nbOfLabels;

  // wait for the other threads to complete that part
  this->Wait();

  // compute the total number of labels
  nbOfLabels = 0;
  for( int i=0; i<nbOfThreads; i++ )
    {
    nbOfLabels += this->m_NumberOfLabels[i];
    }
  
  if( threadId == 0 )
    {
    // set up the union find structure
    this->InitUnion(nbOfLabels);
    // insert all the labels into the structure -- an extra loop but
    // saves complicating the ones that come later
    typename LineMapType::iterator MapBegin, MapEnd, LineIt;
    MapBegin = m_LineMap.begin();
    MapEnd = m_LineMap.end(); 
    LineIt = MapBegin;
    unsigned long label = 1;
    for (LineIt = MapBegin; LineIt != MapEnd; ++LineIt)
      {
      typename lineEncoding::iterator cIt;
      for (cIt = LineIt->begin();cIt != LineIt->end();++cIt)
        {
        cIt->label = label;
        this->InsertSet(label);
        label++;
        }
      }
    }

  // wait for the other threads to complete that part
  this->Wait();

  // now process the map and make appropriate entries in an equivalence
  // table
  // assert( linecount == m_LineMap.size() );
  SizeValueType pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  SizeValueType xsize = output->GetRequestedRegion().GetSize()[0];
  OffsetValueType linecount = pixelcount/xsize;

  SizeValueType lastLineIdForThread =  linecount;
  long nbOfLineIdToJoin = 0;
  if( threadId != nbOfThreads - 1 )
    {
    SizeType localRegionSize = outputRegionForThreadSize;
    localRegionSize[splitAxis] -= 1;
    lastLineIdForThread = firstLineIdForThread + RegionType( outputRegionIdx, localRegionSize ).GetNumberOfPixels() / xsizeForThread;
    m_FirstLineIdToJoin[threadId] = lastLineIdForThread;
    // found the number of line ids to join
    nbOfLineIdToJoin = RegionType( outputRegionIdx, outputRegionForThread.GetSize() ).GetNumberOfPixels() / xsizeForThread
                         - RegionType( outputRegionIdx, localRegionSize ).GetNumberOfPixels() / xsizeForThread;
    }

  for(SizeValueType ThisIdx = firstLineIdForThread; ThisIdx < lastLineIdForThread; ++ThisIdx)
    {
    if( !m_LineMap[ThisIdx].empty() )
      {
      OffsetVectorType::const_iterator I = LineOffsets.begin();
      while( I != LineOffsets.end() )
        {
        OffsetValueType NeighIdx = ThisIdx + (*I);
        // check if the neighbor is in the map
        if ( NeighIdx >= 0 && NeighIdx < linecount && !m_LineMap[NeighIdx].empty() ) 
          {
          // Now check whether they are really neighbors
          bool areNeighbors = this->CheckNeighbors(m_LineMap[ThisIdx][0].where, m_LineMap[NeighIdx][0].where);
          if (areNeighbors)
            {
            // Compare the two lines
            this->CompareLines(m_LineMap[ThisIdx], m_LineMap[NeighIdx]);
            }
          }
        ++I;
        }
      }
    }
  
  // wait for the other threads to complete that part
  this->Wait();

  while( m_FirstLineIdToJoin.size() != 0 )
    {
    const SizeValueType threadChunk = 2 * threadId;
    if( threadChunk < (SizeValueType)m_FirstLineIdToJoin.size() )
      {
      for(SizeValueType ThisIdx = m_FirstLineIdToJoin[threadChunk];
          ThisIdx < m_FirstLineIdToJoin[threadChunk] + nbOfLineIdToJoin;
          ++ThisIdx)
        {
        if( !m_LineMap[ThisIdx].empty() )
          {
          OffsetVectorType::const_iterator I = LineOffsets.begin();
          while( I != LineOffsets.end() )
            {
            OffsetValueType NeighIdx = ThisIdx + (*I);
            // check if the neighbor is in the map
            if ( NeighIdx >= 0 && NeighIdx < linecount && !m_LineMap[NeighIdx].empty() ) 
              {
              // Now check whether they are really neighbors
              bool areNeighbors
                = CheckNeighbors(m_LineMap[ThisIdx][0].where, m_LineMap[NeighIdx][0].where);
              if (areNeighbors)
                {
                // Compare the two lines
                CompareLines(m_LineMap[ThisIdx], m_LineMap[NeighIdx]);
                }
              }
            ++I;
            }
          }
        }
      }

    this->Wait();

    if( threadId == 0 )
      {
      // remove the region already joined
      typename std::vector< SizeValueType > newFirstLineIdToJoin;
      for( SizeValueType i = 1; i < (SizeValueType)m_FirstLineIdToJoin.size(); i += 2 )
        {
        newFirstLineIdToJoin.push_back( m_FirstLineIdToJoin[i] );
        }
      m_FirstLineIdToJoin = newFirstLineIdToJoin;
      }

    this->Wait();

    }

}

template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::AfterThreadedGenerateData()
{
  typename TOutputImage::Pointer output = this->GetOutput();
  typename TInputImage::ConstPointer input = this->GetInput();
  SizeValueType pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  SizeValueType xsize = output->GetRequestedRegion().GetSize()[0];
  SizeValueType linecount = pixelcount/xsize;
  unsigned long int totalLabs = CreateConsecutive();
  ProgressReporter progress(this, 0, linecount);
  // check for overflow exception here
  if( totalLabs > static_cast<unsigned long int>(
          NumericTraits<OutputPixelType>::max() ) )
    {
    itkExceptionMacro(
      << "Number of objects greater than maximum of output pixel type " );
    }

  for (SizeValueType ThisIdx = 0; ThisIdx<linecount; ThisIdx++)
    {
    // now fill the labelled sections
    typedef typename lineEncoding::const_iterator LineIterator;

    LineIterator cIt = m_LineMap[ThisIdx].begin();
    while( cIt != m_LineMap[ThisIdx].end() )
      {
      SizeValueType Ilab = LookupSet( cIt->label);
      OutputPixelType lab = m_Consecutive[Ilab];
      output->SetLine( cIt->where, cIt->length, lab );
      ++cIt;
      }
    progress.CompletedPixel();
    }

  this->m_NumberOfLabels.clear();
  this->m_Barrier = NULL;

  m_LineMap.clear();
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::SetupLineOffsets( OffsetVectorType &LineOffsets)
{
  // Create a neighborhood so that we can generate a table of offsets
  // to "previous" line indexes
  // We are going to mis-use the neighborhood iterators to compute the
  // offset for us. All this messing around produces an array of
  // offsets that will be used to index the map
  typename TOutputImage::Pointer output = this->GetOutput();
  typedef Image<long, TOutputImage::ImageDimension - 1>     PretendImageType;
  typedef typename PretendImageType::RegionType::SizeType   PretendSizeType;
  typedef typename PretendImageType::RegionType::IndexType  PretendIndexType;
  typedef ConstShapedNeighborhoodIterator<PretendImageType> LineNeighborhoodType;

  typename PretendImageType::Pointer fakeImage;
  fakeImage = PretendImageType::New();

  typename PretendImageType::RegionType LineRegion;
  //LineRegion = PretendImageType::RegionType::New();

  OutSizeType OutSize = output->GetRequestedRegion().GetSize();

  PretendSizeType PretendSize;
  // The first dimension has been collapsed
  for (unsigned int i = 0; i<PretendSize.GetSizeDimension(); i++)
    {
    PretendSize[i] = OutSize[i+1];
    }

  LineRegion.SetSize(PretendSize);
  fakeImage->SetRegions( LineRegion );
  PretendSizeType kernelRadius;
  kernelRadius.Fill(1);
  LineNeighborhoodType lnit(kernelRadius, fakeImage, LineRegion);

  // only activate the indices that are "previous" to the current
  // pixel and face connected (exclude the center pixel from the
  // neighborhood)
  //
  setConnectivityPrevious( &lnit, m_FullyConnected );

  typename LineNeighborhoodType::IndexListType ActiveIndexes;
  ActiveIndexes = lnit.GetActiveIndexList();

  typename LineNeighborhoodType::IndexListType::const_iterator LI;
  
  PretendIndexType idx = LineRegion.GetIndex();
  OffsetValueType offset = fakeImage->ComputeOffset( idx );

  for (LI=ActiveIndexes.begin(); LI != ActiveIndexes.end(); LI++)
    {
    LineOffsets.push_back( fakeImage->ComputeOffset( idx + lnit.GetOffset( *LI ) ) - offset );
    }
  
  // LineOffsets is the thing we wanted.
}


template< class TInputImage, class TOutputImage >
bool
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::CheckNeighbors(const OutputIndexType &A, 
                 const OutputIndexType &B)
{
  // this checks whether the line encodings are really neighbors. The
  // first dimension gets ignored because the encodings are along that
  // axis
  OutputOffsetType Off = A - B;
  for (unsigned i = 1; i < OutputImageDimension; i++)
    {
    if (abs(Off[i]) > 1)
      {
      return(false);
      }
    }
  return(true);
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::CompareLines(lineEncoding &current, const lineEncoding &Neighbour)
{
  OffsetValueType offset = 0;
  if (m_FullyConnected)
    {
    offset = 1;
    }

  typename lineEncoding::const_iterator nIt, mIt;
  typename lineEncoding::iterator cIt;

  mIt = Neighbour.begin(); // out marker iterator

  for (cIt = current.begin();cIt != current.end();++cIt)
    {
    //runLength cL = *cIt;
    OffsetValueType cStart = cIt->where[0];  // the start x position
    OffsetValueType cLast = cStart + cIt->length - 1;

    for (nIt=mIt; nIt != Neighbour.end(); ++nIt)
      {
      //runLength nL = *nIt;
      OffsetValueType nStart = nIt->where[0];
      OffsetValueType nLast = nStart + nIt->length - 1;
      // there are a few ways that neighbouring lines might overlap
      //   neighbor      S                  E
      //   current    S                        E
      //------------------------------------------
      //   neighbor      S                  E
      //   current    S                E
      //------------------------------------------
      //   neighbor      S                  E
      //   current             S                  E
      //------------------------------------------
      //   neighbor      S                  E
      //   current             S       E
      //------------------------------------------
      OffsetValueType ss1 = nStart - offset;
      // OffsetValueType ss2 = nStart + offset;
      OffsetValueType ee1 = nLast - offset;
      OffsetValueType ee2 = nLast + offset;
      bool eq = false;
      // the logic here can probably be improved a lot
      if ((ss1 >= cStart) && (ee2 <= cLast))
        {
        // case 1
        eq = true;
        } 
      else 
        {
        if ((ss1 <= cLast) && (ee2 >= cLast))
          {
          // case 2
          eq = true;
          }
        else 
          {
          if ((ss1 <= cStart) && (ee2 >= cStart))
            {
            // case 3 
            eq = true;
            }
          else 
            {
            if ((ss1 <= cStart) && (ee2 >= cLast))
              {
              // case 4
              eq = true;
              }
            }
          }
        }
      if (eq) 
        {
        LinkLabels(nIt->label, cIt->label);
        } 

      if (ee1 >= cLast)
        {
        // No point looking for more overlaps with the current run
        // because the neighbor run is either case 2 or 4
        mIt = nIt;
        break;
        }
      }
    }

}

// union find related functions
template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::InsertSet(const unsigned long int label)
{
  m_UnionFind[label]=label;
}

template< class TInputImage, class TOutputImage >
unsigned long int
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::CreateConsecutive()
{
  m_Consecutive = UnionFindType(m_UnionFind.size());
  m_Consecutive[this->m_OutputBackgroundValue] = this->m_OutputBackgroundValue;
  unsigned long int CLab = 0;
  unsigned long int count = 0;
  for( unsigned long int I = 1; I < m_UnionFind.size(); I++ )
    {
    unsigned long int L = m_UnionFind[I];
    if( L == I ) 
      {
      if( CLab == this->m_OutputBackgroundValue )
        {
        ++CLab;
        }
      m_Consecutive[L] = CLab;
      ++CLab;
      ++count;
      }
    }
  return count;
}

template< class TInputImage, class TOutputImage >
unsigned long int
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::LookupSet(const unsigned long int label)
{
  // recursively set the equivalence if necessary
  if (label != m_UnionFind[label])
    {
    m_UnionFind[label] = this->LookupSet(m_UnionFind[label]);
    }
  return(m_UnionFind[label]);
}

template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::LinkLabels(const unsigned long int lab1, const unsigned long int lab2)
{
  unsigned long E1 = this->LookupSet(lab1);
  unsigned long E2 = this->LookupSet(lab2);

  if (E1 < E2)
    {
    m_UnionFind[E2] = E1;
    }
  else
    {
    m_UnionFind[E1] = E2;
    }
}

template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "InputForegroundValue: "
     << static_cast<typename NumericTraits<InputPixelType>::PrintType>(this->m_InputForegroundValue) << std::endl;
  os << indent << "OutputBackgroundValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(this->m_OutputBackgroundValue) << std::endl;
  os << indent << "Number of Objects: " << this->m_NumberOfObjects << std::endl;
}

} // end namespace itk

#endif
