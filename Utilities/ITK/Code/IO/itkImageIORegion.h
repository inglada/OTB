/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageIORegion.h,v $
  Language:  C++
  Date:      $Date: 2007/08/22 17:47:20 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageIORegion_h
#define __itkImageIORegion_h

#include <algorithm>
#include "itkRegion.h"
#include "itkObjectFactory.h"
#include "itkImageRegion.h"

namespace itk
{

/** \class ImageIORegion
 * \brief An ImageIORegion represents a structured region of data.
 *
 * ImageIORegion is an class that represents some structured portion or
 * piece of an Image. The ImageIORegion is represented with an index and
 * a size in each of the n-dimensions of the image. (The index is the
 * corner of the image, the size is the lengths of the image in each of
 * the topological directions.)  ImageIORegion is not templated over
 * dimension, but uses dynamic arrays instead.
 *
 * \sa Region
 * \sa ImageRegion
 * \sa Index
 * \sa Size
 * \sa MeshRegion
 */

class ITK_EXPORT ImageIORegion: public Region
{
public:
  /** Standard class typedefs. */
  typedef ImageIORegion Self;
  typedef Region        Superclass;

  /** Standard part of all itk objects. */
  itkTypeMacro(ImageIORegion, Region);

  /** Dimension of the image available at run time. */
  unsigned int GetImageDimension() const
    {
    return m_ImageDimension;
    }

  /** Dimension of the region to be written. This differs from the
   * the image dimension and is calculated at run-time by examining
   * the size of the image in each coordinate direction. */
  unsigned int GetRegionDimension() const
    {  
    unsigned int dim=0;
    for (unsigned int i=0; i<m_ImageDimension; i++)
      {
      if ( m_Size[i] > 1 ) dim++;
      }
    return dim;
    }

  /** Index typedef support. An index is used to access pixel values. */
  typedef std::vector<long>  IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef std::vector<long>  SizeType;
  
  /** Return the region type. Images are described with structured regions. */
  virtual Superclass::RegionType GetRegionType() const
  {return Superclass::ITK_STRUCTURED_REGION;}

  /** Constructor. ImageIORegion is a lightweight object that is not reference
   * counted, so the constructor is public. */
  ImageIORegion(unsigned int dimension)
    {
    m_ImageDimension = dimension;
    m_Index.resize(m_ImageDimension);
    m_Size.resize(m_ImageDimension);
    std::fill(m_Index.begin(), m_Index.end(), 0);
    std::fill(m_Size.begin(), m_Size.end(), 0);
    }
  
  /** Constructor. ImageIORegion is a lightweight object that is not reference
   * counted, so the constructor is public.  Default dimension is 2. */
  ImageIORegion()
    {
    m_ImageDimension = 2;
    m_Index.resize(2);
    m_Size.resize(2);
    std::fill(m_Index.begin(), m_Index.end(), 0);
    std::fill(m_Size.begin(), m_Size.end(), 0);
    }
  
  /** Destructor. ImageIORegion is a lightweight object that is not reference
   * counted, so the destructor is public. */
  virtual ~ImageIORegion(){};

  /** Copy constructor. ImageIORegion is a lightweight object that is not
   * reference counted, so the copy constructor is public. */
  ImageIORegion(const Self& region): Region()
    { 
    m_Index =region.m_Index; 
    m_Size = region.m_Size; 
    m_ImageDimension = region.m_ImageDimension;
    }
  
  /** operator=. ImageIORegion is a lightweight object that is not reference
   * counted, so operator= is public. */
  void operator=(const Self& region) 
    {
    m_Index = region.m_Index;  
    m_Size = region.m_Size;
    m_ImageDimension = region.m_ImageDimension;
    }

  /** Set the index defining the corner of the region. */
  void SetIndex(const IndexType &index) 
    {
    m_Index = index;\
    }

  /** Get index defining the corner of the region. */
  const IndexType& GetIndex() const
    {
    return m_Index;
    }
  
  /** Set the size of the region. This plus the index determines the
   * rectangular shape, or extent, of the region. */
  void SetSize(const SizeType &size)
    {
    m_Size = size;
    }

  /** Get the size of the region. */
  const SizeType& GetSize() const
    {
    return m_Size;
    }

  /** Convenience methods to get the size of the image in a particular
   * coordinate direction i. Do not try to access image sizes beyond the
   * the ImageDimension. */
  long GetSize(unsigned long i) const
    {
    return m_Size[i];
    }
  long GetIndex(unsigned long i) const
    {
    return m_Index[i];
    }
  void SetSize(const unsigned long i, long size)
    {
    m_Size[i] = size;
    }
  void SetIndex(const unsigned long i, long idx)
    {
    m_Index[i] = idx;
    }

