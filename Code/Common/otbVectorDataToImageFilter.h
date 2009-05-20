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

#ifndef __otbVectorDataToImageFilter_h
#define __otbVectorDataToImageFilter_h

#include "itkImageSource.h"

#include <mapnik/memory_datasource.hpp>
#include <mapnik/map.hpp>

namespace otb
{
/** \class VectorDataToImageFilter
   *   \brief <b>EXPERIMENTAL FEATURE</b> Render a vector data into an image
   *
   * To use this class, you need to compile the library with the option OTB_USE_MAPNIK
   * to ON. You also need to install mapnik on your machine (http://www.mapnik.org)
   *
   */


template <class TVectorData, class TImage>
  class ITK_EXPORT VectorDataToImageFilter : public itk::ImageSource<TImage>
{
  public:
    /** Standard class typedefs. */
    typedef VectorDataToImageFilter  Self;
    typedef itk::ImageSource<TImage>  Superclass;
    typedef itk::SmartPointer<Self>   Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorDataToImageFilter, ImageSource);

    /** Some typedefs. */
    typedef TVectorData VectorDataType;
    typedef TImage ImageType;
    typedef typename ImageType::Pointer ImagePointer;
    typedef typename VectorDataType::ConstPointer VectorDataConstPointer;
    typedef typename VectorDataType::DataTreeType::TreeNodeType    InternalTreeNodeType;
    typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;

    /** Number of dimensions. */
    itkStaticConstMacro(ImageDimension, unsigned int,
                        TImage::ImageDimension);

    /** Image size typedef. */
    typedef itk::Size<itkGetStaticConstMacro(ImageDimension)> SizeType;

    /** Image index typedef. */
    typedef typename TImage::IndexType IndexType;

    /** Image spacing,origin and direction typedef */
    typedef typename TImage::SpacingType   SpacingType;
    typedef typename TImage::PointType     OriginType;
    typedef typename TImage::DirectionType DirectionType;

    /** Region typedef */
    typedef typename TImage::RegionType RegionType;

    /** typedef specific to mapnik */
    typedef boost::shared_ptr<mapnik::memory_datasource> datasource_ptr;


    virtual void SetInput( const VectorDataType *input);
    const VectorDataType * GetInput(void);

    /** Set the size of the output image. */
    itkSetMacro( Size, SizeType );

    /** Get the size of the output image. */
    itkGetConstReferenceMacro( Size, SizeType );

    /** Set the origin of the vector data.
     * \sa GetOrigin() */
    itkSetMacro(Origin, OriginType);
    virtual void SetOrigin( const double origin[2] );
    virtual void SetOrigin( const float origin[2] );

    itkGetConstReferenceMacro(Origin, OriginType);


    /** Set the spacing (size of a pixel) of the vector data.
    * \sa GetSpacing() */
    virtual void SetSpacing (const SpacingType & spacing);
    virtual void SetSpacing (const double spacing[2]);
    virtual void SetSpacing (const float spacing[2]);

    itkGetConstReferenceMacro(Spacing, SpacingType);

    /** Get/Set methods for the scale factor */
    itkSetMacro(ScaleFactor, double);
    itkGetMacro(ScaleFactor, double);

    /** */
    void AddStyle(std::string style)
    {
      m_StyleList.push_back(style);
    }

    /** Clear the style list */
    void ClearStyleList()
    {
      m_StyleList.clear();
    }

  protected:
    /** Constructor */
    VectorDataToImageFilter();
    /** Destructor */
    virtual ~VectorDataToImageFilter() {};
    /**PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    virtual void GenerateOutputInformation();

    virtual void GenerateData(void);

    virtual void BeforeThreadedGenerateData();


  private:
    VectorDataToImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    void ProcessNode(InternalTreeNodeType * source, datasource_ptr mDatasource);

    SpacingType         m_Spacing;
    OriginType          m_Origin;
    SizeType            m_Size;
    IndexType           m_StartIndex;
    DirectionType       m_Direction;

    mapnik::Map m_Map;

    //This factor is used to flip the data on the Y axis when using a
    //sensor model geometry (where the Y coordinate increases top-down)
    int m_SensorModelFlip;

    //this parameter is used only in the case of sensor geometry
    //to adjust the scale
    double m_ScaleFactor;

    //style list
    std::vector<std::string> m_StyleList;

    //Projection in the proj.4 format (for mapnik)
    std::string m_VectorDataProjectionProj4;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToImageFilter.txx"
#endif

#endif