/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSimpleFilterWatcher.h,v $
  Language:  C++
  Date:      $Date: 2006/02/06 22:01:58 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSimpleFilterWatcher_h
#define __itkSimpleFilterWatcher_h

#include "itkCommand.h"
#include "itkProcessObject.h"
#include "itkTimeProbe.h"


namespace itk
{

/** \class SimpleFilterWatcher
 * \brief Simple mechanism for monitoring the pipeline events of a filter and reporting these events to std::cout
 *
 * SimpleFilterWatcher provides a simple mechanism for monitoring the
 * execution of filter.  SimpleFilterWatcher is a stack-based object
 * which takes a pointer to a ProcessObject at constructor
 * time. SimpleFilterWatcher creates a series of commands that are
 * registered as observers to the specified ProcessObject. The events
 * monitored are:
 *
 *      StartEvent
 *      EndEvent
 *      ProgressEvent
 *      IterationEvent
 *      AbortEvent
 *
 * The callbacks routines registered for these events emit a simple
 * message to std::cout.
 *
 * Example of use:
 *
 * typedef itk::BinaryThresholdImageFilter<ImageType> FilterType;
 * FilterType::Pointer thresholdFilter = FilterType::New();
 *
 * SimpleFilterWatcher watcher(thresholdFilter, "Threshold");
 *
 * The second argument to the constructor to SimpleFilterWatcher is an
 * optional string that is prepended to the event messages. This
 * allows the user to associate the emitted messages to a particular
 * filter/variable.
 *
 *
 * \todo Allow any stream object to be used for the output (not just std::cout)
 * 
 */
class ITKCommon_EXPORT SimpleFilterWatcher
{
public:
  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  SimpleFilterWatcher(itk::ProcessObject* o, const char *comment="");

  /** Copy constructor */
  SimpleFilterWatcher(const SimpleFilterWatcher& );

  /** Default constructor. Only provided so that you can have
   * std::vectors of SimpleFilterWatchers. */
  SimpleFilterWatcher();

  /** operator=  */
  void operator=(const SimpleFilterWatcher& );

  /** Destructor. */
  virtual ~SimpleFilterWatcher();

  /** Method to get the name of the class be monitored by this
   *  SimpleFilterWatcher */
  const char *GetNameOfClass ()
    {
      return (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None");
    }

  /** Methods to control the verbosity of the messages. Quiet
   * reporting limits the information emitted at a ProgressEvent. */
  void QuietOn() {m_Quiet = true;};
  void QuietOff() {m_Quiet = false;};

  /** Methods to use to test the AbortEvent of the a filter. If
   * TestAbort is on, the filter being watched will be aborted when
   * the progress reaches 30%.*/
  void TestAbortOn() {m_TestAbort = true;};
  void TestAbortOff() {m_TestAbort = false;};

  
protected:

  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress()
  {
    if (m_Process)
      {
      m_Steps++;
      if (!m_Quiet)
        {
        std::cout << " | " << m_Process->GetProgress() << std::flush;
        if ((m_Steps % 10) == 0)
          {
          std::cout << std::endl;
          }
        }
      if (m_TestAbort)
        {
        if (m_Process->GetProgress() > .03)
          {
          m_Process->AbortGenerateDataOn();
          }
        }
      }
  }

  /** Callback method to show the AbortEvent */
  virtual void ShowAbort()
  {
    std::cout << std::endl << "      ABORT" << std::endl << std::flush;
  }

  /** Callback method to show the IterationEvent */
  virtual void ShowIteration()
  {
    std::cout << " # " << std::flush;
    m_Iterations++;
  }

  /** Callback method to show the StartEvent */
  virtual void StartFilter()
  {
    m_Steps = 0;
    m_Iterations = 0;
    m_TimeProbe.Start();
    std::cout << "-------- Start "
              << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")
              << " \"" << m_Comment << "\" ";
    if (!m_Quiet)
      {
      if (m_Process)
        {
        std::cout << m_Process;
        }
      else
        {
        std::cout << "Null";
        }
      }
    std::cout << (m_Quiet ? "Progress Quiet " : "Progress ")
              << std::flush;
  }

  /** Callback method to show the EndEvent */
  virtual void EndFilter()
  {
    m_TimeProbe.Stop();
    std::cout << std::endl << "Filter took "
              << m_TimeProbe.GetMeanTime()
              << " seconds.";
    std::cout << std::endl
              << "-------- End "
              << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")
              << " \"" << m_Comment << "\" " << std::endl;
    if (!m_Quiet)
      {
      if (m_Process)
        {
        std::cout << m_Process;
        }
      else
        {
        std::cout << "None";
        }
      std::cout << std::flush;
      }
    if (m_Steps < 1)
      {
      itkExceptionMacro ("Filter does not have progress.");
      }
    }

private:
  TimeProbe m_TimeProbe;
  int m_Steps;
  int m_Iterations;
  bool m_Quiet;
  bool m_TestAbort;
  std::string m_Comment;
  itk::ProcessObject::Pointer m_Process;

  typedef SimpleMemberCommand<SimpleFilterWatcher> CommandType;
  CommandType::Pointer m_StartFilterCommand;
  CommandType::Pointer m_EndFilterCommand;
  CommandType::Pointer m_ProgressFilterCommand;
  CommandType::Pointer m_IterationFilterCommand;
  CommandType::Pointer m_AbortFilterCommand;

  unsigned long m_StartTag;
  unsigned long m_EndTag;
  unsigned long m_ProgressTag;
  unsigned long m_IterationTag;
  unsigned long m_AbortTag;
};

} // end namespace itk

#endif
