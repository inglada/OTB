/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2MaskImageIO.h,v $
  Language:  C++
  Date:      $Date: 2008-05-08 15:28:19 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBrains2MaskImageIO_h
#define __itkBrains2MaskImageIO_h

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <fstream>
#include "itkImageIOBase.h"
#include "itkBrains2IPLHeaderInfo.h"
#include "itkOctree.h"

namespace itk
{
/**
 * \file   Much of the code for this file reader/writer was taken from 
 *         the University of Iowa Psychiatry departments
 *         Imaging Group with permission of the author Vincent Magnotta
 * \author Hans J. Johnson
 *         The University of Iowa 2002
 * \brief This file was written as a modification to the itkMetaImageIO
 *        as a new method for reading in files from the Brains2Mask specification.
 *        Brains2Masks are specified as a single file.  The first part of the file
 *        begins with an "IPL_HEADER_BEGIN" followed by ascii text describing the
 *        dimensions, and other important information about the data.  The header
 *        information concludes with  the "IPL_HEADER_END\n" identifier, and is immediatly followed by
 *        the binary portion of the file.  The binary portion contains a binary octreeHdr
 *        consisting of 6 unsigned integers, followed by sequentially defined unsigned short integers
 *        representing (WHITE=1, BLACK=0, GRAY=2) colors.  The GRAY color indicates that this is a branching node.
 */

/**
 * \todo
 *  \par Everything still needs to be done.
 */

/** \class Brains2MaskImageIO
 * \author Hans J. Johnson
 * \brief Class that defines how to read Brains2Mask file format. 
 * */
class ITK_EXPORT Brains2MaskImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef Brains2MaskImageIO Self;
  typedef ImageIOBase        Superclass;
  typedef SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Brains2MaskImageIO, Superclass);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine if the file can be read with this ImageIO implementation.
   * \author Hans J Johnson
   * \param FileNameToRead The name of the file to test for reading.
   * \post Sets classes ImageIOBase::m_FileName variable to be FileNameToWrite
   * \return Returns true if this ImageIO can read the file specified. 
   */
  virtual bool CanReadFile(const char* FileNameToRead);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine if the file can be written with this ImageIO implementation.
   * \param FileNameToWrite The name of the file to test for writing.
   * \author Hans J. Johnson
   * \post Sets classes ImageIOBase::m_FileName variable to be FileNameToWrite
   * \return Returns true if this ImageIO can write the file specified. 
   */
  virtual bool CanWriteFile(const char * FileNameToWrite);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);


protected:
  Brains2MaskImageIO();
  ~Brains2MaskImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;
private:
  Brains2MaskImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  itk::OctreeNodeBranch *  readOctree (
    std::ifstream & octreestream,
    const ImageIOBase::ByteOrder machineByteOrder,
    const ImageIOBase::ByteOrder fileByteOrder);
  ImageIOBase::ByteOrder m_MachineByteOrder;

  itk::Brains2IPLHeaderInfo m_IPLHeaderInfo;
  OctreeBase               *m_Octree;
  itk::Brains2IPLHeaderInfo m_B2MaskHeader;
};

} // end namespace itk

#endif // __itkBrains2MaskImageIO_h
