/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImage.h,v $
  Language:  C++
  Date:      $Date: 2008-02-04 12:34:11 $
  Version:   $Revision: 1.143 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImage_h
#define __itkImage_h

#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "itkImportImageContainer.h"
#include "itkDefaultPixelAccessor.h"
#include "itkDefaultPixelAccessorFunctor.h"
#include "itkPoint.h"
#include "itkContinuousIndex.h"
#include "itkFixedArray.h"
#include "itkWeakPointer.h"
#include "itkNeighborhoodAccessorFunctor.h"

namespace itk
{
/** \class Image
 *  \brief Templated n-dimensional image class.
 *
 * Images are templated over a pixel type (modeling the dependent
 * variables), and a dimension (number of independent variables).  The
 * container for the pixel data is the ImportImageContainer.
 *
 * Within the pixel container, images are modeled as arrays, defined by a
 * start index and a size.
 *
 * There are three sets of meta-data describing an image. These are "Region"
 * objects that define a portion of an image via a starting index for the image
 * array and a size. The ivar LargestPossibleRegion defines the size and
 * starting index of the image dataset. The entire image dataset, however,
 * need not be resident in memory. The region of the image that is resident in
 * memory is defined by the "BufferedRegion". The Buffer is a contiguous block
 * of memory.  The third set of meta-data defines a region of interest, called
 * the "RequestedRegion". The RequestedRegion is used by the pipeline
 * execution model to define what a filter is requested to produce.
 *
 * [RegionIndex, RegionSize] C [BufferIndex, BufferSize]
 *                           C [ImageIndex, ImageSize]
 *
 * Pixels can be accessed direcly using the SetPixel() and GetPixel()
 * methods or can be accessed via iterators.  Begin() creates
 * an iterator that can walk a specified region of a buffer.
 *
 * The pixel type may be one of the native types; a Insight-defined
 * class type such as Vector; or a user-defined type. Note that
 * depending on the type of pixel that you use, the process objects
 * (i.e., those filters processing data objects) may not operate on
 * the image and/or pixel type. This becomes apparent at compile-time
 * because operator overloading (for the pixel type) is not supported.
 *
 * The data in an image is arranged in a 1D array as [][][][slice][row][col]
 * with the column index varying most rapidly.  The Index type reverses
 * the order so that with Index[0] = col, Index[1] = row, Index[2] = slice,
 * ...
 *
 * \sa ImageContainerInterface
 *
 * \example DataRepresentation/Image/Image1.cxx
 * \example DataRepresentation/Image/Image2.cxx
 * \example DataRepresentation/Image/Image2.cxx
 * \example DataRepresentation/Image/RGBImage.cxx
 * \example DataRepresentation/Image/VectorImage.cxx
 *
 * \ingroup ImageObjects */
template <class TPixel, unsigned int VImageDimension=2>
class ITK_EXPORT Image : public ImageBase<VImageDimension>
{
public:
  /** Standard class typedefs */
  typedef Image                        Self;
  typedef ImageBase<VImageDimension>   Superclass;
  typedef SmartPointer<Self>           Pointer;
  typedef SmartPointer<const Self>     ConstPointer;
  typedef WeakPointer<const Self>      ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Image, ImageBase);

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef TPixel PixelType;

  /** Typedef alias for PixelType */
  typedef TPixel ValueType;

  /** Internal Pixel representation. Used to maintain a uniform API
   * with Image Adaptors and allow to keep a particular internal
   * representation of data while showing a different external
   * representation. */
  typedef TPixel InternalPixelType;

  typedef PixelType IOPixelType;

  /** Accessor type that convert data between internal and external
   *  representations.  */
  typedef DefaultPixelAccessor< PixelType >    AccessorType;
  typedef DefaultPixelAccessorFunctor< Self >  AccessorFunctorType;

  /** Typedef for the functor used to access a neighborhood of pixel
   * pointers. */
  typedef NeighborhoodAccessorFunctor< Self >  NeighborhoodAccessorFunctorType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Container used to store pixels in the image. */
  typedef ImportImageContainer<unsigned long, PixelType> PixelContainer;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType       IndexType;
  typedef typename Superclass::IndexValueType  IndexValueType;

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

  /** A pointer to the pixel container. */
  typedef typename PixelContainer::Pointer        PixelContainerPointer;
  typedef typename PixelContainer::ConstPointer   PixelContainerConstPointer;

  /** Offset typedef (relative position between indices) */
  typedef typename Superclass::OffsetValueType OffsetValueType;

  /** Allocate the image memory. The size of the image must
   * already be set, e.g. by calling SetRegions(). */
  void Allocate();

