/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   23 janvier 2006
  Role      :  Filtre de débruitage de LEE sur une image 
  $Id$ 

=========================================================================*/
#ifndef __otbLeeImageFilter_h
#define __otbLeeImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class LeeImageFilter
 * \brief Applique un filtre de débruitage sur une image.
 *
 * Ce filtre correspond au filtre Anti-Speckle de LEE :
 *
 *	R = E[I] + b(I-E[I]) avec 	b  = C²r / ( C²r + C²v )
 *					Cv = 1 / sqrt(L)  avec L le nombre de vues
 *					Cr = sqrt(Var(I)) / E[I] avec Var(I) = E[I²] - E[I]²
 *
 */

template <class TInputImage, class TOutputImage>
class LeeImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** 	Extrait les dimensions aussi bien des images 
  	d'entrée (Input) que de sortie (Output). */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  typedef TInputImage InputImageType;
  /** "typedef" pour simplifier la définition et la déclaration de variables. */
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
  
  /** Définition des images supportées. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  /** "typedef" définissant un réel dans cette classe. */
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" définissant la taille d'une image. */
  typedef typename InputImageType::SizeType SizeType;

  /** Positionne le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkSetMacro(Radius, SizeType);

  /** Récupère le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(Radius, SizeType);
  
  /** Positionne le nombre de vues utilisé pour le calcul du filtre. */
  itkSetMacro(NbVues, double);
  /** Récupère le nombre de vues (référencé constant) utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(NbVues, double);

  /** LeeImageFilter a besoin d'une zone de traitement plus large en entrée qu'en sortie 
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

  /** LeeImageFilter peut etre implémentée pour un traitement de filtre multithreaded.
   * Ainsi, cette implémentation fournit la méthode ThreadedGenerateData()
   * qui est appelée pour chaque thread du process. Les données image sont allouées automatiquement 
   * par la classe "mère" en appelant la méthode ThreadedGenerateData(). ThreadedGenerateData peut seulement 
   * écrire la portion de l'image spécifiée par le paramètre "outputRegionForThread"
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

  /** Déclaration du rayon */
  SizeType m_Radius;
  /** Déclaration du nombre de vues du filtre */
  double m_NbVues;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeeImageFilter.txx"
#endif

  
#endif
