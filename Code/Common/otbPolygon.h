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
#ifndef __otbPolygon_h
#define __otbPolygon_h

#include "otbPolyLineParametricPathWithValue.h"
#include "itkImageRegion.h"

namespace otb
{
/** \class Polygon
 *  \brief This class represent a 2D polygon.
 *
 *  It derives from otb::PolyLineParametricPathWithValue. The polygon is 
 * considered to be the closed path represented by the 
 * PolyLineParametricPathWithValue.
 * 
 *  It implements some useful methods to work with polygons, such as surface 
 * computation, as well as method useful in the spatial reasoning context, such 
 * as IsInside, IsOnEdge, IsTouching, IsCrossing. 
 *
 * \sa otb::PolyLineParametricPathWithValue
 */
template<class TValue=double>
class ITK_EXPORT Polygon
  : public PolyLineParametricPathWithValue<TValue,2>
{
  public:
  /** Standard typedefs */
  typedef Polygon                                   Self;
  typedef PolyLineParametricPathWithValue<TValue,2> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;
  typedef TValue                                    ValueType;
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(Polygon, PolyLineParametricPathWithValue);
  
  /** Derived typedefs */
  typedef typename Superclass::VertexType          VertexType;
  typedef typename Superclass::VertexListType      VertexListType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::ConstIterator   VertexListIteratorType;

  typedef itk::ImageRegion<2>                       RegionType;
  typedef typename RegionType::SizeType            SizeType;
  typedef typename RegionType::IndexType           IndexType;

 itkSetMacro(Epsilon,double);
 itkGetMacro(Epsilon,double);

  /**
   * Check wether point is strictly inside the polygon.
   * \param point The point to check.
   * \return True if the point is inside the polygon.
   */
  bool IsInside(VertexType point);
  
  /**
   * Check wether point is strictly on the edge of the polygon.
   * \param point The point to check.
   * \return True if the point is on the edge of the polygon.
   */
  bool IsOnEdge(VertexType point);
  
  /**
   * Returns the number of crossings of the polygon with a given segment.
   * \param a First point of the segment,
   * \param b Second point of the segment,
   * \return the number of strict crossings of segment [ab] with the polygon.
   */
  unsigned int NbCrossing(VertexType a, VertexType b);

  /**
   * Returns the number of touchings without crossing of the polygon with a given segment.
   * \param a First point of the segment,
   * \param b Second point of the segment,
   * \return the number of touchings without crossing of segment [ab] with the polygon.
   */
  unsigned int NbTouching(VertexType a, VertexType b);

  /**
   * Check wether two segments [a1a2] and [b1b2] are strictly crossing.
   * \param a1 First point of the first segment,
   * \param a1 Second point of the first segment,
   * \param a1 First point of the second segment,
   * \param a1 Second point of the second segment.
   * \return True if the two segments are strictly crossing.
   */
  bool IsCrossing(VertexType a1, VertexType a2, VertexType b1, VertexType b2);
  
  /**
   * Check wether two segments[a1a2] and [b1b2] are touching without crossing.
   * \param a1 First point of the first segment,
   * \param a1 Second point of the first segment,
   * \param a1 First point of the second segment,
   * \param a1 Second point of the second segment.
   * \return True if the two segments are touching without crossing.
   */
   bool IsTouching(VertexType a1, VertexType a2, VertexType b1, VertexType b2);

 /**
  * Compute the polygon bounding region.
  * \return The region.
  */
 RegionType GetBoundingRegion(void);

  /**
  * Return the polygon surface.
  * \return The surface.
  */
 double GetSurface();

protected:
  /** Constructor */
  Polygon() 
    {
      m_Epsilon = 0.000001;
    };
    
  /** Destructor */
  virtual ~Polygon() {};
  
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  
private:
  Polygon(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_Epsilon;
  double m_Surface;
};
}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolygon.txx"
#endif
#endif
