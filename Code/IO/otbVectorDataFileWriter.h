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
#ifndef __otbVectorDataFileWriter_h
#define __otbVectorDataFileWriter_h

#include "itkProcessObject.h"
#include "itkExceptionObject.h"
#include "otbVectorDataIOBase.h"

namespace otb
{
/** \brief Base exception class for IO problems during writing.
 *
 * \class ImageFileWriterException
 */
class VectorDataFileWriterException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro( VectorDataFileWriterException, ExceptionObject );

  /** Constructor. */
  VectorDataFileWriterException(const char *file, unsigned int line,
                                const char* message = "Error in IO",
                                const char* loc = "Unknown" ) :
      ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  VectorDataFileWriterException(const std::string &file, unsigned int line,
                                const char* message = "Error in IO",
                                const char* loc = "Unknown" ) :
      ExceptionObject(file, line, message, loc)
  {
  }
};


/** \class VectorDataFileWriter
 *  \brief This class writes vector data file format (shapefile and KML)
 *
 *  \ingroup IO
 */

template <class TInputVectorData>
class ITK_EXPORT VectorDataFileWriter : public itk::ProcessObject
{
public :

  /** Standard class typedefs. */
  typedef VectorDataFileWriter                  Self;
  typedef itk::ProcessObject                    Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataFileWriter, itk::ProcessObject);

  typedef TInputVectorData InputVectorDataType;
  typedef VectorDataIOBase<InputVectorDataType> VectorDataIOBaseType;


  /** Set/Get the image input of this writer.  */
  void SetInput(const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);

  /** Does the real work. */
  virtual void Write();

  virtual void GenerateData();

  virtual void Update()
  {
    this->Write();
  }


  /** Specify the name of the output shapefile to write. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  VectorDataFileWriter();
  virtual ~VectorDataFileWriter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string m_FileName; // The file to be read
  typename VectorDataIOBaseType::Pointer m_VectorDataIO;
  bool  m_UserSpecifiedVectorDataIO; // track whether the VectorDataIO
  bool m_FactorySpecifiedVectorDataIO;

private:
  VectorDataFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataFileWriter.txx"
#endif

#endif // __otbVectorDataFileWriter_h


