/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2HeaderBase.h,v $
  Language:  C++
  Date:      $Date: 2007/03/29 18:39:26 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBrains2HeaderBase_h
#define __itkBrains2HeaderBase_h

#include "itkMacro.h"

#include <string>
#include <list>
#include <fstream>

namespace itk {
/**
 * \class Brains2HeaderBase
 * An abstract class to define b2 header information.  This just adds
 * file reading and writing routines to the standard STL std::list<
 * std::pair<std::string, std::string> >
 */
class Brains2HeaderBase: public std::list< std::pair<std::string, std::string> >
{
public:
  /** Standard class typedefs. */
  typedef Brains2HeaderBase                                Self;
  typedef std::list< std::pair<std::string, std::string> > Superclass;
  typedef Self *                                           Pointer;
  typedef const Self *                                     ConstPointer;
  /**
   * Default constructor
   * \author Hans J. Johnson
   */
  Brains2HeaderBase();
  /**
   * Default destructor
   * \author Hans J. Johnson
   */
  virtual ~Brains2HeaderBase();
  /**
   * A routine for reading Brains2 header information from a file
   * \author Hans J. Johnson
   * \param filename The name of the file to read header information from
   */
  virtual void ReadBrains2Header(std::string filename);
  /**
   * A routine for writing Brains2 header information from a file
   * \author Hans J. Johnson
   * \param filename The name of the file to write header information to
   */
  virtual void WriteBrains2Header(std::string filename) const;
  /**
   * A routine for reading Brains2 header information from a file
   * \author Hans J. Johnson
   * \param inputstream The file stream to read from.
   * \return the current location in the input stream
   * \pre The file must already have been opened and be prepared to reading
   * \post The file stream is left open.
   */
  virtual std::ifstream & ReadBrains2Header(std::ifstream & inputstream);
  /**
   * A routine for writing Brains2 header information from a file
   * \author Hans J. Johnson
   * \param outputstream The file stream to write to.
   * \return the current location in the input stream
   * \pre The file must already have been opened and be prepared to reading
   * \post The file stream is left open.
   */
  virtual std::ofstream & WriteBrains2Header(std::ofstream & outputstream) const;

  /**
   * A routine for printing Brains2 header information to a standard stream
   * \author Hans J. Johnson
   * \param os The stream to print to
   */
  virtual void PrintSelf(std::ostream &os) const;
  /**
   * A funtion to determine if a tag exists
   * \author Hans J. Johnson
   * \param KeyID
   * \return true if a key is found, and false otherwise
   */
  bool DoesKeyExist(const std::string &KeyID) const;
  /**
   * A function to return the value associated with a key as a floating point number.
   * \author Hans J. Johnson
   * \param KeyID A string that identifies the desired key.
   * \return The requested floating point number, or 0.0F if KeyID not found.
   */
  float getFloat(const std::string &KeyID) const;
  /**
   * A function to return the value associated with a key as an integer number.
   * \author Hans J. Johnson
   * \param KeyID A string that identifies the desired key.
   * \return The requested integer number, or 0. if KeyID not found.
   */
  int getInt(const std::string &KeyID) const;
  /**
   * A function to return the value associated with a key as a string.
   * \author Hans J. Johnson
   * \param KeyID A string that identifies the desired key.
   * \return The requested string, or "" if KeyID not found.
   */
  std::string getString(const std::string &KeyID) const;
  /**
   * A function to clear the header of all values
   * \author Hans J. Johnson
   * \post After this function is completed, the lists will all be empty.
   */
  void ClearHeader(void);
protected:
  virtual std::string GetHeaderBeginTag(void) const= 0;
  virtual std::string GetHeaderEndTag(void) const = 0;
  std::list<Brains2HeaderBase *> m_Child;
private:
};
} //End of namespace itk
#endif // __itkBrains2HeaderBase_h
