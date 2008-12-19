/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWrapPadImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-09 15:31:37 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWrapPadImageFilter_txx
#define __itkWrapPadImageFilter_txx

#include "itkWrapPadImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "itkFastMutexLock.h"
#include "itkProgressReporter.h"
#include <vector>

namespace itk
{
  
/** Used for mutex locking */
static SimpleFastMutexLock VectorMutex;

/**
 * Given an n dimensional list of output region breakpoints in indices
 * and size (where the current region and maximum region for each dimension
 * is encoded in regIndices and regLimit), choose the next output region.
 */ 
template <class TInputImage, class TOutputImage>
int WrapPadImageFilter<TInputImage,TOutputImage>
::GenerateNextOutputRegion(long *regIndices, long *regLimit, 
                           std::vector<long>* indices,
                           std::vector<long>* sizes,
                           OutputImageRegionType& outputRegion)
{
  unsigned int ctr;
  int done = 0;
  OutputImageIndexType nextIndex = outputRegion.GetIndex();
  OutputImageSizeType nextSize = outputRegion.GetSize();
    
  //
  // Starting at the first dimension, increment the counter and set a new 
  // value for the region parameters.  If we wrap on a region, then we 
  // also increment to the next region for the next higher dimension.
  //
  for (ctr=0; (ctr<ImageDimension) && !done; ctr++)
    {
    regIndices[ctr]++;
    done = 1;
    if (regIndices[ctr] >= regLimit[ctr]) 
      {
      regIndices[ctr] = 0;
      done = 0;
      }
    nextIndex[ctr] = indices[ctr][regIndices[ctr]];
    nextSize[ctr] = sizes[ctr][regIndices[ctr]];
    }
    
  //
  // Set what we have learned into the image region.
  //
  outputRegion.SetIndex(nextIndex);
  outputRegion.SetSize(nextSize);
    
  // 
  // If any dimension has zero size, then we do not need to process this
  // region.  Report this back to the calling routine.
  //
  for (ctr=0; ctr<ImageDimension; ctr++)
    {
    if (nextSize[ctr] == 0)
      {
      return 0;
      }
    }
    
  return 1;
}

/**
 * Given an n dimensional list of input region breakpoints in indices
 * and size (where the current region and maximum region for each dimension
 * is encoded in regIndices and regLimit), choose the next input region.
 */ 
template <class TInputImage, class TOutputImage>
int WrapPadImageFilter<TInputImage,TOutputImage>
::GenerateNextInputRegion(long *regIndices, long *regLimit, 
                          std::vector<long>* indices, 
                          std::vector<long>* sizes,
                          InputImageRegionType& inputRegion)
{
  unsigned int ctr;
  int done = 0;
  InputImageIndexType nextIndex = inputRegion.GetIndex();
  InputImageSizeType nextSize = inputRegion.GetSize();
  
  //
  // Starting at the first dimension, increment the counter and set a new 
  // value for the region parameters.  If we wrap on a region, then we 
  // also increment to the next region for the next higher dimension.
  //
  for (ctr=0; (ctr<ImageDimension) && !done; ctr++)
    {
    regIndices[ctr]++;
    done = 1;
    if (regIndices[ctr] >= regLimit[ctr]) 
      {
      regIndices[ctr] = 0;
      done = 0;
      }
    nextIndex[ctr] = indices[ctr][regIndices[ctr]];
    nextSize[ctr] = sizes[ctr][regIndices[ctr]];
    }
  
  //
  // Set what we have learned into the image region.
  //
  inputRegion.SetIndex(nextIndex);
  inputRegion.SetSize(nextSize);
  
  // 
  // If any dimension has zero size, then we do not need to process this
  // region.  Report this back to the calling routine.
  //
  for (ctr=0; ctr<ImageDimension; ctr++)
    {
    if (nextSize[ctr] == 0)
      {
      return 0;
      }
    }
  
  return 1;
}

/**
 * Given the start and end indices of a region, determine how many
 * instances of size fit within the region.  The variable offset provides
 * a way to adjust width of the area while forcing alignment to the
 * start or end location.
 */
template <class TInputImage, class TOutputImage>
int
WrapPadImageFilter<TInputImage,TOutputImage>
::FindRegionsInArea(long start, long end, long size, long offset)
{
  int result = 1;
  long regionsize;
  
  regionsize = end - start;
  if (regionsize > 0)  // Find out home many regions we have,
    {
    result = regionsize / size;
    result++;
    if (offset > 0)
      {
      result = result - (offset/size);
      }
    }
  
  return result;
}

/**
 * Generate region 0 (inter-region) information.  Based on the indices
 * of the input and the output for this dimension, decide what are the 
 * starting points and the lengths of the output region directly 
 * corresponding to the input region.  Padding will be on either 
 * side of this region.  The algorithmic complications are necessary
 * to support the streaming interface and multithreading.
 */
template <class TInputImage, class TOutputImage>
int 
WrapPadImageFilter<TInputImage,TOutputImage>
::BuildInterRegions(std::vector<long>& inputRegionStart, 
                    std::vector<long>& outputRegionStart,
                    std::vector<long>& inputRegionSizes, 
                    std::vector<long>& outputRegionSizes,
                    long inputIndex, long outputIndex,
                    long inputSize, long outputSize, 
                    int numRegs, int & regCtr)
{
  long sizeTemp;  // Holder for current size calculation.

  // Region 0 is between, which has a starting index equal to 
  // the input region starting index, unless that would be
  // outside the bounds of the output image.
  if (inputIndex > outputIndex) 
    {
    outputRegionStart[0] = inputIndex;
    inputRegionStart[0] = inputIndex;
    }
  else
    {
    outputRegionStart[0] = outputIndex;
    inputRegionStart[0] = outputIndex;
    }
  
  // Size of the in region is the area from index 0 to the end of the 
  // input or the output, whichever comes first.
  if ((inputIndex+inputSize) 
      < (outputIndex+outputSize)) 
    {
    sizeTemp = inputIndex + inputSize - outputRegionStart[0];
    }
  else
    {
    sizeTemp = outputIndex + outputSize - outputRegionStart[0];
    }
  outputRegionSizes[0] = ((sizeTemp > 0) ? sizeTemp:0);
  inputRegionSizes[0] = ((sizeTemp > 0) ? sizeTemp:0);
  
  regCtr = numRegs;
  return 1;
}

/**
 * Generate region 1 (pre-region) information.  Based on the indices
 * of the input and the output for this dimension, decide what are the 
 * starting points and the lengths of the output region directly 
 * preceding the input region in this dimension.  This may require
 * more than one region be defined if the padding is larger than the
 * size of the input image in this dimension.  Other algorithmic 
 * complications are necessary to support the streaming interface 
 * and multithreading.
 */
template <class TInputImage, class TOutputImage>
int 
WrapPadImageFilter<TInputImage,TOutputImage>
::BuildPreRegions(std::vector<long>& inputRegionStart, 
                  std::vector<long>& outputRegionStart,
                  std::vector<long>& inputRegionSizes, 
                  std::vector<long>& outputRegionSizes,
                  long inputIndex, long outputIndex,
                  long inputSize, long outputSize, 
                  int numRegs, int & regCtr)
{
  long sizeTemp;  // Holder for current size calculation.
  int ctr;       // Generic loop counter.
  
  // Handle the pre-region.  Within the pre-region, the first and last
  // groups may be truncated and only contain the back part of the input
  // data.  All other regions will be complete copies of the input.
  outputRegionStart[regCtr] = outputIndex;
  
  // Size of the pre-region is all the output that preceeds the input, 
  // all except the first (and possibly the last) will be the size of 
  // the input.
  sizeTemp = outputRegionStart[0] - outputIndex;
  sizeTemp = ((sizeTemp > 0) ? (sizeTemp % inputSize) : 0);
  outputRegionSizes[regCtr] = sizeTemp;
  inputRegionSizes[regCtr] = sizeTemp;
  inputRegionStart[regCtr] = inputIndex + inputSize - sizeTemp;
  // Handle the rest of the pre-region by stepping through in blocks of
  // the size of the input image.
  for (ctr=1; ctr<numRegs; ctr++) 
    {
    regCtr++;
    outputRegionStart[regCtr] = outputRegionStart[regCtr-1]
      + static_cast<long>(outputRegionSizes[regCtr-1]);
    inputRegionStart[regCtr] = inputIndex;
    outputRegionSizes[regCtr] = inputSize;
    inputRegionSizes[regCtr] = inputSize;
    }
  // Fix size on last region, if necessary.
  if ((outputRegionStart[regCtr]+static_cast<long>(outputRegionSizes[regCtr])) 
      > (outputIndex+outputSize)) 
    {
    outputRegionSizes[regCtr] = outputIndex + outputSize 
      - outputRegionStart[regCtr];
    inputRegionSizes[regCtr] = outputRegionSizes[regCtr];
    }

  return regCtr;
}

/**
 * Generate region 2 (post-region) information.  Based on the indices
 * of the input and the output for this dimension, decide what are the 
 * starting points and the lengths of the output region directly 
 * succeeding the input region in this dimension.  This may require
 * more than one region be defined if the padding is larger than the
 * size of the input image in this dimension.  Other algorithmic 
 * complications are necessary to support the streaming interface 
 * and multithreading.
 */
template <class TInputImage, class TOutputImage>
int 
WrapPadImageFilter<TInputImage,TOutputImage>
::BuildPostRegions(std::vector<long>& inputRegionStart, 
                   std::vector<long>& outputRegionStart,
                   std::vector<long>& inputRegionSizes, 
                   std::vector<long>& outputRegionSizes,
                   long inputIndex, long outputIndex,
                   long inputSize, long outputSize, 
                   int numRegs, int & regCtr)
{
  long sizeTemp;  // Holder for current size calculation.
  int ctr;       // Generic loop counter.

  // Handle the post region.  The post region has a number of
  // areas of size equal to the input region, followed by one
  // region of possibly smaller size.
  regCtr++;
  sizeTemp = outputIndex + outputSize - inputIndex - inputSize;
  sizeTemp = ((sizeTemp>0) ? (sizeTemp % inputSize):0);
  outputRegionSizes[regCtr] = sizeTemp;
  inputRegionSizes[regCtr] = sizeTemp;
  outputRegionStart[regCtr] = outputIndex + outputSize - sizeTemp;
  inputRegionStart[regCtr] = inputIndex;
  
  for (ctr=numRegs-1; ctr>=1; ctr--) 
    {
    regCtr++;
    outputRegionStart[regCtr] = outputRegionStart[regCtr-1] - inputSize;
    inputRegionStart[regCtr] = inputIndex;
    outputRegionSizes[regCtr] = inputSize;
    inputRegionSizes[regCtr] = inputSize;
    }
  // Fix size on last region, if necessary.
  if (outputRegionStart[regCtr] < outputIndex)
    {
    sizeTemp = outputIndex - outputRegionStart[regCtr];
    outputRegionStart[regCtr] += sizeTemp;
    inputRegionStart[regCtr] += sizeTemp;
    outputRegionSizes[regCtr] -= sizeTemp;
    inputRegionSizes[regCtr] = outputRegionSizes[regCtr];
    }
  
  return regCtr;
}


/** 
 * WrapPadImageFilter needs a different input requested region than
 * output requested region.  As such, WrapPadImageFilter needs to
 * provide an implementation for GenerateInputRequestedRegion() in
 * order to inform the pipeline execution model.
 *
 * \sa PadImageFilter::GenerateInputRequestedRegion() 
 * \sa ProcessObject::GenerateInputRequestedRegion() 
 */
template <class TInputImage, class TOutputImage>
void 
WrapPadImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  unsigned int dimCtr;
  int regCtr;

