/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTextureFunctorBase_h
#define __otbTextureFunctorBase_h

#include "otbMath.h"
#include "itkMacro.h"
#include "itkNumericTraits.h"
#include "itkNeighborhood.h"
#include "itkOffset.h"
#include "itkSize.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/** \class TextureFunctorBase
 *  \brief This functor is the base for all texture functors.
 *
 *  It uses a neighborhood iterator which radius is the one of the considered
 *  neighborhood plsu an offset (m_Offset) to compute texture.
 *  It computes the mean, standard deviation of the two areas and the joint histogram using
 *  Scott formula for the bins lengths computation.
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
   * \ingroup Textures
 */

template <class TScalarInputPixelType, class TScalarOutputPixelType>
class TextureFunctorBase
{
public:
  TextureFunctorBase()
  {
    m_Offset.Fill(1);
    m_Mini = itk::NumericTraits<double>::max();
    m_MiniOff = itk::NumericTraits<double>::max();
    m_Maxi = itk::NumericTraits<double>::NonpositiveMin();
    m_MaxiOff = itk::NumericTraits<double>::NonpositiveMin();
    m_Mean = 0.;
    m_MeanOff = 0.;
    m_Std = 0.;
    m_StdOff = 0.;
    m_NeighBinLength = 0;
    m_OffsetBinLength = 0;
    m_Histo.clear();
  }
  virtual ~TextureFunctorBase() {}

  typedef TScalarInputPixelType                       InputScalarType;
  typedef TScalarOutputPixelType                      OutputScalarType;
  typedef itk::VariableLengthVector<InputScalarType>  InputVectorType;
  typedef itk::VariableLengthVector<OutputScalarType> OutputVectorType;
  typedef itk::Offset<>                               OffsetType;
  typedef itk::Size<>                                 RadiusType;
  typedef itk::Neighborhood<InputScalarType, 2>       NeighborhoodType;
  typedef itk::Neighborhood<InputVectorType, 2>       NeighborhoodVectorType;
  typedef std::vector<double>                         DoubleVectorType;
  typedef std::vector<int>                            IntVectorType;
  typedef std::vector<IntVectorType>                  IntVectorVectorType;

  void SetOffset(OffsetType off){ m_Offset = off; }
  OffsetType GetOffset(){ return m_Offset; }
  double GetMaxi(){ return m_Maxi; }
  double GetMini(){ return m_Mini; }
  double GetMaxiOff(){ return m_MaxiOff; }
  double GetMiniOff(){ return m_MiniOff; }
  double GetMean(){ return m_Mean; }
  double GetMeanOff(){ return m_MeanOff; }
  double GetStd(){ return m_Std; }
  double GetStdOff(){ return m_StdOff; }
  double GetNeighBinLength(){ return m_NeighBinLength; }
  double GetOffsetBinLength(){ return m_OffsetBinLength; }
  IntVectorVectorType GetHisto(){ return m_Histo; }

  /** Computes the histogram bins using Scott formula, plus min/max, means, stds */
  void StatComputation(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    double binLength    = 0;
    double binLengthOff = 0;
    m_Mean = 0.;
    m_MeanOff = 0.;
    m_Std = 0.;
    m_StdOff = 0.;
    RadiusType radius = neigh.GetRadius();
    m_Mini = itk::NumericTraits<double>::max();
    m_MiniOff = itk::NumericTraits<double>::max();
    m_Maxi = itk::NumericTraits<double>::NonpositiveMin();
    m_MaxiOff = itk::NumericTraits<double>::NonpositiveMin();
    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double scottCoef =  3.5 / (vcl_pow(area, 1 / 3));

    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0] + this->GetOffset()[0] - 1;
    offsetOffInit[1] = -radius[1] + this->GetOffset()[1] - 1;