  /** Compare two regions. */
  bool
  operator==(const Self &region) const
    {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    same = same && (m_ImageDimension == region.m_ImageDimension);
    return same;
    }

  /** Compare two regions. */
  bool
  operator!=(const Self &region) const
    {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    same = same && (m_ImageDimension == region.m_ImageDimension);
    return !same;
    }
  
  /** Test if an index is inside */
  bool
  IsInside(const IndexType &index) const
    {
    for(unsigned int i=0; i<m_ImageDimension; i++)
      {
      if( index[i] < m_Index[i] ) 
        {
        return false;
        }
      if( index[i] >= m_Index[i] + m_Size[i] ) 
        {
        return false;
        }
      }
    return true;
    }
 
  /** Get the number of pixels contained in this region. This just
   * multiplies the size components. */
//  unsigned long GetNumberOfPixels() const;

protected:
  /** Methods invoked by Print() to print information about the object
   * including superclasses. Typically not called by the user (use Print()
   * instead) but used in the hierarchical print process to combine the
   * output of several classes.  */
  virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:
  unsigned int      m_ImageDimension;
  std::vector<long> m_Index;
  std::vector<long> m_Size;
};


// Declare operator<<
extern std::ostream & operator<<(std::ostream &os, const ImageIORegion &region); 

/** \class ImageIORegionAdaptor
 * \brief Helper class for converting ImageRegions into ImageIORegions and back.
 *
 **/
template< unsigned int VDimension >
class ImageIORegionAdaptor
{
public:
  typedef ImageRegion<VDimension>  ImageRegionType;
  typedef ImageIORegion            ImageIORegionType;

  typedef typename ImageRegionType::SizeType  ImageSizeType;
  typedef typename ImageRegionType::IndexType ImageIndexType;

  static void Convert( const ImageRegionType & inImageRegion, ImageIORegionType & outIORegion )
    {
    //
    // The ImageRegion and ImageIORegion objects may have different dimensions.
    // Here we only copy the common dimensions between the two. If the ImageRegion
    // has more dimensions than the ImageIORegion, then the defaults of the ImageRegion
    // will take care of the remaining codimension. If the ImageRegion has less dimensions
    // than the ImageIORegion, then the remaining IO dimensions are simply ignored.
    //
    const unsigned int ioDimension = outIORegion.GetImageDimension();
    const unsigned int imageDimension = VDimension;

    unsigned int minDimension = ( ioDimension > imageDimension ) ? imageDimension : ioDimension;

    ImageSizeType  size  = inImageRegion.GetSize();
    ImageIndexType index = inImageRegion.GetIndex();
    
    for( unsigned int i = 0; i < minDimension; i++ )
      {
      outIORegion.SetSize(  i, size[i] );
      outIORegion.SetIndex( i, index[i] );
      }

    //
    // Fill in the remaining codimension (if any) with default values
    //
    for( unsigned int k = minDimension; k < ioDimension; k++ )
      {
      outIORegion.SetSize(  k, 1 ); // Note that default size in IO is 1 not 0
      outIORegion.SetIndex( k, 0 );
      }
    }

  static void Convert( const ImageIORegionType & inIORegion, ImageRegionType & outImageRegion )
    {
    ImageSizeType  size;
    ImageIndexType index;

    size.Fill(1);  // initialize with default values
    index.Fill(0);

    //
    // The ImageRegion and ImageIORegion objects may have different dimensions.
    // Here we only copy the common dimensions between the two. If the ImageRegion
    // has more dimensions than the ImageIORegion, then the defaults of the ImageRegion
    // will take care of the remaining codimension. If the ImageRegion has less dimensions
    // than the ImageIORegion, then the remaining IO dimensions are simply ignored.
    //
    const unsigned int ioDimension = inIORegion.GetImageDimension();
    const unsigned int imageDimension = VDimension;

    unsigned int minDimension = ( ioDimension > imageDimension ) ? imageDimension : ioDimension;

    for(unsigned int i=0; i<minDimension; i++)
      {
      size[i]  = inIORegion.GetSize(i);
      index[i] = inIORegion.GetIndex(i);
      }

    outImageRegion.SetSize( size );
    outImageRegion.SetIndex( index );
    }
};

} // end namespace itk

#endif