  // call the superclass' implementation of this method
  // Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  InputImagePointer  inputPtr = 
    const_cast< InputImageType * >( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  
  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  OutputImageIndexType outputIndex =
    outputPtr->GetRequestedRegion().GetIndex();
  InputImageIndexType inputIndex =
    inputPtr->GetLargestPossibleRegion().GetIndex();
  OutputImageSizeType outputSize =
    outputPtr->GetRequestedRegion().GetSize();
  InputImageSizeType inputSize =
    inputPtr->GetLargestPossibleRegion().GetSize();
  
  OutputImageRegionType outputRegion; 
  InputImageRegionType inputRegion;
  
  // For n dimensions, there are k^n combinations of before, between, and
  // after on these regions.  We are keeping this flexible so that we 
  // can handle other blockings imposed by the mirror and wrap algorithms.
  long inRegLimit[ImageDimension];
  long outRegLimit[ImageDimension];
  long minIndex[ImageDimension], maxIndex[ImageDimension];
  int numPre[ImageDimension], numPost[ImageDimension], numIn[ImageDimension];
  std::vector<long> outputRegionStart[ImageDimension];
  std::vector<long> outputRegionSizes[ImageDimension];  
  std::vector<long> inputRegionStart[ImageDimension];
  std::vector<long> inputRegionSizes[ImageDimension];
  
  // Calculate the actual number of regions for each dimension,
  // and set up the required variables here.
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    numIn[dimCtr] = 1;  // Always assume exactly one inter region.
    numPre[dimCtr] =    // Count how many versions of input fit in pre-pad
      this->FindRegionsInArea(outputIndex[dimCtr], inputIndex[dimCtr],
                              static_cast<long>(inputSize[dimCtr]), 
                              inputIndex[dimCtr]-outputIndex[dimCtr]
                              - static_cast<long>(outputSize[dimCtr]));
    numPost[dimCtr] =   // Count how many versions of input fit in post-pad
      this->FindRegionsInArea(inputIndex[dimCtr]+static_cast<long>(inputSize[dimCtr]),
                              outputIndex[dimCtr]+static_cast<long>(outputSize[dimCtr]),
                              static_cast<long>(inputSize[dimCtr]),
                              outputIndex[dimCtr]-inputIndex[dimCtr]
                              - static_cast<long>(inputSize[dimCtr]));
    inRegLimit[dimCtr] = numPre[dimCtr] + numIn[dimCtr] + numPost[dimCtr];
    outRegLimit[dimCtr] = numPre[dimCtr] + numIn[dimCtr] + numPost[dimCtr];
    outputRegionStart[dimCtr].resize(outRegLimit[dimCtr]);
    outputRegionSizes[dimCtr].resize(outRegLimit[dimCtr]);
    inputRegionStart[dimCtr].resize(inRegLimit[dimCtr]);
    inputRegionSizes[dimCtr].resize(inRegLimit[dimCtr]);
    }
  
