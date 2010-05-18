/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSharedMorphologyUtilities.txx,v $
  Language:  C++
  Date:      $Date: 2009-10-29 15:03:32 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSharedMorphologyUtilities_txx
#define __itkSharedMorphologyUtilities_txx

#include "itkSharedMorphologyUtilities.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include <list>

namespace itk {

/**
 * \class SharedMorphUtilities 
 * \brief functionality in common for anchor and VanHerkGilWerman openings/closings and
 * erosions/dilation
 *
 */

template <class TRegion, class TLine>
bool NeedToDoFace(const TRegion AllImage,
                  const TRegion face,
                  const TLine line)
{
  // can't use the continuous IsInside (even if I could get it to
  // work) because on the edge doesn't count as inside for this test

  // If the component of the vector orthogonal to the face doesn't go
  // inside the image then we can ignore the face
  
  // find the small dimension of the face - should only be one
  typename TRegion::IndexType ISt = AllImage.GetIndex();

  typename TRegion::SizeType FSz = face.GetSize();
  typename TRegion::IndexType FSt = face.GetIndex();

  unsigned smallDim = 0;
  for (unsigned i = 0; i < AllImage.GetImageDimension(); i++)
    {
    if (FSz[i] == 1)
      {
      smallDim = i;
      break;
      }
    }
  long startI = ISt[smallDim];
  long facePos = FSt[smallDim] + FSz[smallDim] - 1;
  if (facePos == startI) 
    {
    // at the start of dimension - vector must be positive
    if (line[smallDim] > 0.000001) return true;  
    // some small angle that we consider to be zero - should be more rigorous
    }
  else
    {
    // at the end of dimension - vector must be positive
    if (line[smallDim] < -0.000001) return true;  
    }
  return (false);
  
}
template <class TImage, class TBres, class TLine>
int ComputeStartEnd(const typename TImage::IndexType StartIndex,
                    const TLine line,
                    const float tol,
                    const typename TBres::OffsetArray LineOffsets,
                    const typename TImage::RegionType AllImage, 
                    unsigned &start,
                    unsigned &end)
{
  // compute intersection between ray and box
  typename TImage::IndexType ImStart = AllImage.GetIndex();
  typename TImage::SizeType ImSize = AllImage.GetSize();
  float Tfar = NumericTraits<float>::max();
  float Tnear  = NumericTraits<float>::NonpositiveMin();
  float domdir  = NumericTraits<float>::NonpositiveMin();
  int sPos, ePos;
  unsigned perpdir = 0;
  for (unsigned i = 0; i< TImage::RegionType::ImageDimension; i++)
    {
    if (vcl_fabs(line[i]) > domdir)
      {
      domdir = vcl_fabs(line[i]);
      perpdir = i;
      }
    if (vcl_fabs(line[i]) > tol)
      {
      int P1 = ImStart[i] - StartIndex[i];
      int P2 = ImStart[i] + ImSize[i] - 1 - StartIndex[i];
      float T1 = ((float)(P1))/line[i];
      float T2 = ((float)(P2))/line[i];
      
      if (T1 > T2)
        {
        // T1 is meant to be the near face
        std::swap(T1, T2);
        }
      // want the farthest Tnear and nearest TFar
      if (T1 > Tnear) 
        {
        Tnear = T1;  
        }
      if (T2 < Tfar) 
        {
        Tfar = T2;
        }
      }
    else
      {
      // parallel to an axis - check for intersection at all
      if ((StartIndex[i] < ImStart[i]) || (StartIndex[i] > ImStart[i] + (int)ImSize[i] - 1))
        {
        // no intersection
        start=end=0;
        return(0);
        }
      }
    }
  sPos = (int)(Tnear*vcl_fabs(line[perpdir]) + 0.5);
  ePos = (int)(Tfar*vcl_fabs(line[perpdir]) + 0.5);
  
  //std::cout << Tnear << " " << Tfar << std::endl;
  if (Tfar < Tnear) // seems to need some margin
    {
    // in theory, no intersection, but search between them
    bool intersection = false;
    unsigned inside;
    if (Tnear - Tfar < 10)
      {
//      std::cout << "Searching " << Tnear << " " << Tfar << std::endl;
      assert(ePos >= 0);
      assert(sPos < (int)LineOffsets.size());
      for (int i = ePos; i<= sPos; i++)
        {
        if (AllImage.IsInside(StartIndex + LineOffsets[i]))
          {
          inside = i;
          intersection=true;
          break;
          }
        }
      }
    if (intersection)
      {
//      std::cout << "Found intersection after all :: " << inside << std::endl;
      sPos = ePos = inside;
      assert(ePos + 1 >= 0);
      assert(ePos + 1 < (int)LineOffsets.size());
      while (AllImage.IsInside(StartIndex + LineOffsets[ePos + 1]))
        {
        ++ePos;
        assert(ePos + 1 >= 0);
        assert(ePos + 1 < (int)LineOffsets.size());
        }
      assert(sPos - 1 >= 0);
      assert(sPos - 1 < (int)LineOffsets.size());
      while (AllImage.IsInside(StartIndex + LineOffsets[sPos - 1]))
        {
        --sPos;
        assert(sPos - 1 >= 0);
        assert(sPos - 1 < (int)LineOffsets.size());
        }
      start = sPos;
      end = ePos;
      }
    else
      {
//      std::cout << StartIndex << "No intersection" << std::endl;
      start=end=0;
      return(0);
      }
    }
  else
    {
    
    assert(sPos >= 0);
    assert(sPos < (int)LineOffsets.size());
    if (AllImage.IsInside(StartIndex + LineOffsets[sPos]))
      {
      for (;sPos>0;)
        {
        assert(sPos - 1 >= 0);
        assert(sPos - 1 < (int)LineOffsets.size());
        if (!AllImage.IsInside(StartIndex + LineOffsets[sPos - 1])) break;
        else --sPos;
        }
      }
    else
      {
      for(;sPos<(int)LineOffsets.size();)
        {
        assert(sPos >= 0);
        assert(sPos < (int)LineOffsets.size());
        ++sPos;
        if (!AllImage.IsInside(StartIndex + LineOffsets[sPos])) ++sPos;
        else break;
        }
      }
    if (AllImage.IsInside(StartIndex + LineOffsets[ePos]))
      {
      for(;ePos<(int)LineOffsets.size();)
        {
        assert(ePos + 1 >= 0);
        assert(ePos + 1 < (int)LineOffsets.size());
        if (!AllImage.IsInside(StartIndex + LineOffsets[ePos + 1])) break;
        else ++ePos;
        }
      }
    else
      {
      for (;ePos>0;)
        {
        --ePos;
        assert(ePos >= 0);
        assert(ePos < (int)LineOffsets.size());
        if (!AllImage.IsInside(StartIndex + LineOffsets[ePos])) --ePos;
        else break;
        }
      }
    }
  start = sPos;
  end = ePos;
  return (1);
}


template <class TImage, class TBres>
void CopyLineToImage(const typename TImage::Pointer output,
                     const typename TImage::IndexType StartIndex,
                     const typename TBres::OffsetArray LineOffsets,
                     const typename TImage::PixelType * outbuffer,
                     const unsigned start,
                     const unsigned end)
{
  unsigned size = end - start + 1;
  for (unsigned i = 0; i <size; i++)
    {
    assert(start + i >= 0);
    assert(start + i < LineOffsets.size());
#if 1
    output->SetPixel(StartIndex + LineOffsets[start + i], outbuffer[i+1]);  //compat
#else
    typename TImage::IndexType I = StartIndex + LineOffsets[start + i];
    output->SetPixel(I, 1 + output->GetPixel(I));
#endif
    }
}


template <class TInputImage, class TLine>
typename TInputImage::RegionType
MakeEnlargedFace(const typename TInputImage::ConstPointer itkNotUsed( input ),
               const typename TInputImage::RegionType AllImage,
               const TLine line)
{
#if 0
  // use the face calculator to produce a face list
  typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>
FaceCalculatorType;
  FaceCalculatorType faceCalculator;
  typename TInputImage::SizeType radius;
  radius.Fill(1);
  typename FaceCalculatorType::FaceListType faceList;
  faceList = faceCalculator(input, AllImage, radius);
  typename FaceCalculatorType::FaceListType::iterator fit;
  fit = faceList.begin();
  ++fit;
#else
  // the face list calculator strategy fails in multithreaded mode
  // with 1D kernels
  // because it doesn't return faces of the sub-blocks if they don't
  // fall along the edge of the image
  typedef typename TInputImage::RegionType      RegionType;
  typedef typename TInputImage::SizeType        SizeType;
  typedef typename TInputImage::IndexType       IndexType;
  typedef std::list<RegionType>                 FaceListType;
  FaceListType faceList;

  for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
    {
    RegionType R1, R2;
    SizeType S1 = AllImage.GetSize();
    IndexType I2 = AllImage.GetIndex();

    S1[i]=1;
    R1 = AllImage;
    R2 = AllImage;

    // the first face will have the same starting index and one
    // dimension removed
    R1.SetSize(S1);
    
    I2[i] = I2[i] + AllImage.GetSize()[i] - 1;
    R2.SetSize(S1);
    R2.SetIndex(I2);
    faceList.push_back(R1);
    faceList.push_back(R2);
//    std::cout << R1 << R2 << std::endl;
    }
  typename FaceListType::iterator fit;
  fit = faceList.begin();
#endif
  typename TInputImage::RegionType RelevantRegion;
  bool foundFace = false;
  float MaxComp = NumericTraits<float>::NonpositiveMin();
  unsigned DomDir = 0;
  //std::cout << "------------" << std::endl;
  // figure out the dominant direction of the line
  for (unsigned i = 0;i< TInputImage::RegionType::ImageDimension;i++) 
    {
    if (vcl_fabs(line[i]) > MaxComp)
      {
      MaxComp = vcl_fabs(line[i]);
      DomDir = i;
      }
    }
  
  for (;fit != faceList.end();++fit) 
    {
    // check whether this face is suitable for parallel sweeping - i.e
    // whether the line is within 45 degrees of the perpendicular
    // Figure out the perpendicular using the region size
    unsigned FaceDir = 0;
    //    std::cout << "Face " << *fit << std::endl;
    for (unsigned i = 0;i< TInputImage::RegionType::ImageDimension;i++) 
      {
      if (fit->GetSize()[i] == 1) FaceDir = i;
      }
    if (FaceDir == DomDir) // within 1 degree 
      {
      // now check whether the line goes inside the image from this face
      if ( NeedToDoFace<ITK_TYPENAME TInputImage::RegionType, TLine>(AllImage, *fit, line) ) 
        {
//        std::cout << "Using face: " << *fit << line << std::endl;
        RelevantRegion = *fit;
        foundFace = true;
        break;
        }
      }
    }
  if (foundFace) 
    {
    // enlarge the region so that sweeping the line across it will
    // cause all pixels to be visited.
    // find the dimension not within the face
    unsigned NonFaceDim = 0;
    
    for (unsigned i = 0; i < TInputImage::RegionType::ImageDimension;i++) 
      {
      if (RelevantRegion.GetSize()[i] == 1)
        {
        NonFaceDim=i;
        break;
        }
      }

    // figure out how much extra each other dimension needs to be extended
    typename TInputImage::SizeType NewSize = RelevantRegion.GetSize();
    typename TInputImage::IndexType NewStart = RelevantRegion.GetIndex();
    unsigned NonFaceLen = AllImage.GetSize()[NonFaceDim];
    for (unsigned i = 0; i < TInputImage::RegionType::ImageDimension;i++) 
      {
      if (i != NonFaceDim)
        {
        int Pad = Math::Ceil<int>((float)(NonFaceLen) * line[i]/vcl_fabs(line[NonFaceDim])); 
        if (Pad < 0)
          {
          // just increase the size - no need to change the start
          NewSize[i] += abs(Pad) + 1;
          }
        else 
          {
          // change the size and index
          NewSize[i] += Pad + 1;
          NewStart[i] -= Pad + 1;
          }
        }
      }
    RelevantRegion.SetSize(NewSize);
    RelevantRegion.SetIndex(NewStart);
    }
  else 
    {
    std::cout << "Line " << line << " doesnt correspond to a face" << std::endl;
    }
//  std::cout << "Result region = " << RelevantRegion << std::endl;

//  std::cout << "+++++++++++++++++" << std::endl;
  return RelevantRegion;
}

template <class TImage, class TBres, class TLine>
int FillLineBuffer(typename TImage::ConstPointer input,
                   const typename TImage::IndexType StartIndex,
                   const TLine line,  // unit vector
                   const float tol,
                   const typename TBres::OffsetArray LineOffsets,
                   const typename TImage::RegionType AllImage, 
                   typename TImage::PixelType * inbuffer,
                   unsigned int &start,
                   unsigned int &end)
{

//   if (AllImage.IsInside(StartIndex))
//     {
//     start = 0;
//     }
//   else
  
#if 0
  // we need to figure out where to start
  // this is an inefficient way we'll use for testing
  for (start=0;start < LineOffsets.size();++start)
    {
    if (AllImage.IsInside(StartIndex + LineOffsets[start])) break;
    }
#else
  int status = ComputeStartEnd<TImage, TBres, TLine>(StartIndex, line, tol, LineOffsets, AllImage,
                                                     start, end);
  if (!status) return(status);
#endif
#if 1
  unsigned size = end - start + 1;
  // compat
  for (unsigned i = 0; i < size;i++)
    {
    assert(start + i >= 0);
    assert(start + i < LineOffsets.size());
    inbuffer[i+1] = input->GetPixel(StartIndex + LineOffsets[start + i]);
    }
#else
  typedef ImageRegionConstIteratorWithIndex<TImage> ItType;
  ItType it(input, AllImage);
  it.SetIndex(StartIndex);
  for (unsigned i = 0; i < lastPos;i++)
    {
    inbuffer[i]= it.Get();
    assert(i >= 0);
    assert(i < LineOffsets.size());
    typename TImage::IndexType I = StartIndex + LineOffsets[i];
    typename TImage::OffsetType Off = I - it.GetIndex();
    it += Off;
    }
#endif
  return(1);
}

template <class TLine>
unsigned int GetLinePixels(const TLine line)
{
  float N = line.GetNorm();
  float correction = 0.0;
  
  for (unsigned int i = 0; i < TLine::Dimension; i++)
    {
    float tt = vcl_fabs(line[i]/N);
    if (tt > correction) correction=tt;
    }
  
  N *= correction;
  return (int)(N + 0.5);
}

} // namespace itk

#endif
