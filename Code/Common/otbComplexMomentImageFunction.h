/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   20 mars 2006
  Version   :   
  Role      :   Complex Geometric Moments Class of iamges 
  $Id:$

=========================================================================*/
#ifndef _otbComplexMomentImageFunction_h
#define _otbComplexMomentImageFunction_h

#include "otbGeometricMomentImageFunction.h"

#include <complex>

namespace otb
{

/**
 * \class ComplexMomentImageFunction
 * \brief Calculate the complex moment value in the full image.
 *
 * Calculate the complex moment value over an image. 
 * The implemented equation is:
 *
 *  \f[  c_{p,q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * With:
 *
 *   - \f$ (x,y) \f$ pixel localization;
 *   - \f$ f(x,y) \f$  the pixel value over the \f$(x,y) \f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 * 
 * \ingroup ImageFunctions
 */
template < class TInput, 
           class TOutput = std::complex<double >,
	   class TCoordRep = float >
class ITK_EXPORT ComplexMomentImageFunction :
    public GeometricMomentImageFunction<TInput, TOutput,TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentImageFunction                                 Self;
  typedef GeometricMomentImageFunction<TInput, TOutput,TCoordRep>    Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentImageFunction, GeometricMomentImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   ComplexType;

  /** Dimension of the underlying image. */
//  itkStaticConstMacro(ImageDimension, unsigned int,
//                      InputType::ImageDimension);
  			 
  /** Evalulate the function at specified index */
  virtual ComplexType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evaluate the function at non-integer positions */
  virtual ComplexType Evaluate( const PointType& point ) const
    { 
      IndexType index;
      this->ConvertPointToNearestIndex( point, index );
      return this->EvaluateAtIndex( index ); 
    }
  virtual ComplexType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
      IndexType index;
      this->ConvertContinuousIndexToNearestIndex( cindex, index );
      return this->EvaluateAtIndex( index ) ; 
    }
  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro( NeighborhoodRadius, int );
  itkGetConstReferenceMacro( NeighborhoodRadius, int );

protected:
  ComplexMomentImageFunction();
  ~ComplexMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  unsigned int m_P;
  unsigned int m_Q;
  int m_NeighborhoodRadius;
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentImageFunction.txx"
#endif

#endif