  //
  // Generate the break points for the image regions we counted in the 
  // previous loop.
  //
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    //
    // Generate region 0 (inter-region) information.  Based on the indices
    // of the input and the output for this dimension, decide what are the 
    // starting points and the lengths of the output region directly 
    // corresponding to the input region.  Padding will be on either 
    // side of this region.
    //
    regCtr = BuildInterRegions(inputRegionStart[dimCtr], 
                               outputRegionStart[dimCtr],
                               inputRegionSizes[dimCtr], 
                               outputRegionSizes[dimCtr],
                               inputIndex[dimCtr], 
                               outputIndex[dimCtr],
                               static_cast<long>(inputSize[dimCtr]), 
                               static_cast<long>(outputSize[dimCtr]), 
                               numIn[dimCtr], regCtr);
      
    //
    // Generate region 1 (pre-region) information for that part of the
    // output image which precedes the input image in this dimension.
    //
    regCtr = BuildPreRegions(inputRegionStart[dimCtr], 
                             outputRegionStart[dimCtr],
                             inputRegionSizes[dimCtr], 
                             outputRegionSizes[dimCtr],
                             inputIndex[dimCtr], 
                             outputIndex[dimCtr],
                             static_cast<long>(inputSize[dimCtr]), 
                             static_cast<long>(outputSize[dimCtr]), 
                             numPre[dimCtr], regCtr);
      
