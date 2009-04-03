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
#ifndef __otbStandardRenderingFunction_h
#define __otbStandardRenderingFunction_h

#include "otbRenderingFunction.h"
#include "otbMacro.h"
#include <assert.h>

namespace otb
{
namespace Function
{
/** \class Identiy
* \brief Default math functor parameter for rendering function.
*/
template <class TInputPixel, class TOutputPixel>
class Identity
{
public:
  Identity(){};
  ~Identity(){};
  bool operator !=(const Identity &) const
  {
    return false;
  }
  bool operator ==(const Identity & other) const
  {
    return !(*this != other);
  }

  inline TOutputPixel operator()(const TInputPixel & A) const
  {
    return static_cast<TOutputPixel>(A);
  }
};

/**\class StandardRenderingFunction
 * \brief Standard rendering.
 * If the input image is an Image, the function
 * renders it with R, G and B channels all equals. 
 * If it is a VectorImage, the function renders
 * the selected channels.
 */
template <class TPixelPrecision, class TRGBPixel, class TTransferFunction = Identity<TPixelPrecision,TPixelPrecision> >
class StandardRenderingFunction
  : public RenderingFunction<TPixelPrecision,TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef StandardRenderingFunction                   Self;
  typedef RenderingFunction<TPixelPrecision,TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(StandardRenderingFunction,RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                  OutputPixelType;
  typedef typename OutputPixelType::ValueType        OutputValueType;
  typedef TPixelPrecision                            ScalarPixelType;
  typedef itk::VariableLengthVector<ScalarPixelType> VectorPixelType;
  /** Extrema vector */
  typedef std::vector<ScalarPixelType>               ExtremaVectorType;
  typedef TTransferFunction                          TransferFunctionType;

  /** Evaluate method (scalar version) */
  inline virtual const OutputPixelType Evaluate(ScalarPixelType spixel) const
  {    
    OutputPixelType resp;
    resp.Fill(this->Evaluate(m_TransferFunction(spixel),m_TransferedMinimum[0],m_TransferedMaximum[0]));
    return resp;
  }
  /** Evaluate method (vector version) */
  inline virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel) const
  {
    OutputPixelType resp;
    resp.SetRed(Evaluate(m_TransferFunction(vpixel[m_RedChannelIndex]),m_TransferedMinimum[m_RedChannelIndex],m_TransferedMaximum[m_RedChannelIndex]));
    resp.SetBlue(Evaluate(m_TransferFunction(vpixel[m_BlueChannelIndex]),m_TransferedMinimum[m_BlueChannelIndex],m_TransferedMaximum[m_BlueChannelIndex]));
    resp.SetGreen(Evaluate(m_TransferFunction(vpixel[m_GreenChannelIndex]),m_TransferedMinimum[m_GreenChannelIndex],m_TransferedMaximum[m_GreenChannelIndex]));
    return resp;
  }


  inline const std::string Describe(ScalarPixelType spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"Grayscale[value: "<< static_cast<typename itk::NumericTraits<ScalarPixelType>::PrintType>(spixel)<<", displayed: "<< static_cast<unsigned int>(output[0])<<"]";
    return oss.str();
  }

  inline const std::string Describe(const VectorPixelType & vpixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(vpixel);

    for(unsigned int channel = 0; channel < vpixel.Size();++channel)
      {
      if(channel == m_RedChannelIndex)
	{
	oss<<"c= "<<channel<<", R= "<<(int)output[0];
	if(output[0]<100)
	  {
	  oss<<" ";
	  }
	if(output[0]<10)
	  {
	  oss<<" ";
	  }
	oss<<", v= "<<static_cast<typename itk::NumericTraits<ScalarPixelType>::PrintType>(vpixel[channel])<<std::endl;
	}
      else if(channel == m_BlueChannelIndex)
	{
	oss<<"c= "<<channel<<", G= "<<(int)output[1]; 
	if(output[1]<100)
	  {
	  oss<<" ";
	  }
	if(output[1]<10)
	  {
	  oss<<" ";
	  }
	oss<<", v= "<<static_cast<typename itk::NumericTraits<ScalarPixelType>::PrintType>(vpixel[channel])<<std::endl;
	}
      else if(channel == m_GreenChannelIndex)
	{
	oss<<"c= "<<channel<<", B= "<<(int)output[2]; 
	if(output[2]<100)
	  {
	  oss<<" ";
	  }
	if(output[2]<10)
	  {
	  oss<<" ";
	  }
	oss<<", v= "<<static_cast<typename itk::NumericTraits<ScalarPixelType>::PrintType>(vpixel[channel])<<std::endl;
	}
      else
	{
	oss<<"c= "<<channel<<",         v= "<<static_cast<typename itk::NumericTraits<ScalarPixelType>::PrintType>(vpixel[channel])<<std::endl; 
	}
      }
    return oss.str();
  }