    offsetOff = offsetOffInit;
    for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++)
      {
      offsetOff[0]++;
      offsetOff[1] = offsetOffInit[1];
      offset[0] = l;
      for (int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); ++k)
        {
        offsetOff[1]++;
        offset[1] = k;
        m_Mean += static_cast<double>(neigh[offset]);
        m_MeanOff += static_cast<double>(neighOff[offsetOff]);

        m_Mini    = std::min(static_cast<double>(neigh[offset]),      m_Mini);
        m_Maxi    = std::max(static_cast<double>(neigh[offset]),      m_Maxi);
        m_MiniOff = std::min(static_cast<double>(neighOff[offsetOff]), m_MiniOff);
        m_MaxiOff = std::max(static_cast<double>(neighOff[offsetOff]), m_MaxiOff);
        }
      }
    m_Mean *= areaInv;
    m_MeanOff *= areaInv;

    offsetOff = offsetOffInit;

    for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++)
      {
      offsetOff[0]++;
      offsetOff[1] = offsetOffInit[1];
      offset[0] = l;
      for (int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); ++k)
        {
        offsetOff[1]++;
        offset[1] = k;
        binLength += vcl_pow((m_Mean - static_cast<double>(neigh[offset])), 2);
        binLengthOff += vcl_pow((m_MeanOff - static_cast<double>(neighOff[offsetOff])), 2);
        }
      }

    binLength *= areaInv;
    binLength = vcl_sqrt(binLength);
    binLengthOff *= areaInv;
    binLengthOff = vcl_sqrt(binLengthOff);
    m_Std = binLength;
    m_StdOff = binLengthOff;

    m_NeighBinLength = scottCoef * binLength;
    m_OffsetBinLength = scottCoef * binLengthOff;
  }

  inline OutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    RadiusType radiusOff = neigh.GetRadius();
    //OutputScalarType outPix;
    //outPix.SetSize( neigh.GetCenterPixel().GetSize() );
    //outPix.Fill(0);
    OffsetType offset;
    offset.Fill(0);
    // Compute the neighborhood radius from the neigh+offset iterator to extract the neighborhood area from the iterator
    RadiusType radius;
    radius[0] =
      static_cast<unsigned int>(0.5 *
                                (static_cast<double>(neigh.GetSize()[0] - 1)) - static_cast<double>(vcl_abs(m_Offset[0])));
    radius[1] =
      static_cast<unsigned int>(0.5 *
                                (static_cast<double>(neigh.GetSize()[1] - 1)) - static_cast<double>(vcl_abs(m_Offset[1])));

    NeighborhoodType inNeigh;
    inNeigh.SetRadius(radius);
    NeighborhoodType offNeigh;
    offNeigh.SetRadius(radiusOff);
    // Extract the neighborhood area
    for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++)
      {
      offset[0] = l;
      for (int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); ++k)
        {
        offset[1] = k;
        inNeigh[offset] =  neigh[offset];    //neigh.GetPixel(offset);
        }
      }
    // Extract the offset area
    offset.Fill(0);
    for (int l = -static_cast<int>(radiusOff[0]); l <= static_cast<int>(radiusOff[0]); l++)
      {
      offset[0] = l;
      for (int k = -static_cast<int>(radiusOff[1]); k <= static_cast<int>(radiusOff[1]); ++k)
        {
        offset[1] = k;
        offNeigh[offset] =  neigh[offset];    //neigh.GetPixel(offset);
        }
      }
    OutputScalarType outPix = static_cast<OutputScalarType>(this->ComputeOverSingleChannel(inNeigh, offNeigh));

    return outPix;
  }

  inline OutputVectorType operator ()(const NeighborhoodVectorType& neigh)
  {
    RadiusType       radiusOff = neigh.GetRadius();
    OutputVectorType outPix;
    outPix.SetSize(neigh.GetCenterValue/*Pixel*/ ().GetSize());
    outPix.Fill(0);
    OffsetType offset;
    offset.Fill(0);
    // Compute the neighborhood radius from the neigh+offset iterator to extract the neighborhood area from the iterator
    RadiusType radius;
    radius[0] =
      static_cast<unsigned int>(0.5 *
                                (static_cast<double>(neigh.GetSize()[0] - 1)) - static_cast<double>(vcl_abs(m_Offset[0])));
    radius[1] =
      static_cast<unsigned int>(0.5 *
                                (static_cast<double>(neigh.GetSize()[1] - 1)) - static_cast<double>(vcl_abs(m_Offset[1])));

    // For each channel
    for (unsigned int i = 0; i < neigh.GetCenterValue/*Pixel*/ ().GetSize(); ++i)
      {
      NeighborhoodType inNeigh;
      inNeigh.SetRadius(radius);
      NeighborhoodType offNeigh;
      offNeigh.SetRadius(radiusOff);
      // Extract the neighborhood area
      for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++)
        {
        offset[0] = l;
        for (int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); ++k)
          {
          offset[1] = k;
          inNeigh[offset] = neigh[offset][i];        //neigh.GetPixel(offset)[i];
          }
        }
      // Extract the offset area
      offset.Fill(0);
      for (int l = -static_cast<int>(radiusOff[0]); l <= static_cast<int>(radiusOff[0]); l++)
        {
        offset[0] = l;
        for (int k = -static_cast<int>(radiusOff[1]); k <= static_cast<int>(radiusOff[1]); ++k)
          {
          offset[1] = k;
          offNeigh[offset] = neigh[offset][i];
          }
        }
      outPix[i] = static_cast<OutputScalarType>(this->ComputeOverSingleChannel(inNeigh, offNeigh));
      }
    return outPix;
  }

  void ComputeJointHistogram(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    this->StatComputation(neigh, neighOff);

    RadiusType radius = neigh.GetRadius();
    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0] + this->GetOffset()[0] - 1;
    offsetOffInit[1] = -radius[1] + this->GetOffset()[1] - 1;

    int histoIdX = 0;
    int histoIdY = 0;

    IntVectorType histoTemp;
    if (m_NeighBinLength != 0) histoTemp =
        IntVectorType(static_cast<unsigned int>(vcl_floor(static_cast<double>(this->GetMaxi() -
                                                                              this->GetMini()) / m_NeighBinLength) + 1.),
                      0);
    else histoTemp = IntVectorType(1, 0);

    if (m_OffsetBinLength != 0) m_Histo =
        IntVectorVectorType(static_cast<unsigned int>(vcl_floor(static_cast<double>(this->GetMaxiOff() -
                                                                                    this->GetMiniOff()) /
                                                                m_OffsetBinLength) + 1.), histoTemp);
    else m_Histo = IntVectorVectorType(1, histoTemp);

    offsetOff = offsetOffInit;
    for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++)
      {
      offsetOff[0]++;
      offsetOff[1] = offsetOffInit[1];
      offset[0] = l;
      for (int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); ++k)
        {
        offsetOff[1]++;
        offset[1] = k;
        histoIdX = 0;
        histoIdY = 0;
        if (m_OffsetBinLength != 0) histoIdX =
            static_cast<int>(vcl_floor((static_cast<double>(neighOff[offsetOff]) -
                                        this->GetMiniOff()) / static_cast<double>(m_OffsetBinLength)));
        if (m_NeighBinLength != 0) histoIdY =
            static_cast<int>(vcl_floor((static_cast<double>(neigh[offset]) -
                                        this->GetMini()) / static_cast<double>(m_NeighBinLength)));

        m_Histo[histoIdX][histoIdY]++;

        }
      }
  }

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff) = 0;

private:
  OffsetType m_Offset;
  /** Stores min/max neighborhood area values */
  double m_Mini;
  double m_Maxi;
  /** Stores min/max neighborhood+offset values */
  double m_MiniOff;
  double m_MaxiOff;
  /** Stores mean of neighborhood+offset */
  double m_Mean;
  double m_MeanOff;
  /** Stores standard deviation of neighborhood+offset */
  double m_Std;
  double m_StdOff;
  /** Bins length */
  double m_NeighBinLength;
  double m_OffsetBinLength;
  /** Joint histogramm */
  IntVectorVectorType m_Histo;
};

} // namespace Functor
} // namespace otb

#endif
