/*
Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkMultilayerNeuralNetworkBase.h,v $
Language:  C++
Date:      $Date: 2007-08-30 18:23:05 $
Version:   $Revision: 1.9 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MultiLayerNeuralNetworkBase_h
#define __MultiLayerNeuralNetworkBase_h

#include "itkNeuralNetworkObject.h"
#include "itkLayerBase.h"

namespace itk
{
  namespace Statistics
    {

    template<class TMeasurementVector, class TTargetVector,class TLearningLayer=LayerBase<TMeasurementVector, TTargetVector> >
      class MultilayerNeuralNetworkBase : public NeuralNetworkObject<TMeasurementVector, TTargetVector>
        {
      public:

        typedef MultilayerNeuralNetworkBase Self;
        typedef NeuralNetworkObject<TMeasurementVector, TTargetVector> Superclass;
        typedef SmartPointer<Self> Pointer;
        typedef SmartPointer<const Self> ConstPointer;
        itkTypeMacro(MultilayerNeuralNetworkBase, NeuralNetworkObject);

        /** New macro for creation of through a Smart Pointer. */
        itkNewMacro( Self );

        typedef typename Superclass::ValueType ValueType;
        typedef typename Superclass::MeasurementVectorType MeasurementVectorType;
        typedef typename Superclass::TargetVectorType TargetVectorType;
        typedef typename Superclass::NetworkOutputType NetworkOutputType;

        typedef typename Superclass::LayerInterfaceType LayerInterfaceType;
        typedef TLearningLayer LearningLayerType;
        typedef LearningFunctionBase<typename TLearningLayer::LayerInterfaceType, TTargetVector> LearningFunctionInterfaceType;

        typedef std::vector<typename LayerInterfaceType::WeightSetInterfaceType::Pointer> WeightVectorType;
        typedef std::vector<typename LayerInterfaceType::Pointer> LayerVectorType;

        typedef TransferFunctionBase<ValueType> TransferFunctionInterfaceType;
        typedef InputFunctionBase<ValueType*, ValueType> InputFunctionInterfaceType;

//#define __USE_OLD_INTERFACE  Comment out to ensure that new interface works
#ifdef __USE_OLD_INTERFACE
        itkSetMacro(NumOfLayers, int);
        itkGetConstReferenceMacro(NumOfLayers, int);

        itkSetMacro(NumOfWeightSets, int);
        itkGetConstReferenceMacro(NumOfWeightSets, int);
#else
        int GetNumOfLayers(void) const
          {
          return m_Layers.size();
          }
        int GetNumOfWeightSets(void) const
          {
          return m_Weights.size();
          }

#endif

        void AddLayer(LayerInterfaceType *);
        LayerInterfaceType * GetLayer(int layer_id);
        const LayerInterfaceType * GetLayer(int layer_id) const;

        void AddWeightSet(typename LayerInterfaceType::WeightSetInterfaceType*);
        typename LayerInterfaceType::WeightSetInterfaceType* GetWeightSet(unsigned int id)
        {
        return m_Weights[id].GetPointer();
        }
#ifdef __USE_OLD_INTERFACE
        const typename LayerInterfaceType::WeightSetInterfaceType* GetWeightSet(unsigned int id) const;
#endif

        void SetLearningFunction(LearningFunctionInterfaceType* f);

        virtual NetworkOutputType GenerateOutput(TMeasurementVector samplevector);

        virtual void BackwardPropagate(NetworkOutputType errors);
        virtual void UpdateWeights(ValueType);

        void SetLearningRule(LearningFunctionInterfaceType*);

        void SetLearningRate(ValueType learningrate);

        void InitializeWeights();

      protected:
        MultilayerNeuralNetworkBase();
        ~MultilayerNeuralNetworkBase();

        LayerVectorType                 m_Layers;
        WeightVectorType                m_Weights;
        typename LearningFunctionInterfaceType::Pointer   m_LearningFunction;
        ValueType                       m_LearningRate;
//#define __USE_OLD_INTERFACE  Comment out to ensure that new interface works
#ifdef __USE_OLD_INTERFACE
        //These are completely redundant variables that can be more reliably queried from
        // m_Layers->size() and m_Weights->size();
        int                             m_NumOfLayers;
        int                             m_NumOfWeightSets;
#endif
        /** Method to print the object. */
        virtual void PrintSelf( std::ostream& os, Indent indent ) const;
        };

    } // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMultilayerNeuralNetworkBase.txx"
#endif

#endif
