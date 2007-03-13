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
#ifndef __otbLeeImageFilter_h
#define __otbLeeImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class LeeImageFilter
 * \brief Anti-speckle image filter
 *
 * This class implements Lee's filter for despeckleing of SAR
 * images. The estimated reflectivity \f$R\f$ is computed as follows:

\f[R = E[I] + b(I-E[I]) \f] with
\f$ b = C^2_r / ( C^2_r + C^2_v )\f$ and \f$C_v =
\frac{1}{\sqrt{L}}\f$,  where
\f$L\f$ the image number of looks and
\f$C_r = \frac{\sqrt{Var(I)}}{E[I]} \f$ and  \f$Var(I) = E[I^2] - E[I]^2\f$.
 *
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT LeeImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** 	Extrait les dimensions aussi bien des images 
  	d'entr�e (Input) que de sortie (Output). */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  /** "typedef" pour simplifier la d�finition et la d�claration de variables. */
  typedef TInputImage InputImageType;
  /** "typedef" pour simplifier la d�finition et la d�claration de variables. */
  typedef TOutputImage OutputImageType;

  /** "typedef" pour les classes standards. */
  typedef LeeImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(LeeImageFilter, ImageToImageFilter);
  
  /** D�finition des images support�es. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  /** "typedef" d�finissant un r�el dans cette classe. */
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" d�finissant la taille d'une image. */
  typedef typename InputImageType::SizeType SizeType;

  /** Positionne le rayon d�finissant le voisinage utilis� pour le calcul du filtre. */
  itkSetMacro(Radius, SizeType);

  /** R�cup�re le rayon d�finissant le voisinage utilis� pour le calcul du filtre. */
  itkGetConstReferenceMacro(Radius, SizeType);
  
  /** Positionne le nombre de vues utilis� pour le calcul du filtre. */
  itkSetMacro(NbLooks, double);
  /** R�cup�re le nombre de vues (r�f�renc� constant) utilis� pour le calcul du filtre. */
  itkGetConstReferenceMacro(NbLooks, double);

  /** LeeImageFilter a besoin d'une zone de traitement plus large en entr�e qu'en sortie 
   * pemettant une utilisation needs a larger input requested region than
   * the output requested region.  As such, LeeImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  LeeImageFilter();
  virtual ~LeeImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LeeImageFilter peut etre impl�ment�e pour un traitement de filtre multithreaded.
   * Ainsi, cette impl�mentation fournit la m�thode ThreadedGenerateData()
   * qui est appel�e pour chaque thread du process. Les donn�es image sont allou�es automatiquement 
   * par la classe "m�re" en appelant la m�thode ThreadedGenerateData(). ThreadedGenerateData peut seulement 
   * �crire la portion de l'image sp�cifi�e par le param�tre "outputRegionForThread"
   *
   *	Filtre de LEE : 
   *	R = E[I] + b(I-E[I]) avec 	b  = C²r / ( C²r + C²v )
   *					Cv = 1 / sqrt(L)  avec L le nombre de vues
   *					Cr = sqrt(Var(I)) / E[I] avec Var(I) = E[I²] - E[I]²
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  LeeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** D�claration du rayon */
  SizeType m_Radius;
  /** D�claration du nombre de vues du filtre */
  double m_NbLooks;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeeImageFilter.txx"
#endif

  
#endif