  /** Get the transfer function for tuning */
  TransferFunctionType & GetTransferFunction()
  {
    return m_TransferFunction;
  }

  /** Set the red channel index (vector mode only) */
  void SetRedChannelIndex(unsigned int index)
  {
    m_RedChannelIndex = index;
  }
  
  /** Get the red channel index (vector mode only) */
  unsigned int GetRedChannelIndex(void)
  {
    return m_RedChannelIndex;
  }

  /** Set the blue channel index (vector mode only) */
  void SetBlueChannelIndex(unsigned int index)
  {
    m_BlueChannelIndex = index;
  }
  
  /** Get the blue channel index (vector mode only) */
  unsigned int GetBlueChannelIndex(void)
  {
    return m_BlueChannelIndex;
  }

  /** Set the green channel index (vector mode only) */
  void SetGreenChannelIndex(unsigned int index)
  {
    m_GreenChannelIndex = index;
  }
  
  /** Get the green channel index (vector mode only) */
  unsigned int GetGreenChannelIndex(void)
  {
    return m_GreenChannelIndex;
  }
  
  /** Set all channels (grayscale mode) */
  void SetAllChannels(unsigned int index)
  {
    m_RedChannelIndex   = index;
    m_BlueChannelIndex  = index;
    m_GreenChannelIndex = index; 
  }

  /** Togle the UserDefinedTransferedMinMax mode */
  void SetUserDefinedTransferedMinMax(bool val)
  {
    m_UserDefinedTransferedMinMax = val;
  }

  /** Togle the UserDefinedTransferedMinMax mode */
  bool GetUserDefinedTransferedMinMax(void)
  {
    return m_UserDefinedTransferedMinMax;
  }

  /** Set the transfered minimum (scalar version) */
  virtual void SetTransferedMinimum(ScalarPixelType spixel)
  {
    m_TransferedMinimum.clear();
    m_TransferedMinimum.push_back(spixel);
  }
  
  /** Set the transfered maximum (scalar version) */
  virtual void SetTransferedMaximum(ScalarPixelType spixel)
  {
    m_TransferedMaximum.clear();
    m_TransferedMaximum.push_back(spixel);
  }

  /** Set transfered minimum (vector version) */
  virtual void SetTransferedMinimum(const VectorPixelType & vpixel)
  {
    m_TransferedMinimum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMinimum.push_back(vpixel[i]);
      }
  }

  /** Set transfered maximum (vector version) */
  virtual void SetTransferedMaximum(const VectorPixelType & vpixel)
  {
    m_TransferedMaximum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_TransferedMaximum.push_back(vpixel[i]);
      }
  }

  /** Update transfered min and max */
  virtual void Initialize()
  {
    if(!m_UserDefinedTransferedMinMax)
      {
      typename ExtremaVectorType::const_iterator minIt = this->m_Minimum.begin();
      typename ExtremaVectorType::const_iterator maxIt = this->m_Maximum.begin();
      
      m_TransferedMinimum.clear();
      m_TransferedMaximum.clear();
      
      while(minIt != this->m_Minimum.end() && maxIt != this->m_Maximum.end())
        {
        const double v1 = m_TransferFunction(*minIt);
        const double v2 = m_TransferFunction(*maxIt);
        m_TransferedMinimum.push_back(std::min(v1,v2));
        m_TransferedMaximum.push_back(std::max(v1,v2));
        ++minIt;
        ++maxIt;
        }
      }
  }
     
protected:
  /** Constructor */
  StandardRenderingFunction() : m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2), m_TransferFunction(),
                                m_UserDefinedTransferedMinMax(false),         m_TransferedMinimum(), m_TransferedMaximum()
  {}
  /** Destructor */
  ~StandardRenderingFunction() {}
  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  const OutputValueType Evaluate(ScalarPixelType input, ScalarPixelType min, ScalarPixelType max) const
  {
    if(input > max)
      {
      return 255;
      }
    else if(input < min)
      {
      return 0;
      }
    else
      {
      return static_cast<OutputValueType>(vcl_floor(255.*(static_cast<double>(input)-static_cast<double>(min))
                                                    /(static_cast<double>(max)-static_cast<double>(min))+0.5));
      }
  }

private:
  StandardRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Index of the channels to display (vector mode only, has no effet
   *  on scalar mode)
   */
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

  /** Transfer function 
   *  \note This member is declared mutable because some functors that
   * can be used as a transfer function but are not const correct.
   *  Since a const reference is passed to the functor anyway, it is
   * not harmful to do so and preserves const correctness of the
   *  Evaluate() mehtods.
   */
  mutable TransferFunctionType m_TransferFunction;

  /** If true, values mapped by the transfert function are clamped to
      user defined min/max */
  bool m_UserDefinedTransferedMinMax;

  /** Transfered min and max */
  ExtremaVectorType m_TransferedMinimum;
  ExtremaVectorType m_TransferedMaximum;
};
} // end namespace Functor
} // end namespace otb

#endif


