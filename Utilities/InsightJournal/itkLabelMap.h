/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMap.h,v $
  Language:  C++
  Date:      $Date: 2006/04/20 14:54:09 $
  Version:   $Revision: 1.136 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMap_h
#define __itkLabelMap_h

#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "itkFixedArray.h"
#include "itkWeakPointer.h"
#include <map>

namespace itk
{
/** \class LabelMap
 *  \brief Templated n-dimensional image to store labeled objects.
 *
 * LabelMap is an image class specialized in storing the labeled
 * images. It represent the image in a different way than itk::Image.
 * Instead of storing the content of the image in an array of pixels values,
 * it store the a collection of labeled objects, and a background
 * value.
 * This way of storing the content of the image allow an easy and efficient
 * manipulation of the objects in the image.
 *
 * The LabelMap shares a lot of methods with the itk::Image class.
 * it make it usable as input or output of the itk::ImageToImageFilter for example.
 * However the methods don't have the same complexity in the 2 classes, because
 * of the different way to store the data. GetPixel() is run in constant time
 * for example in itk::Image, but have a worst case complexity of O(L), where
 * L is the number of lines in the image (imageSize[1] * imageSize[2] for a 3D
 * image).
 *
 * \ingroup ImageObjects */
template <class TLabelObject >
class ITK_EXPORT LabelMap : public ImageBase<TLabelObject::ImageDimension>
{
public:
  /** Standard class typedefs */
  typedef LabelMap               Self;
  typedef ImageBase<TLabelObject::ImageDimension>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMap, ImageBase);

  typedef TLabelObject LabelObjectType;

