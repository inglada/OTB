/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/02/20 15:28:29 $
  Version:   $Revision: 1.21 $

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

  output->SetBackgroundValue( m_BackgroundValue );

  long nbOfThreads = this->GetNumberOfThreads();
  if( itk::MultiThreader::GetGlobalMaximumNumberOfThreads() != 0 )
    {
    nbOfThreads = std::min( this->GetNumberOfThreads(), itk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
    }
  // number of threads can be constrained by the region size, so call the SplitRequestedRegion
  // to get the real number of threads which will be used
  typename TOutputImage::RegionType splitRegion;  // dummy region - just to call the following method
  nbOfThreads = this->SplitRequestedRegion(0, nbOfThreads, splitRegion);
  // std::cout << "nbOfThreads: " << nbOfThreads << std::endl;

  // set up the vars used in the threads
  m_NumberOfLabels.clear();
  m_NumberOfLabels.resize( nbOfThreads, 0 );
  m_Barrier = Barrier::New();
  m_Barrier->Initialize( nbOfThreads );
  long pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  long xsize = output->GetRequestedRegion().GetSize()[0];
  long linecount = pixelcount/xsize;
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

  long nbOfThreads = this->GetNumberOfThreads();
  if( itk::MultiThreader::GetGlobalMaximumNumberOfThreads() != 0 )
    {
    nbOfThreads = std::min( this->GetNumberOfThreads(), itk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
    }

  // create a line iterator
  typedef itk::ImageLinearConstIteratorWithIndex<InputImageType>
    InputLineIteratorType;
  InputLineIteratorType inLineIt(input, outputRegionForThread);
  inLineIt.SetDirection(0);

  // set the progress reporter to deal with the number of lines
  long pixelcountForThread = outputRegionForThread.GetNumberOfPixels();
  long xsizeForThread = outputRegionForThread.GetSize()[0];
  long linecountForThread = pixelcountForThread/xsizeForThread;
  ProgressReporter progress(this, threadId, linecountForThread);

  // find the split axis
  IndexType outputRegionIdx = output->GetRequestedRegion().GetIndex();
  IndexType outputRegionForThreadIdx = outputRegionForThread.GetIndex();
  int splitAxis = 0;
  for( int i=0; i<ImageDimension; i++ )
    {
    if( outputRegionIdx[i] != outputRegionForThreadIdx[i] )
      {
      splitAxis = i;
      }
    }

  // compute the number of pixels before that threads
  SizeType outputRegionSize = output->GetRequestedRegion().GetSize();
  outputRegionSize[splitAxis] = outputRegionForThreadIdx[splitAxis] - outputRegionIdx[splitAxis];
  long firstLineIdForThread = RegionType( outputRegionIdx, outputRegionSize ).GetNumberOfPixels() / xsizeForThread;
  long lineId = firstLineIdForThread;

  OffsetVec LineOffsets;
  SetupLineOffsets(LineOffsets);

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
      if (PVal == m_ForegroundValue)
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
          && inLineIt.Get() == m_ForegroundValue )
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

  m_NumberOfLabels[threadId] = nbOfLabels;

  // wait for the other threads to complete that part
  this->Wait();

  // compute the total number of labels
  nbOfLabels = 0;
  for( int i=0; i<nbOfThreads; i++ )
    {
    nbOfLabels += m_NumberOfLabels[i];
    }
  
  if( threadId == 0 )
    {
    // set up the union find structure
    InitUnion(nbOfLabels);
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
        InsertSet(label);
        label++;
        }
      }
    }

  // wait for the other threads to complete that part
  this->Wait();

  // now process the map and make appropriate entries in an equivalence
  // table
  // assert( linecount == m_LineMap.size() );
  long pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  long xsize = output->GetRequestedRegion().GetSize()[0];
  long linecount = pixelcount/xsize;

  long lastLineIdForThread =  linecount;
  long nbOfLineIdToJoin = 0;
  if( threadId != nbOfThreads - 1 )
    {
    SizeType outputRegionForThreadSize = outputRegionForThread.GetSize();
    outputRegionForThreadSize[splitAxis] -= 1;
    lastLineIdForThread = firstLineIdForThread + RegionType( outputRegionIdx, outputRegionForThreadSize ).GetNumberOfPixels() / xsizeForThread;
    m_FirstLineIdToJoin[threadId] = lastLineIdForThread;
    // found the number of line ids to join
    nbOfLineIdToJoin = RegionType( outputRegionIdx, outputRegionForThread.GetSize() ).GetNumberOfPixels() / xsizeForThread - RegionType( outputRegionIdx, outputRegionForThreadSize ).GetNumberOfPixels() / xsizeForThread;
    }

  for(long ThisIdx = firstLineIdForThread; ThisIdx < lastLineIdForThread; ++ThisIdx)
    {
    if( !m_LineMap[ThisIdx].empty() )
      {
      for (OffsetVec::const_iterator I = LineOffsets.begin();
           I != LineOffsets.end(); ++I)
        {
        long NeighIdx = ThisIdx + (*I);
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
        }
      }
    }
  
  // wait for the other threads to complete that part
  this->Wait();

  while( m_FirstLineIdToJoin.size() != 0 )
    {
    if( threadId * 2 < (long)m_FirstLineIdToJoin.size() )
      {
      for(long ThisIdx = m_FirstLineIdToJoin[threadId * 2];
          ThisIdx < m_FirstLineIdToJoin[threadId * 2] + nbOfLineIdToJoin;
          ++ThisIdx)
        {
        if( !m_LineMap[ThisIdx].empty() )
          {
          for (OffsetVec::const_iterator I = LineOffsets.begin();
              I != LineOffsets.end(); ++I)
            {
            long NeighIdx = ThisIdx + (*I);
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
            }
          }
        }
      }

    this->Wait();

    if( threadId == 0 )
      {
      // remove the region already joined
      typename std::vector< long > newFirstLineIdToJoin;
      for( int i = 1; i<(long)m_FirstLineIdToJoin.size(); i+=2 )
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
  long pixelcount = output->GetRequestedRegion().GetNumberOfPixels();
  long xsize = output->GetRequestedRegion().GetSize()[0];
  long linecount = pixelcount/xsize;
  unsigned long int totalLabs = CreateConsecutive();
  m_ObjectCount = totalLabs;
  ProgressReporter progress(this, 0, linecount);
  // check for overflow exception here
  if( totalLabs > static_cast<unsigned long int>(
          NumericTraits<OutputPixelType>::max() ) )
    {
    itkExceptionMacro(
      << "Number of objects greater than maximum of output pixel type " );
    }

  for (long ThisIdx = 0; ThisIdx<linecount; ThisIdx++)
    {
    // now fill the labelled sections
    typename lineEncoding::const_iterator cIt;

    for (cIt = m_LineMap[ThisIdx].begin();cIt != m_LineMap[ThisIdx].end();++cIt)
      {
      unsigned long Ilab = LookupSet( cIt->label);
      OutputPixelType lab = m_Consecutive[Ilab];
      output->SetLine( cIt->where, cIt->length, lab );
//       std::cout << cIt->where << " " << cIt->length << " " << lab+0.0 << std::endl;
      }
    progress.CompletedPixel();
    }

  m_NumberOfLabels.clear();
  m_Barrier = NULL;
  m_LineMap.clear();
}


template< class TInputImage, class TOutputImage >
void
BinaryImageToLabelMapFilter< TInputImage, TOutputImage >
::SetupLineOffsets(OffsetVec &LineOffsets)
{
  // Create a neighborhood so that we can generate a table of offsets
  // to "previous" line indexes
  // We are going to mis-use the neighborhood iterators to compute the
  // offset for us. All this messing around produces an array of
  // offsets that will be used to index the map
  typename TOutputImage::Pointer output = this->GetOutput();
  typedef Image<long, TOutputImage::ImageDimension - 1>   PretendImageType;
  typedef typename PretendImageType::RegionType::SizeType PretendSizeType;
  typedef typename PretendImageType::RegionType::IndexType PretendIndexType;
  typedef ConstShapedNeighborhoodIterator<PretendImageType>
    LineNeighborhoodType;

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
  long offset = fakeImage->ComputeOffset( idx );

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
  long offset = 0;
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
    long cStart = cIt->where[0];  // the start x position
    long cLast = cStart + cIt->length - 1;

    for (nIt=mIt; nIt != Neighbour.end(); ++nIt)
      {
      //runLength nL = *nIt;
      long nStart = nIt->where[0];
      long nLast = nStart + nIt->length - 1;
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
      long ss1 = nStart - offset;
      // long ss2 = nStart + offset;
      long ee1 = nLast - offset;
      long ee2 = nLast + offset;
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
  m_Consecutive[m_BackgroundValue] = m_BackgroundValue;
  unsigned long int CLab = 0;
  for (unsigned long int I = 1; I < m_UnionFind.size(); I++)
    {
    unsigned long int L = m_UnionFind[I];
    if (L == I) 
      {
      if( CLab == m_BackgroundValue )
        {
        ++CLab;
        }
      m_Consecutive[L] = CLab;
      ++CLab;
      }
    }
  return(CLab);
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
  os << indent << "ObjectCount: "  << m_ObjectCount << std::endl;
  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
}

} // end namespace itk

#endif
