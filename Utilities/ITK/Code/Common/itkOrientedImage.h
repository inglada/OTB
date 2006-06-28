/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOrientedImage.h,v $
  Language:  C++
  Date:      $Date: 2006/04/13 17:57:25 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOrientedImage_h
#define __itkOrientedImage_h

#include "itkImage.h"
#include "itkImageTransformHelper.h"

namespace itk
{

/** \class OrientedImage
 *  \brief Templated n-dimensional oriented image class.
 *
 * \note
 * This work is part of the National Alliance for Medical Image Computing 
 * (NAMIC), funded by the National Institutes of Health through the NIH Roadmap
 * for Medical Research, Grant U54 EB005149.
 *
 * \ingroup ImageObjects */
template <class TPixel, unsigned int VImageDimension>
class ITK_EXPORT OrientedImage : public Image<TPixel, VImageDimension>
{
public:
  /** Standard class typedefs */
  typedef OrientedImage               Self;
  typedef Image<TPixel, VImageDimension>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OrientedImage, Image);

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType  IndexType;

  /** Direction typedef support. The direction cosines of the image. */
  typedef typename Superclass::DirectionType  DirectionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  typedef typename Superclass::AccessorType        AccessorType;
  typedef typename Superclass::AccessorFunctorType AccessorFunctorType;
  typedef typename Superclass::IOPixelType         IOPixelType;

  /** Tyepdef for the functor used to access a neighborhood of pixel pointers.*/
  typedef NeighborhoodAccessorFunctor< Self > 
                                            NeighborhoodAccessorFunctorType;

  /** Return the NeighborhoodAccessor functor. This method is called by the 
   * neighborhood iterators. */
  NeighborhoodAccessorFunctorType GetNeighborhoodAccessor() 
    { return NeighborhoodAccessorFunctorType(); }
  
  /** Return the NeighborhoodAccessor functor. This method is called by the 
   * neighborhood iterators. */
  const NeighborhoodAccessorFunctorType GetNeighborhoodAccessor() const
    { return NeighborhoodAccessorFunctorType(); }
  
   /** Set the spacing of the image and precompute the transforms for
   * the image. */
  virtual void SetSpacing (const SpacingType spacing)
    {
    Superclass::SetSpacing(spacing);

    DirectionType scale;
    for (unsigned int i=0; i < VImageDimension; i++)
      {
      scale[i][i] = this->m_Spacing[i];
      }
    m_IndexToPhysicalPoint = this->m_Direction * scale;
    m_PhysicalPointToIndex = m_IndexToPhysicalPoint.GetInverse();
    }

  virtual void SetSpacing (const double spacing[VImageDimension])
    {
    Superclass::SetSpacing(spacing);

    DirectionType scale;
    for (unsigned int i=0; i < VImageDimension; i++)
      {
      scale[i][i] = this->m_Spacing[i];
      }
    m_IndexToPhysicalPoint = this->m_Direction * scale;
    m_PhysicalPointToIndex = m_IndexToPhysicalPoint.GetInverse();
    }

  virtual void SetSpacing (const float spacing[VImageDimension])
    {
    Superclass::SetSpacing(spacing);

    DirectionType scale;
    for (unsigned int i=0; i < VImageDimension; i++)
      {
      scale[i][i] = this->m_Spacing[i];
      }
    m_IndexToPhysicalPoint = this->m_Direction * scale;
    m_PhysicalPointToIndex = m_IndexToPhysicalPoint.GetInverse();
    }

  /** Set the direction of the image and precompute the transforms for
   * the image. */
  virtual void SetDirection (const DirectionType direction)
    {
    Superclass::SetDirection(direction);

    DirectionType scale;
    for (unsigned int i=0; i < VImageDimension; i++)
      {
      scale[i][i] = this->m_Spacing[i];
      }
    m_IndexToPhysicalPoint = this->m_Direction * scale;
    m_PhysicalPointToIndex = m_IndexToPhysicalPoint.GetInverse();
    }