  typedef typename LabelObjectType::Pointer LabelObjectPointerType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, LabelObjectType::ImageDimension);

  /** Label typedef support. */
  typedef typename LabelObjectType::LabelType  LabelType;
  typedef LabelType PixelType;

  /** the LabelObject container type */
  typedef std::map< LabelType, LabelObjectPointerType > LabelObjectContainerType;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType  IndexType;

  /** Offset typedef support. An offset is used to access pixel values. */
  typedef typename Superclass::OffsetType OffsetType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType  SizeType;

  /** Direction typedef support. A matrix of direction cosines. */
  typedef typename Superclass::DirectionType  DirectionType;

  /** Region typedef support. A region is used to specify a subset of an image. */
  typedef typename Superclass::RegionType  RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename Superclass::PointType PointType;

  /** Offset typedef (relative position between indices) */
  typedef typename Superclass::OffsetValueType OffsetValueType;

  /** Convenience methods to set the LargestPossibleRegion,
   *  BufferedRegion and RequestedRegion. Allocate must still be called.
   */
  void SetRegions( const RegionType & region )
    {
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  void SetRegions( const SizeType & size )
    {
    RegionType region; region.SetSize(size);
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  /** Restore the data object to its initial state. This means releasing
   * memory. */
  virtual void Initialize();

  /**  */
  virtual void Allocate();

  virtual void Graft(const DataObject *data);

  /** \brief Get the continuous index from a physical point
   *
   * Returns true if the resulting index is within the image, false otherwise.
   * \sa Transform */
  template<class TCoordRep>
  bool TransformPhysicalPointToContinuousIndex(
              const Point<TCoordRep, ImageDimension>& point,
              ContinuousIndex<TCoordRep, ImageDimension>& index   ) const
    {
    // Update the output index
    for (unsigned int i = 0 ; i < ImageDimension ; i++)
      {
      index[i] = static_cast<TCoordRep>( (point[i]- this->m_Origin[i]) / this->m_Spacing[i] );
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
  template<class TCoordRep>
  bool TransformPhysicalPointToIndex(
            const Point<TCoordRep, ImageDimension>& point,
            IndexType & index                                ) const
    {
    typedef typename IndexType::IndexValueType IndexValueType;

    // Update the output index
    for (unsigned int i = 0 ; i < ImageDimension ; i++)
      {
      index[i] = static_cast<IndexValueType>( (point[i]- this->m_Origin[i]) / this->m_Spacing[i] );
      }

    // Now, check to see if the index is within allowed bounds
    const bool isInside =
      this->GetLargestPossibleRegion().IsInside( index );

    return isInside;
    }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a continuous index (in the index space)
   * \sa Transform */
  template<class TCoordRep>
  void TransformContinuousIndexToPhysicalPoint(
            const ContinuousIndex<TCoordRep, ImageDimension>& index,
            Point<TCoordRep, ImageDimension>& point        ) const
    {
    for (unsigned int i = 0 ; i < ImageDimension ; i++)
      {
      point[i] = static_cast<TCoordRep>( this->m_Spacing[i] * index[i] + this->m_Origin[i] );
      }
    }

  /** Get a physical point (in the space which
   * the origin and spacing infomation comes from)
   * from a discrete index (in the index space)
   *
   * \sa Transform */
  template<class TCoordRep>
  void TransformIndexToPhysicalPoint(
                      const IndexType & index,
                      Point<TCoordRep, ImageDimension>& point ) const
    {
    for (unsigned int i = 0 ; i < ImageDimension ; i++)
      {
      point[i] = static_cast<TCoordRep>( this->m_Spacing[i] *
        static_cast<double>( index[i] ) + this->m_Origin[i] );
      }
    }


  /**
   * Return the LabelObject with the label given in parameter.
   * This method thorws an exception if the label doesn't exist in this image,
   * or if the label is the background one.
   */
  LabelObjectType * GetLabelObject( const LabelType & label );

  const LabelObjectType * GetLabelObject( const LabelType & label ) const;

  /**
   * Return true is the image contains the label given in parameter and false
   * otherwise. If the label is the background one, true is also returned, so
   * this method may not be a good enough test before calling GetLabelObject().
   */
  bool HasLabel( const LabelType label ) const;
  
  /**
   * Return the LabelObject with at the position given in parameter.
   * This method can be useful when the labels are not consecutives, but is quite
   * inefficient.
   * This method thorws an exception if the index doesn't exist in this image.
   */
  LabelObjectType * GetNthLabelObject( const unsigned long & pos );

  const LabelObjectType * GetNthLabelObject( const unsigned long & pos ) const;

  /**
   * Return the pixel value at a given index in the image. This method
   * has a worst case complexity of O(L) where L is the number of lines in the
   * image - use it with care.
   */
  const LabelType & GetPixel( const IndexType & idx ) const;
  
  /**
   * Set the pixel value at a given index in the image.
   * If no label object has this pixel value, a new label object is created. If
   * a label object already exist, the index is added to it.
   * The worst case complexity of this method is O(L) where L is the number of
   * lines in the image. However, the execution time will be quite low if the
   * pixels are set in the image in raster mode.
   */
  void SetPixel( const IndexType & idx, const LabelType & label );

  /**
   * Set a full line in the image. If no label object has this label in the image,
   * a new object is created. If a label object already exist with that label, the
   * line is added to it WITHOUT any check - it means that if the label object may
   * contain several time the same pixel after have run that method.
   * This method runs in constant time.
   */
  void SetLine( const IndexType & idx, const unsigned long & length, const LabelType & label );

  /**
   * Return the label object at a given index. This method
   * has a worst case complexity of O(L) where L is the number of lines in the
   * image - use it with care.
   */
  LabelObjectType * GetLabelObject( const IndexType & idx ) const;
  
  /**
   * Add a label object to the image. If a label object already has the label,
   * it is overiden.
   */
  void AddLabelObject( LabelObjectType * labelObject );
  
  /**
   * Add a label object to the image. The label of the label object is 
   * ignored, and a new label is given to the label object.
   */
  void PushLabelObject( LabelObjectType * labelObject );
  
  /**
   * Remove a label object.
   */
  void RemoveLabelObject( LabelObjectType * labelObject );
  
  /**
   * Remove a label object.
   */
  void RemoveLabel( const LabelType & label );

  /**
   * Remove all the labels in the image
   */
  void ClearLabels();

  /**
   * Return the label object container
   */
  const LabelObjectContainerType & GetLabelObjectContainer() const;

  /**
   * Return the numbner of label objects in the image
   */
  unsigned long GetNumberOfLabelObjects() const;
  
  /**
   * Set/Get the background label
   */
  itkGetConstMacro(BackgroundValue, LabelType);
  itkSetMacro(BackgroundValue, LabelType);
  
  /**
   * Print all the objects stored in that collection - a convenient method
   * for prototyping.
   */
  void PrintLabelObjects(std::ostream& os) const;
  void PrintLabelObjects() const
    {
    this->PrintLabelObjects( std::cerr );
    }

protected:
  LabelMap();
  virtual ~LabelMap() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  LabelMap(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LabelObjectContainerType m_LabelObjectContainer;
  LabelType m_BackgroundValue;
};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
# include "itkLabelMap.txx"
#endif

#endif
