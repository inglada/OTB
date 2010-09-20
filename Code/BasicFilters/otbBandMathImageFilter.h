/*=========================================================================

  Program:   ORFEO Toolbox
    Language:  C++
    Date:      $Date$
    Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from ITK. See ITKCopyright.txt
    for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
        PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbBandMathImageFilter_h
#define __otbBandMathImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkArray.h"

#include "otbParser.h"

namespace otb
{
/** \class BandMathImageFilter
 * \brief Performs a mathematical operation on the input images
 * according to the formula specified by the user.
 * 
 * This filter is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 * 
 * OTB additional functions:
 * ndvi(r, niri)
 * 
 * OTB additional contants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * In order to use this filter, at least one input image is to be
 * set. An associated variable name can be specified or not by using
 * the corresponding SetNthInput method. For the nth input image, if
 * no associated variable name has been spefified, a default variable
 * name is given by concatenating the letter "b" (for band) and the
 * coresponding input index.
 * Next step is to set the expression according to the variable
 * names. For example, in the default case with three input images the
 * following expression is valid :
 * "ndvi(b1,b2)*b3"
 * 
 * \sa Parser
 * 
 * \ingroup Streamed
 * \ingroup Threaded
 */

template< class TImage >
class ITK_EXPORT BandMathImageFilter 
  : public itk::InPlaceImageFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef BandMathImageFilter< TImage >                 Self;
  typedef itk::InPlaceImageFilter< TImage >             Superclass;
  typedef itk::SmartPointer< Self >                     Pointer;
  typedef itk::SmartPointer< const Self >               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(BandMathImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                  ImageType;
  typedef typename ImageType::ConstPointer        ImagePointer;
  typedef typename ImageType::RegionType          ImageRegionType; 
  typedef typename ImageType::PixelType           PixelType;
  typedef Parser                                  ParserType;
  
  /** Set the nth filter input with or without a specified associated variable name */
  void SetNthInput( unsigned int idx, const ImageType * image);
  void SetNthInput( unsigned int idx, const ImageType * image, const std::string& varName);

  /** Change the nth filter input associated variable name */
  void SetNthInputName(unsigned int idx, const std::string& expression);

  /** Set the expression to be parsed */
  void SetExpression(const std::string& expression);

  /** Return the expression to be parsed */
  std::string GetExpression() const;
  
  /** Return the nth filter input associated variable name */
  std::string GetNthInputName(unsigned int idx) const;

  /** Return a pointer on the nth filter input */
  ImageType * GetNthInput(unsigned int idx);

protected :
  BandMathImageFilter();
  virtual ~BandMathImageFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
 
  void BeforeThreadedGenerateData();
  void ThreadedGenerateData(const ImageRegionType& outputRegionForThread, int threadId );
  void AfterThreadedGenerateData();

private :
  BandMathImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string                           m_Expression; 
  std::vector<ParserType::Pointer>      m_VParser;
  std::vector< std::vector<double> >    m_AImage;
  std::vector< std::string >            m_VVarName;

  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBandMathImageFilter.txx"
#endif

#endif