  /** \brief Get the continuous index from a physical point
   *
   * Returns true if the resulting index is within the image, false otherwise.
   * \sa Transform */
  template<class TCoordRep>
  bool TransformPhysicalPointToContinuousIndex(
              const Point<TCoordRep, VImageDimension>& point,
              ContinuousIndex<TCoordRep, VImageDimension>& index   ) const
    {
    Vector<double, VImageDimension> cvector;

    cvector = m_PhysicalPointToIndex * (point - this->m_Origin);
    for (unsigned int i = 0 ; i < VImageDimension ; i++)
      {
      index[i] = static_cast<TCoordRep>(cvector[i]);
      }

    // Now, check to see if the index is within allowed bounds
    const bool isInside =
      this->GetLargestPossibleRegion().IsInside( index );

    return isInside;
    }

  /** Get the index (discrete) from a physical point.
   * Floating point index results are truncated to integers.
   * Returns true if the resulting index is within the image, false otherwise
   * \sa Transform */
#if 1
  template<class TCoordRep>
  bool TransformPhysicalPointToIndex(
    const Point<TCoordRep, VImageDimension>& point,
    IndexType & index ) const
    {
      ImageTransformHelper<VImageDimension,VImageDimension-1,VImageDimension-1>::TransformPhysicalPointToIndex(
        this->m_PhysicalPointToIndex, this->m_Origin, point, index);

    // Now, check to see if the index is within allowed bounds
    const bool isInside =
      this->GetLargestPossibleRegion().IsInside( index );
    return isInside;
    }
#else
  template<class TCoordRep>
  bool TransformPhysicalPointToIndex(
            const Point<TCoordRep, VImageDimension>& point,
            IndexType & index                                ) const
    {
    typedef typename IndexType::IndexValueType IndexValueType;
    for (unsigned int i = 0; i < VImageDimension; i++)
      {
      index[i] = 0.0;
      for (unsigned int j = 0; j < VImageDimension; j++)
        {
        index[i] += 
          m_PhysicalPointToIndex[i][j] * (point[j] - this->m_Origin[j]);
        }
      }

    // Now, check to see if the index is within allowed bounds
    const bool isInside =
      this->GetLargestPossibleRegion().IsInside( index );

    return isInside;
    }
#endif
  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a continuous index (in the index space)
   * \sa Transform */
  template<class TCoordRep>
  void TransformContinuousIndexToPhysicalPoint(
            const ContinuousIndex<TCoordRep, VImageDimension>& index,
            Point<TCoordRep, VImageDimension>& point        ) const
    {
    Vector<double,VImageDimension> cvector;
    for (unsigned int i = 0 ; i < VImageDimension ; i++)
      {
      cvector[i] = index[i];
      }

    point = this->m_Origin + m_IndexToPhysicalPoint * cvector;
    }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a discrete index (in the index space)
   *
   * \sa Transform */
#if 1
  template<class TCoordRep>
  void TransformIndexToPhysicalPoint(
                      const IndexType & index,
                      Point<TCoordRep, VImageDimension>& point ) const
    {
      ImageTransformHelper<VImageDimension,VImageDimension-1,VImageDimension-1>::TransformIndexToPhysicalPoint(
        this->m_IndexToPhysicalPoint, this->m_Origin, index, point);
    }
#else
  template<class TCoordRep>
  void TransformIndexToPhysicalPoint(
                      const IndexType & index,
                      Point<TCoordRep, VImageDimension>& point ) const
    {
    for (unsigned int i = 0; i < VImageDimension; i++)
      {
      point[i] = this->m_Origin[i];
      for (unsigned int j = 0; j < VImageDimension; j++)
        {
        point[i] += m_IndexToPhysicalPoint[i][j] * index[j];
        }
      }
    }
#endif
protected:
  OrientedImage();
  virtual ~OrientedImage() {};

private:
  OrientedImage(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  DirectionType m_IndexToPhysicalPoint;
  DirectionType m_PhysicalPointToIndex;
};
} // end namespace itk
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOrientedImage.txx"
#endif

#endif

