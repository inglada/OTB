/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelOverlayImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/02/26 15:46:54 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelOverlayImageFilter_h
#define __itkLabelOverlayImageFilter_h

#include "itkLabelToRGBFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkConceptChecking.h"

namespace itk
{

namespace Functor {  
 
/**  
 \class LabelOverlay
 * \brief A functor class used internally by LabelOverlayImageFilter 
 */
template< class TInputPixel, class TLabel, class TRGBPixel >
class LabelOverlay
{
public:
  LabelOverlay() 
    {
    // provide some default value for external use (outside 
    // LabelOverlayImageFilter) Inside LabelOverlayImageFilter, 
    // the values are always initialized
    m_UseBackground = false;
    m_BackgroundValue = NumericTraits<TLabel>::Zero;
    }

  inline TRGBPixel operator()(  const TInputPixel & p1, const TLabel & p2)
    {
    TRGBPixel rgbPixel;
    if( m_UseBackground && p2 == m_BackgroundValue )
      {
      // value is background
      // return a gray pixel with the same intensity than the input pixel
      typename TRGBPixel::ValueType p = 
                        static_cast< typename TRGBPixel::ValueType >( p1 );
      rgbPixel[0] = p;
      rgbPixel[1] = p;
      rgbPixel[2] = p;
      return rgbPixel;
      }

    // taint the input pixel with the colored one returned by 
    // the color functor.
    TRGBPixel opaque = m_RGBFunctor(p2);
    for( unsigned int i = 0; i<3; i++ )
      {
      rgbPixel[i] = static_cast< typename TRGBPixel::ValueType >( 
                          opaque[i] * m_Opacity + p1 * ( 1.0 - m_Opacity ) );
      }
    return rgbPixel;
    }

  bool operator != (const LabelOverlay &l) const
    {
    bool value = l.m_Opacity != m_Opacity || 
                 m_UseBackground != l.m_UseBackground || 
                 m_BackgroundValue != l.m_BackgroundValue; 

    return value;
    }

  ~LabelOverlay() {}

  void SetOpacity( double opacity ) 
    { 
    m_Opacity = opacity; 
    }

  void SetBackgroundValue( TLabel v ) 
    { 
    m_BackgroundValue = v; 
    }

  void SetUseBackground( bool b ) 
    {
    m_UseBackground = b; 
    }

protected:

private: 
  double          m_Opacity;
  bool            m_UseBackground;
  TLabel          m_BackgroundValue;

  typename Functor::LabelToRGBFunctor<TLabel, TRGBPixel> m_RGBFunctor;
};
}  // end namespace functor


/** \class LabelOverlayImageFilter
 * \brief Apply a colormap to a label image and put it on top of the 
 *  input image
 *
 * Apply a colormap to a label image and put it on top of the input image. 
 * The set of colors is a good selection of distinct colors. The opacity of
 * the label image can be defined by the user. The user can also choose if 
 * the want to use a background and which label value is the background. 
 * A background label produce a gray pixel with the same intensity 
 * than the input one.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, 
 * INRA de Jouy-en-Josas, France.
 *
 * This class was contributed to the Insight Journal 
 *     http://hdl.handle.net/1926/172
 *
 *
 * \sa ScalarToRGBPixelFunctor LabelToRGBImageFilter
 * \ingroup Multithreaded
 *
 */
template <typename  TInputImage, class TLabelImage, typename  TOutputImage>
class ITK_EXPORT LabelOverlayImageFilter :
    public
     BinaryFunctorImageFilter<TInputImage, TLabelImage, TOutputImage, 
       Functor::LabelOverlay< 
          typename TInputImage::PixelType, 
          typename TLabelImage::PixelType, 
          typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef LabelOverlayImageFilter  Self;

  typedef BinaryFunctorImageFilter<TInputImage, TLabelImage, TOutputImage, 
                        Functor::LabelOverlay< 
                            typename TInputImage::PixelType, 
                            typename TLabelImage::PixelType, 
                            typename TOutputImage::PixelType>   >  Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef TOutputImage OutputImageType;
  typedef TLabelImage  LabelImageType;
  typedef TInputImage  InputImageType;

  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TLabelImage::PixelType  LabelPixelType;
  typedef typename TInputImage::PixelType  InputPixelType;

  /** Runtime information support. */
  itkTypeMacro(LabelOverlayImageFilter, BinaryFunctorImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
   /** Set the label image */
  void SetLabelImage( const TLabelImage *input);

  /** Get the label image */
  const LabelImageType * GetLabelImage() const;

  /** Set/Get the opacity of the colored label image. The value must be
   * between 0 and 1
   */
  itkSetMacro( Opacity, double );
  itkGetConstReferenceMacro( Opacity, double );

  /** Set/Get the background value */
  itkSetMacro( BackgroundValue, LabelPixelType );
  itkGetConstReferenceMacro( BackgroundValue, LabelPixelType );

  /** Set/Get if one of the labels must be considered as background */
  itkSetMacro( UseBackground, bool );
  itkGetConstReferenceMacro( UseBackground, bool );
  itkBooleanMacro(UseBackground);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<OutputPixelType>));
  itkConceptMacro(OutputPixelShouldHaveValueType,
    (Concept::HasValueType<OutputPixelType>));
  itkConceptMacro(OutputPixelShouldHaveBracketOperator,
    (Concept::BracketOperator<
        OutputPixelType, 
        unsigned int,
        typename OutputPixelType::ValueType>));
  /** End concept checking */
#endif
 
protected:
  LabelOverlayImageFilter();
  virtual ~LabelOverlayImageFilter() {};

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void);

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const;
  
private:
  LabelOverlayImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double                        m_Opacity;
  bool                          m_UseBackground;
  LabelPixelType                m_BackgroundValue;

};


  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelOverlayImageFilter.txx"
#endif

#endif
