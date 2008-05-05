/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTextOutput.h,v $
  Language:  C++
  Date:      $Date: 2007-10-27 19:51:42 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTextOutput_h
#define __itkTextOutput_h

#include "itkOutputWindow.h"
#include "itkObjectFactory.h"

namespace itk
{

// this class is used to send output to stdout and not the itk window
class ITKCommon_EXPORT TextOutput : public OutputWindow
{
public:
  typedef TextOutput                Self;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods).   */
  itkTypeMacro( TextOutput, OutputWindow );

  /** New macro for creation of through a Smart Pointer   */
  itkNewMacro(TextOutput);

  virtual void DisplayText(const char* s)
    { std::cout << s << std::endl; }

protected:
  TextOutput();
  virtual ~TextOutput();

private:
  TextOutput(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


}


#endif