    //
    // Generate region 2 (post-region) information for that part of the
    // output image which succeeds the input image in this dimension.
    //
    regCtr = BuildPostRegions(inputRegionStart[dimCtr], 
                              outputRegionStart[dimCtr],
                              inputRegionSizes[dimCtr], 
                              outputRegionSizes[dimCtr],
                              inputIndex[dimCtr], 
                              outputIndex[dimCtr],
                              static_cast<long>(inputSize[dimCtr]), 
                              static_cast<long>(outputSize[dimCtr]), 
                              numPost[dimCtr], regCtr);
    }
  
  //
  // Pick the indices which span the largest input region we need for this
  // output region.
  //
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    minIndex[dimCtr] = inputRegionStart[dimCtr][0]; 
    maxIndex[dimCtr] = minIndex[dimCtr] + static_cast<long>(inputRegionSizes[dimCtr][0]);
      
    for (regCtr=1; 
         regCtr<(numIn[dimCtr]+numPre[dimCtr]+numPost[dimCtr]); 
         regCtr++)
      {
      if (minIndex[dimCtr] == maxIndex[dimCtr])
        {
        minIndex[dimCtr] = inputRegionStart[dimCtr][regCtr]; 
        maxIndex[dimCtr] = minIndex[dimCtr] 
          + static_cast<long>(inputRegionSizes[dimCtr][regCtr]);
        }
      else
        {
        if (inputRegionStart[dimCtr][regCtr] < minIndex[dimCtr]) 
          {
          minIndex[dimCtr] = inputRegionStart[dimCtr][regCtr];
          }
        if ((inputRegionStart[dimCtr][regCtr]
             +static_cast<long>(inputRegionSizes[dimCtr][regCtr]))
            > maxIndex[dimCtr])
          {
          maxIndex[dimCtr] = inputRegionStart[dimCtr][regCtr]
            + static_cast<long>(inputRegionSizes[dimCtr][regCtr]);
          }
        }
      }
    }
  
  typename TInputImage::SizeType  inputRequestedRegionSize;
  typename TInputImage::IndexType inputRequestedRegionStartIndex;
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    inputRequestedRegionStartIndex[dimCtr] = minIndex[dimCtr];
    inputRequestedRegionSize[dimCtr] = maxIndex[dimCtr] - minIndex[dimCtr];
    }

  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.SetIndex( inputRequestedRegionStartIndex );
  
  inputPtr->SetRequestedRegion( inputRequestedRegion );
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
WrapPadImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  unsigned int dimCtr;
  int regCtr;
  int numRegions=1; // Actual number of regions in our decomposed space.
  int goodInput, goodOutput;
  // Are the regions non-empty?
  
  itkDebugMacro(<<"Actually executing");
  
  // Get the input and output pointers
  InputImageConstPointer  inputPtr  = this->GetInput();
  OutputImagePointer      outputPtr = this->GetOutput();
  
  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  OutputImageIndexType outputIndex = outputRegionForThread.GetIndex();
  InputImageIndexType inputIndex 
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  OutputImageSizeType outputSize = outputRegionForThread.GetSize();
  InputImageSizeType inputSize 
    = inputPtr->GetLargestPossibleRegion().GetSize();
  
  OutputImageRegionType outputRegion; 
  InputImageRegionType inputRegion;
  
  // For n dimensions, there are k^n combinations of before, between, and
  // after on these regions.  We are keeping this flexible so that we 
  // can handle other blockings imposed by the mirror and wrap algorithms.
  long inRegIndices[ImageDimension];
  long inRegLimit[ImageDimension];
  long outRegIndices[ImageDimension];
  long outRegLimit[ImageDimension];
  int numPre[ImageDimension], numPost[ImageDimension], numIn[ImageDimension];
  std::vector<long> outputRegionStart[ImageDimension];
  std::vector<long> outputRegionSizes[ImageDimension];  
  std::vector<long> inputRegionStart[ImageDimension];
  std::vector<long> inputRegionSizes[ImageDimension];
  
  // Calculate the actual number of regions for each dimension,
  // and set up the required variables here.
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    numIn[dimCtr] = 1;  // Always assume exactly one inter region.
    numPre[dimCtr] =    // Count how many versions of input fit in pre-pad
      this->FindRegionsInArea(outputIndex[dimCtr], inputIndex[dimCtr],
                              static_cast<long>(inputSize[dimCtr]), 
                              inputIndex[dimCtr]-outputIndex[dimCtr]
                              - static_cast<long>(outputSize[dimCtr]));
    numPost[dimCtr] =   // Count how many versions of input fit in post-pad
      this->FindRegionsInArea(inputIndex[dimCtr]+static_cast<long>(inputSize[dimCtr]),
                              outputIndex[dimCtr]+static_cast<long>(outputSize[dimCtr]),
                              static_cast<long>(inputSize[dimCtr]),
                              outputIndex[dimCtr]-inputIndex[dimCtr]
                              - static_cast<long>(inputSize[dimCtr]));
    inRegLimit[dimCtr] = numPre[dimCtr] + numIn[dimCtr] + numPost[dimCtr];
    inRegIndices[dimCtr] = inRegLimit[dimCtr] - 1;
    outRegLimit[dimCtr] = numPre[dimCtr] + numIn[dimCtr] + numPost[dimCtr];
    outRegIndices[dimCtr] = outRegLimit[dimCtr] - 1;
    numRegions *= outRegLimit[dimCtr];
    outputRegionStart[dimCtr].resize(outRegLimit[dimCtr]);
    outputRegionSizes[dimCtr].resize(outRegLimit[dimCtr]);
    inputRegionStart[dimCtr].resize(inRegLimit[dimCtr]);
    inputRegionSizes[dimCtr].resize(inRegLimit[dimCtr]);
    }
  
  //
  // Generate the break points for the image regions we counted in the 
  // previous loop.
  //
  for (dimCtr=0; dimCtr<ImageDimension; dimCtr++) 
    {
    //
    // Generate region 0 (inter-region) information.  Based on the indices
    // of the input and the output for this dimension, decide what are the 
    // starting points and the lengths of the output region directly 
    // corresponding to the input region.  Padding will be on either 
    // side of this region.
    //
    regCtr = BuildInterRegions(inputRegionStart[dimCtr], 
                               outputRegionStart[dimCtr],
                               inputRegionSizes[dimCtr], 
                               outputRegionSizes[dimCtr],
                               inputIndex[dimCtr], 
                               outputIndex[dimCtr],
                               static_cast<long>(inputSize[dimCtr]), 
                               static_cast<long>(outputSize[dimCtr]), 
                               numIn[dimCtr], regCtr);

    //
    // Generate region 1 (pre-region) information for that part of the
    // output image which precedes the input image in this dimension.
    //
    regCtr = BuildPreRegions(inputRegionStart[dimCtr], 
                             outputRegionStart[dimCtr],
                             inputRegionSizes[dimCtr], 
                             outputRegionSizes[dimCtr],
                             inputIndex[dimCtr], 
                             outputIndex[dimCtr],
                             static_cast<long>(inputSize[dimCtr]), 
                             static_cast<long>(outputSize[dimCtr]), 
                             numPre[dimCtr], regCtr);

    //
    // Generate region 2 (post-region) information for that part of the
    // output image which succeeds the input image in this dimension.
    //
    regCtr = BuildPostRegions(inputRegionStart[dimCtr], 
                              outputRegionStart[dimCtr],
                              inputRegionSizes[dimCtr], 
                              outputRegionSizes[dimCtr],
                              inputIndex[dimCtr], 
                              outputIndex[dimCtr],
                              static_cast<long>(inputSize[dimCtr]), 
                              static_cast<long>(outputSize[dimCtr]), 
                              numPost[dimCtr], regCtr);
    }
  
  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
  // Define/declare iterators that will walk the input and output regions
  // for this thread.  
  typedef ImageRegionIterator<TOutputImage>     OutputIterator;
  typedef ImageRegionConstIterator<TInputImage> InputIterator;
  
  // Now walk the regions.
  for (regCtr=0; regCtr<numRegions; 
       regCtr++)
    {
    // If both a valid output and input region are defined for the particular
    // defined region, then copy the input values to the output values.
    goodOutput = this->GenerateNextOutputRegion
      (outRegIndices,outRegLimit,outputRegionStart,
       outputRegionSizes,outputRegion);
    goodInput = this->GenerateNextInputRegion
      (inRegIndices,inRegLimit,inputRegionStart,
       inputRegionSizes,inputRegion);
    if (goodInput && goodOutput)
      {
      OutputIterator outIt = OutputIterator(outputPtr, outputRegion);
      InputIterator  inIt  = InputIterator(inputPtr, inputRegion);
          
      // Do the actual copy of the input pixels to the output
      // pixels here.
      while (!outIt.IsAtEnd() )
        {
        // copy the input pixel to the output
        outIt.Set( inIt.Get() );
        ++outIt;
        ++inIt;
        progress.CompletedPixel();
        }
      } 
    }
}

} // end namespace itk

#endif