  /** Convenience methods to set the LargestPossibleRegion,
   *  BufferedRegion and RequestedRegion. Allocate must still be called.
   */
  void SetRegions(RegionType region)
    {
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  void SetRegions(SizeType size)
    {
    RegionType region; region.SetSize(size);
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  /** Restore the data object to its initial state. This means releasing
   * memory. */
  virtual void Initialize();

  /** Fill the image buffer with a value.  Be sure to call Allocate()
   * first. */
  void FillBuffer (const TPixel& value);

  /** \brief Set a pixel value.
   *
   * Allocate() needs to have been called first -- for efficiency,
   * this function does not check that the image has actually been
   * allocated yet. */
  void SetPixel(const IndexType &index, const TPixel& value)
    {
    typename Superclass::OffsetValueType offset = this->ComputeOffset(index);
    (*m_Buffer)[offset] = value;
    }

  /** \brief Get a pixel (read only version).
   *
   * For efficiency, this function does not check that the
   * image has actually been allocated yet. */
  const TPixel& GetPixel(const IndexType &index) const
    {
    typename Superclass::OffsetValueType offset = this->ComputeOffset(index);
    return ( (*m_Buffer)[offset] );
    }

  /** \brief Get a reference to a pixel (e.g. for editing).
   *
   * For efficiency, this function does not check that the
   * image has actually been allocated yet. */
  TPixel& GetPixel(const IndexType &index)
    {
    typename Superclass::OffsetValueType offset = this->ComputeOffset(index);
    return ( (*m_Buffer)[offset] );
    }

  /** \brief Access a pixel. This version can be an lvalue.
   *
   * For efficiency, this function does not check that the
   * image has actually been allocated yet. */
  TPixel & operator[](const IndexType &index)
    { return this->GetPixel(index); }

  /** \brief Access a pixel. This version can only be an rvalue.
   *
   * For efficiency, this function does not check that the
   * image has actually been allocated yet. */
  const TPixel& operator[](const IndexType &index) const
     { return this->GetPixel(index); }

  /** Return a pointer to the beginning of the buffer.  This is used by
   * the image iterator class. */
  TPixel *GetBufferPointer()
    { return m_Buffer ? m_Buffer->GetBufferPointer() : 0; }
  const TPixel *GetBufferPointer() const
    { return m_Buffer ? m_Buffer->GetBufferPointer() : 0; }

  /** Return a pointer to the container. */
  PixelContainer* GetPixelContainer()
    { return m_Buffer.GetPointer(); }

  const PixelContainer* GetPixelContainer() const
    { return m_Buffer.GetPointer(); }

  /** Set the container to use. Note that this does not cause the
   * DataObject to be modified. */
  void SetPixelContainer( PixelContainer *container );

  /** Graft the data and information from one image to another. This
   * is a convenience method to setup a second image with all the meta
   * information of another image and use the same pixel
   * container. Note that this method is different than just using two
   * SmartPointers to the same image since separate DataObjects are
   * still maintained. This method is similar to
   * ImageSource::GraftOutput(). The implementation in ImageBase
   * simply calls CopyInformation() and copies the region ivars.
   * The implementation here refers to the superclass' implementation
   * and then copies over the pixel container. */
  virtual void Graft(const DataObject *data);

  
  /** Return the Pixel Accessor object */
  AccessorType GetPixelAccessor( void ) 
    { return AccessorType(); }

  /** Return the Pixel Accesor object */
  const AccessorType GetPixelAccessor( void ) const
    { return AccessorType(); }

  /** Return the NeighborhoodAccessor functor */
  NeighborhoodAccessorFunctorType GetNeighborhoodAccessor() 
    { return NeighborhoodAccessorFunctorType(); }
  
  /** Return the NeighborhoodAccessor functor */
  const NeighborhoodAccessorFunctorType GetNeighborhoodAccessor() const
    { return NeighborhoodAccessorFunctorType(); }
  

  /** \brief Get the continuous index from a physical point
   *
   * Returns true if the resulting index is within the image, false otherwise.
   * \sa Transform */
  template<class TCoordRep>
  bool TransformPhysicalPointToContinuousIndex(
              const Point<TCoordRep, VImageDimension>& point,
              ContinuousIndex<TCoordRep, VImageDimension>& index   ) const
    {
    // Update the output index
    for (unsigned int i = 0; i < VImageDimension; i++)
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
            const Point<TCoordRep, VImageDimension>& point,
            IndexType & index                                ) const
    {
    // Update the output index
    for (unsigned int i = 0; i < VImageDimension; i++)
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
            const ContinuousIndex<TCoordRep, VImageDimension>& index,
            Point<TCoordRep, VImageDimension>& point        ) const
    {
    for (unsigned int i = 0; i < VImageDimension; i++)
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
                      Point<TCoordRep, VImageDimension>& point ) const
    {
    for (unsigned int i = 0; i < VImageDimension; i++)
      {
      point[i] = static_cast<TCoordRep>( this->m_Spacing[i] *
        static_cast<double>( index[i] ) + this->m_Origin[i] );
      }
    }

  /** Take a vector or covariant vector that has been computed in the
   * coordinate system parallel to the image grid and rotate it by the
   * direction cosines in order to get it in terms of the coordinate system of
   * the image acquisition device.  This implementation in the Image only needs
   * to copy the input vector or covariant vector given that the Image class
   * implicitly has an Identity Matrix as direction cosines. The arguments of
   * the method are of type FixedArray to make possible to use this method with
   * both Vector and CovariantVector. The Method is implemented differently in
   * the itk::OrientedImage. 
   *
   * \sa OrientedImage
   */ 
  template<class TCoordRep>
  void TransformLocalVectorToPhysicalVector(
    const FixedArray<TCoordRep, VImageDimension> & inputGradient,
          FixedArray<TCoordRep, VImageDimension> & outputGradient ) const
    {
    for( unsigned int i = 0; i < VImageDimension; i++ )
      {
      outputGradient[i] = inputGradient[i];
      }
    }

protected:
  Image();
  void PrintSelf(std::ostream& os, Indent indent) const;
  virtual ~Image() {};
private:
  Image(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Memory for the current buffer. */
  PixelContainerPointer m_Buffer;
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_Image(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT Image< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef Image< ITK_TEMPLATE_2 x > Image##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkImage+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkImage.txx"
#endif

#endif
