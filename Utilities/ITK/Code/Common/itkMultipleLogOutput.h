/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultipleLogOutput.h,v $
  Language:  C++
  Date:      $Date: 2006/02/05 20:57:46 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMultipleLogOutput_h
#define __itkMultipleLogOutput_h

#include "itkLogOutput.h"

#include <fstream>
#include <set>

namespace itk
{

/** \class MultipleLogOutput
 *  \brief Class MultipleLogOutput allows writing simultaneously to multiple
 *  streams. Note that the class derives from std::streambuf and contains a
 *  std::set<> of LogOutput.
 *
 * \author Hee-Su Kim, Compute Science Dept. Kyungpook National University,
 *                     ISIS Center, Georgetown University.
 *
 *  \ingroup OSSystemObjects LoggingObjects
 */

class ITKCommon_EXPORT MultipleLogOutput : public LogOutput
{

public:

  typedef MultipleLogOutput  Self;
  typedef LogOutput   Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef LogOutput               OutputType;

  itkTypeMacro(MultipleLogOutput, LogOutput);

  itkNewMacro(MultipleLogOutput);
  
public:

  /** Register a additional output stream into the list of LogOutputs to write
   * to. The messages will be sent to the streams in the same order that the
   * streams have been added here.  */
  void AddLogOutput( OutputType * output );


  /** Broadcast a flush operation to all the output streams */
  virtual void Flush();

  /** Write to multiple outputs */
  virtual void Write( double timestamp );

  /** Write to multiple outputs */
  virtual void Write(const std::string & content );

  /** Write to a buffer */
  virtual void Write(const std::string & content, double timestamp );


protected:
  /** Constructor */
  MultipleLogOutput();

  /** Destructor */
  virtual ~MultipleLogOutput();

private:
  
  typedef std::set< OutputType::Pointer >   ContainerType;

  ContainerType        m_Output;

};

}

#endif //__itkMultipleLogOutput_h
