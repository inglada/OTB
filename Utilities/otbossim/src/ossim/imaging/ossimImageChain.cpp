//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageChain.cpp 16816 2010-03-07 17:23:23Z dburken $
#include <algorithm>
#include <iostream>
#include <iterator>

#include <ossim/imaging/ossimImageChain.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimIdManager.h>

static ossimTrace traceDebug("ossimImageChain");


RTTI_DEF3(ossimImageChain, "ossimImageChain", ossimImageSource,
          ossimConnectableObjectListener, ossimConnectableContainerInterface);

class ossimImageChainChildListener : public ossimConnectableObjectListener
{
public:
   ossimImageChainChildListener(ossimImageChain* owner)
      :theChain(owner)
      {
      }
   virtual void processEvent(ossimEvent& event);
//   virtual void propertyEvent(ossimPropertyEvent& event);
//   virtual void refreshEvent(ossimRefreshEvent& /* event */);
   ossimImageChain* theChain;
};

void ossimImageChainChildListener::processEvent(ossimEvent& event)
{
   if(!theChain) return;
   ossimConnectableObjectListener::processEvent(event);
   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
                                          event.getCurrentObject());
   
   if((ossimConnectableObject*)theChain->getFirstSource() == obj)
   {
      if(event.isPropagatingToOutputs())
      {
         ossimConnectableObject::ConnectableObjectList& outputList = theChain->getOutputList();
         ossim_uint32 idx = 0;
         for(idx = 0; idx < outputList.size();++idx)
         {
            if(outputList[idx].valid())
            {
               outputList[idx]->fireEvent(event);
               outputList[idx]->propagateEventToOutputs(event);
            }
         }
      }
   }
}
#if 0
void ossimImageChainChildListener::propertyEvent(ossimPropertyEvent& event)
{
   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
                                          event.getObject());
   
   
   // if it originated from within my chain then propagate up to parent
   // only if I have no outputs
   //
   if(obj)
   {
      if(!theChain->findObject(PTR_CAST(ossimConnectableObject,event.getObject())))
      {
         ossimConnectableObject* interface = theChain->findObject(obj->getId(), false);
         
         if(interface)
         {
            ossimConnectableObject* parent = PTR_CAST(ossimConnectableObject,
                                                      theChain->getOwner());
            if(parent&&
               !theChain->getNumberOfOutputs())
            {
               parent->propagateEventToOutputs(event);
            }
         }
      }
      else
      {
         if(!theChain->thePropagateEventFlag)
         {
            theChain->propagateEventToOutputs(event);
         }
      }
   }
}
#endif

ossimImageChain::ossimImageChain()
:ossimImageSource(0,
                  0, // number of inputs
                  0, // number of outputs
                  false, // input's fixed
                  false), // outputs ar not fixed
    ossimConnectableContainerInterface((ossimObject*)NULL),
    theBlankTile(NULL),
    theLoadStateFlag(false)
{
   ossimConnectableContainerInterface::theBaseObject = this;
   //thePropagateEventFlag = false;
   addListener((ossimConnectableObjectListener*)this);
   theChildListener = new ossimImageChainChildListener(this);
}

ossimImageChain::~ossimImageChain()
{
   removeListener((ossimConnectableObjectListener*)this);
   deleteList();

   if(theChildListener)
   {
      delete theChildListener;
      theChildListener = NULL;
   }
}

bool ossimImageChain::addFirst(ossimConnectableObject* obj)
{
   ossimConnectableObject* rightOfThisObj =
      (ossimConnectableObject*)getFirstObject();

   return insertRight(obj, rightOfThisObj);
}

bool ossimImageChain::addLast(ossimConnectableObject* obj)
{
   if(theImageChainList.size() > 0)
   {
      ossimConnectableObject* lastSource = theImageChainList[ theImageChainList.size() -1].get();
      if(dynamic_cast<ossimImageSource*>(obj)&&lastSource)
      {
         obj->disconnect();
         ossimConnectableObject::ConnectableObjectList tempIn = getInputList();
         lastSource->disconnectAllInputs();
         lastSource->connectMyInputTo(obj);
         obj->changeOwner(this);
         obj->connectInputList(tempIn);

         tempIn = obj->getInputList();
         theInputListIsFixedFlag = obj->getInputListIsFixedFlag();
         setNumberOfInputs(obj->getNumberOfInputs());
         theImageChainList.push_back(obj);
         
         obj->addListener((ossimConnectableObjectListener*)this);
         return true;
      }
   }
   else
   {
      return add(obj);
   }

   return false;;
}

ossimImageSource* ossimImageChain::getFirstSource()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>(theImageChainList[0].get());
   }

   return 0;
}

ossimObject* ossimImageChain::getFirstObject()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>(theImageChainList[0].get());
   }

   return 0;
}

ossimImageSource* ossimImageChain::getLastSource()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(theImageChainList.end()-1)).get());
   }

   return NULL;
}

ossimObject* ossimImageChain::getLastObject()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(theImageChainList.end()-1)).get());
   }

   return 0;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimId& id,
                                                    bool recurse)
{
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).get())
      {
         if(id == (*current)->getId())
         {
            return (*current).get();
         }
      }
      
      ++current;
   }
   
   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface,
                                                         (*current).get());
      
      if(child)
      {
         ossimConnectableObject* object = child->findObject(id, true);

         if(object) return object;
      }
      ++current;
   }
   
   return NULL;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimConnectableObject* obj,
                                                    bool recurse)
{
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid())
      {
         if(obj == (*current).get())
         {
            return (*current).get();
         }
      }
      
      ++current;
   }
   
   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
      
      if(child)
      {
         ossimConnectableObject* object = child->findObject(obj, true);

         if(object) return object;
      }
      ++current;
   }
   
   return 0;
}

ossimConnectableObject* ossimImageChain::findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                               bool recurse)
{
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(typeInfo))
      {
         return (*current).get();
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, 
                                                            (*current).get());
         
         if(child)
         {
            ossimConnectableObject* temp = child->findFirstObjectOfType(typeInfo, recurse);
            if(temp)
            {
               return temp;
            }
         }
         ++current;
      }
   }
   
   return (ossimConnectableObject*)NULL;   
}

ossimConnectableObject* ossimImageChain::findFirstObjectOfType(const ossimString& className,
                                                               bool recurse)
{
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(className) )
      {
         return (*current).get();
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject* temp = child->findFirstObjectOfType(className, recurse);
            if(temp)
            {
               return temp;
            }
         }
         ++current;
      }
   }
   
   return (ossimConnectableObject*)0;   
}

ossimConnectableObject::ConnectableObjectList ossimImageChain::findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                           bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;
   ossimConnectableObject::ConnectableObjectList::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(typeInfo))
      {
         ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), 
                                                                                      result.end(), 
                                                                                      (*current).get());
         if(iter == result.end())
         {
            result.push_back((*current).get());
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject::ConnectableObjectList temp;
            temp = child->findAllObjectsOfType(typeInfo, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), temp[index]);
               if(iter == result.end())
               {
                  result.push_back(temp[index]);
               }
            }
         }
         ++current;
      }
   }
   
   return result;
   
}

ossimConnectableObject::ConnectableObjectList ossimImageChain::findAllObjectsOfType(const ossimString& className,
                                                                           bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;
   ossimConnectableObject::ConnectableObjectList::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(className))
      {
         ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), 
                                                                         result.end(), 
                                                                         (*current).get());
         if(iter == result.end())
         {
            result.push_back((*current).get());
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject::ConnectableObjectList temp;
            temp = child->findAllObjectsOfType(className, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), temp[index]);
               if(iter == result.end())
               {
                  result.push_back(temp[index]);
               }
            }
         }
         ++current;
      }
   }
   
   return result;
   
}

void ossimImageChain::makeUniqueIds()
{
   setId(ossimIdManager::instance()->generateId());
   for(long index = 0; index < (long)theImageChainList.size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               theImageChainList[index].get());
      if(container)
      {
         container->makeUniqueIds();
      }
      else
      {
         if(theImageChainList[index].valid())
         {
            theImageChainList[index]->setId(ossimIdManager::instance()->generateId());
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfObjects(bool recurse)const
{
   ossim_uint32 result = (ossim_uint32)theImageChainList.size();
   
   if(recurse)
   {
      for(ossim_uint32 i = 0; i < theImageChainList.size(); ++i)
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, theImageChainList[i].get());
         if(child)
         {
            result += child->getNumberOfObjects(true);
         }
      }
   }
   
   return result;   
}

ossim_uint32 ossimImageChain::getNumberOfSources() const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimImageChain::getNumberOfSources is deprecated!"
      << "\nUse: ossimImageChain::getNumberOfObjects(false)"
      << std::endl;
   return getNumberOfObjects(false);
}

bool ossimImageChain::addChild(ossimConnectableObject* object)
{
   return add(object);
}

bool ossimImageChain::removeChild(ossimConnectableObject* object)
{
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   ossim_uint32 i = 0;
   while(current != theImageChainList.end())
   {
      if((*current).valid())
      {
         if(object == (*current).get())
         {
            // Send an event to any listeners.
            ossimContainerEvent event((ossimObject*)this,
                                      (ossimObject*)object,
                                      OSSIM_EVENT_REMOVE_OBJECT_ID);
            fireEvent(event);
            object->changeOwner(NULL);
            
            object->removeListener((ossimConnectableObjectListener*)this);
            object->removeListener((ossimConnectableObjectListener*)theChildListener);
            
            ossimConnectableObject::ConnectableObjectList input  = object->getInputList();
            ossimConnectableObject::ConnectableObjectList output = object->getOutputList();
            
            // remember the old size before removing
            ossim_uint32 chainSize = (ossim_uint32)theImageChainList.size();
            current = theImageChainList.erase(current);
            
            // Clear connections between this object and child.
            object->disconnect();
            
            //              object->disconnect(this);
            //             object->disconnectAllInputs();
            if(i == 0) // is it the first in the chain
            {
               theImageChainList[0]->removeListener(theChildListener);
               theImageChainList[0]->addListener(theChildListener);
            }
            else if(i == (chainSize-1)) // is it the last
            {
               if(chainSize > 1)
               {
                  ossimConnectableObject* tempObj = theImageChainList[theImageChainList.size()-1].get();
                  if(tempObj)
                  {
                     tempObj->connectInputList(input);
                  }
               }
            }
            else // must be an interior
            {
               ossim_uint32 inIndex  = 0;
               
               for(inIndex = 0; inIndex < input.size(); ++inIndex)
               {
                  if(input[inIndex].valid())
                  {
                     input[inIndex]->disconnectAllOutputs();
                     input[inIndex]->connectOutputList(output);
                  }
               }
            }
            
            return true;
         }
      }
      ++current;
      ++i;
   }
   
   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
      
      if(child)
      {
         bool result = child->removeChild(object);
         if(result)
         {
            return result;
         }
      }
      ++current;
   }
   
   return false;
}

ossimConnectableObject* ossimImageChain::removeChild(const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, true);

   removeChild(obj);
   
   return obj;
}

void ossimImageChain::getChildren(vector<ossimConnectableObject*>& children,
                                  bool immediateChildrenOnlyFlag)
{
   ossim_uint32 i = 0;
   
   vector<ossimConnectableObject*> temp;
   for(i = 0; i < theImageChainList.size();++i)
   {
      temp.push_back(theImageChainList[i].get());
   }

   for(i = 0; i < temp.size();++i)
   {
      ossimConnectableContainerInterface* interface = PTR_CAST(ossimConnectableContainerInterface,
                                                               temp[i]);
      if(immediateChildrenOnlyFlag)
      {
         children.push_back(temp[i]);
      }
      else if(!interface)
      {
         children.push_back(temp[i]);         
      }
   }
   
   if(!immediateChildrenOnlyFlag)
   {
      for(i = 0; i < temp.size();++i)
      {
         ossimConnectableContainerInterface* interface = PTR_CAST(ossimConnectableContainerInterface,
                                                                  temp[i]);
         if(interface)
         {
            interface->getChildren(children, false);
         }
      }
   }
}

bool ossimImageChain::add(ossimConnectableObject* source)
{
   bool result = false;
   if(PTR_CAST(ossimImageSource, source))
   {
     source->changeOwner(this);
     if(theImageChainList.size() > 0)
     {
        source->disconnectAllOutputs();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
        theImageChainList[0]->removeListener(theChildListener);
        theImageChainList.insert(theImageChainList.begin(), source);
        theImageChainList[0]->addListener(theChildListener);
        source->addListener((ossimConnectableObjectListener*)this);
        theImageChainList[0]->connectMyInputTo(theImageChainList[1].get());
        result = true;
     }
     else
     {
#if 1
        theInputListIsFixedFlag = false;
        theOutputListIsFixedFlag = false;
        
        if(!theInputObjectList.empty())
        {
           source->connectInputList(getInputList());
        }
        else
        {
           theInputObjectList = source->getInputList();
        }
        theInputListIsFixedFlag = source->getInputListIsFixedFlag();
        theOutputObjectList      = source->getOutputList();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
        theImageChainList.push_back(source);
        source->addListener((ossimConnectableObjectListener*)this);
        theImageChainList[0]->addListener(theChildListener);
        result = true;
#endif
#if 0
        theInputObjectList = source->getInputList();
        theInputListIsFixedFlag = source->getInputListIsFixedFlag();
        
        theOutputObjectList      = source->getOutputList();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
   
        theImageChainList.push_back(source);
        source->addListener((ossimConnectableObjectListener*)this);
        theImageChainList[0]->addListener(theChildListener);
        result = true;
#endif
     }
   }

   if (result && source)
   {
      ossimContainerEvent event(this, source, OSSIM_EVENT_ADD_OBJECT_ID);
      fireEvent(event);
   }
   
   return result;
}

bool ossimImageChain::insertRight(ossimConnectableObject* newObj,
                                  ossimConnectableObject* rightOfThisObj)
{
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   
   if(findObject(rightOfThisObj, false))
   {
      std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = theImageChainList.begin();
      while(iter != theImageChainList.end())
      {
         if( (*iter) == rightOfThisObj)
         {
            break;
         }
         ++iter;
      }
      if(rightOfThisObj == theImageChainList[0].get())
      {
         return add(newObj);
      }
      else if(PTR_CAST(ossimImageSource, newObj))
      {
         ossimConnectableObject::ConnectableObjectList outputList = rightOfThisObj->getOutputList();
         rightOfThisObj->disconnectAllOutputs();

         // Core dump fix.  Connect input prior to outputs. (drb)
         newObj->connectMyInputTo(rightOfThisObj); 
         newObj->connectOutputList(outputList);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter, newObj);
         if (newObj)
         {
            // Send event to any listeners.
            ossimContainerEvent event(this, newObj, OSSIM_EVENT_ADD_OBJECT_ID);
            fireEvent(event);
         }
         return true;
      }
   }

   return false;
}

bool ossimImageChain::insertRight(ossimConnectableObject* newObj,
                                  const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertRight(newObj, obj);
   }

   return false;
}

bool ossimImageChain::insertLeft(ossimConnectableObject* newObj,
                                 ossimConnectableObject* leftOfThisObj)
{
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   
   if(findObject(leftOfThisObj, false))
   {
      std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = theImageChainList.begin();

      while(iter != theImageChainList.end())
      {
         if( (*iter) == leftOfThisObj)
         {
            break;
         }
         ++iter;
      }
      if(leftOfThisObj==theImageChainList[theImageChainList.size()-1].get())
      {
         return addLast(newObj);
      }
      else if(PTR_CAST(ossimImageSource, newObj))
      {
         ossimConnectableObject::ConnectableObjectList inputList = leftOfThisObj->getInputList();
         leftOfThisObj->disconnectAllInputs();
         newObj->connectInputList(inputList);
         leftOfThisObj->connectMyInputTo(newObj);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter+1, newObj);
         if (newObj)
         {
            // Send an event to any listeners.
            ossimContainerEvent event(this, newObj, OSSIM_EVENT_ADD_OBJECT_ID);
            fireEvent(event);
         }
         return true;
      }
   }

   return false;
}

bool ossimImageChain::insertLeft(ossimConnectableObject* newObj,
                                  const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertLeft(newObj, obj);
   }

   return false;
}

bool ossimImageChain::replace(ossimConnectableObject* newObj,
                              ossimConnectableObject* oldObj)
{
   ossim_int32 idx = indexOf(oldObj);
   if(idx >= 0)
   {
      ossimConnectableObject::ConnectableObjectList& inputList = getInputList();
      ossimConnectableObject::ConnectableObjectList& outputList = oldObj->getOutputList();
      oldObj->removeListener((ossimConnectableObjectListener*)this);
      oldObj->removeListener(theChildListener);
      theImageChainList[idx] = newObj;
      newObj->connectInputList(inputList);
      newObj->connectOutputList(outputList);
      newObj->changeOwner(this);
      newObj->addListener((ossimConnectableObjectListener*)this);
      if(idx == 0)
      {
         newObj->addListener(theChildListener);
      }
   }
   
   return (idx >= 0);
}

ossimRefPtr<ossimImageData> ossimImageChain::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                             theImageChainList[0].get());
      
      if(interface)
      {
         // make sure we initialize in reverse order.
         // some source may depend on the initialization of
         // its inputs
         return interface->getTile(tileRect, resLevel);
      }  
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource, getInput(0));
         if(interface)
         {
            return interface->getTile(tileRect, resLevel);
         }
      }
   }

   if(theBlankTile.get())
   {
      theBlankTile->setOrigin(tileRect.ul());
      theBlankTile->setWidthHeight(tileRect.width(),
                                   tileRect.height());
   }
   
   return theBlankTile;
}

ossim_uint32 ossimImageChain::getNumberOfInputBands() const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getNumberOfOutputBands();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNumberOfOutputBands();
         }
      }
   }

   return 0;
}

double ossimImageChain::getNullPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getNullPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNullPixelValue(band);
         }
      }
   }
   
   return ossim::defaultNull(getOutputScalarType());
}

double ossimImageChain::getMinPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getMinPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMinPixelValue(band);
         }
      }
   }

   return ossim::defaultMin(getOutputScalarType());
}

double ossimImageChain::getMaxPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                                  theImageChainList[0].get());
      if(inter)
      {
         return inter->getMaxPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMaxPixelValue(band);
         }
      }
   }

   return ossim::defaultMax(getOutputScalarType());
}

void ossimImageChain::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                                  theImageChainList[0].get());
      if(inter)
      {
         return inter->getOutputBandList(bandList);
      }
   }
}
   
ossimScalarType ossimImageChain::getOutputScalarType() const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getOutputScalarType();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getOutputScalarType();
         }
      }
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimImageChain::getTileWidth()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         theImageChainList[0].get());
         if(interface)
         {
            return interface->getTileWidth();;
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getTileWidth();
         }
      }
   }
   
   return 0;   
}

ossim_uint32 ossimImageChain::getTileHeight()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         theImageChainList[0].get());
         if(interface)
         {
            return interface->getTileHeight();
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getTileHeight();
         }
      }
   }
   
   return 0;   
}
   
ossimIrect ossimImageChain::getBoundingRect(ossim_uint32 resLevel)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {

      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());

      if(interface)
      {
         return interface->getBoundingRect(resLevel);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getBoundingRect();
         }
      }
   }
   ossimDrect rect;
   rect.makeNan();
   
   return rect;
   
}

void ossimImageChain::getValidImageVertices(vector<ossimIpt>& validVertices,
                                            ossimVertexOrdering ordering,
                                            ossim_uint32 resLevel)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface =PTR_CAST(ossimImageSource,
                                                     theImageChainList[0].get());

      if(interface)
      {
         interface->getValidImageVertices(validVertices,
                                          ordering,
                                          resLevel);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getValidImageVertices(validVertices,
                                             ordering,
                                             resLevel);
         }
      }
   }
}

ossimImageGeometry*  ossimImageChain::getImageGeometry()
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource, theImageChainList[0].get());
      if(interface)
      {
         return interface->getImageGeometry();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource, getInput(0));
         if(interface)
         {
            return interface->getImageGeometry();
         }
      }
      
   }
   return 0;
}

void ossimImageChain::getDecimationFactor(ossim_uint32 resLevel,
                                          ossimDpt& result) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         interface->getDecimationFactor(resLevel,
                                        result);
         return;
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getDecimationFactor(resLevel, result);
            return;
         }
      }
   }

   result.makeNan();
}

void ossimImageChain::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         interface->getDecimationFactors(decimations);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getDecimationFactors(decimations);
            return;
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfDecimationLevels()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());

      if(interface)
      {
         return interface->getNumberOfDecimationLevels();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNumberOfDecimationLevels();
         }
      }
   }

   return 1;
}

bool ossimImageChain::addAllSources(map<ossimId, vector<ossimId> >& idMapping,
                                    const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   static const char* MODULE = "ossimImageChain::addAllSources";
   ossimString copyPrefix = prefix;
   bool result =  ossimImageSource::loadState(kwl, copyPrefix.c_str());

   if(!result)
   {
      return result;
   }
   long index = 0;

//   ossimSource* source = NULL;

   vector<ossimId> inputConnectionIds;
   ossimString regExpression =  ossimString("^(") + copyPrefix + "object[0-9]+.)";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   long numberOfSources = (long)keys.size();//kwl.getNumberOfSubstringKeys(regExpression);

   int offset = (int)(copyPrefix+"object").size();
   int idx = 0;
   std::vector<int> theNumberList(numberOfSources);
   for(idx = 0; idx < (int)theNumberList.size();++idx)
     {
       ossimString numberStr(keys[idx].begin() + offset,
			     keys[idx].end());
       theNumberList[idx] = numberStr.toInt();
     }
   std::sort(theNumberList.begin(), theNumberList.end());
   for(idx=0;idx < (int)theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("object");
      newPrefix += ossimString::toString(theNumberList[idx]);
      newPrefix += ossimString(".");

      if(traceDebug())
      {
         CLOG << "trying to create source with prefix: " << newPrefix
              << std::endl;
      }
      ossimRefPtr<ossimObject> object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      ossimConnectableObject* source = PTR_CAST(ossimConnectableObject, object.get());
      
      if(source)
      {
         // we did find a source so include it in the count
         if(traceDebug())
         {
            CLOG << "Created source with prefix: " << newPrefix << std::endl;
         }
         if(PTR_CAST(ossimImageSource, source))
         {
            ossimId id = source->getId();
            inputConnectionIds.clear();
            
            findInputConnectionIds(inputConnectionIds,
                                   kwl,
                                   newPrefix);
            if(inputConnectionIds.size() == 0)
            {
               // we will try to do a default connection
               //
               
               if(theImageChainList.size())
               {
                  if(traceDebug())
                  {
                     CLOG << "connecting " << source->getClassName() << " to "
                          << theImageChainList[0]->getClassName() << std::endl;
                  }
                  source->connectMyInputTo(0, theImageChainList[0].get());
               }
            }
            else
            {
               // we remember the connection id's so we can connect this later.
               // this way we make sure all sources were actually
               // allocated.
               //
               idMapping.insert(std::make_pair(id, inputConnectionIds));
            }
            add(source);
         }
         else
         {
            source = 0;
         }
      }
      else
      {
         object = 0;
         source = 0;
      }
      
      ++index;
   }
   if(theImageChainList.size())
   {
     ossimConnectableObject* obj = theImageChainList[(ossim_int32)theImageChainList.size()-1].get();
     if(obj)
     {
        setNumberOfInputs(obj->getNumberOfInputs());
     }
   }
   
   return result;
}

void ossimImageChain::findInputConnectionIds(vector<ossimId>& result,
                                             const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   ossimString copyPrefix = prefix;
   ossim_uint32 idx = 0;
   
   ossimString regExpression =  ossimString("^") + ossimString(prefix) + "input_connection[0-9]+";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   
   ossim_int32 offset = (ossim_int32)(copyPrefix+"input_connection").size();
   ossim_uint32 numberOfKeys = (ossim_uint32)keys.size();
   std::vector<int> theNumberList(numberOfKeys);
   for(idx = 0; idx < theNumberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
   }
   std::sort(theNumberList.begin(), theNumberList.end());
   copyPrefix += ossimString("input_connection");
   for(idx=0;idx < theNumberList.size();++idx)
   {
      const char* lookup = kwl.find(copyPrefix,ossimString::toString(theNumberList[idx]));
      if(lookup)
      {
         long id = ossimString(lookup).toLong();
         result.push_back(ossimId(id));
      }
   }
}
                    

bool ossimImageChain::connectAllSources(const map<ossimId, vector<ossimId> >& idMapping)
{
   if(idMapping.size())
   {
      map<ossimId, vector<ossimId> >::const_iterator iter = idMapping.begin();

      while(iter != idMapping.end())
      {
         ossimConnectableObject* currentSource = findObject((*iter).first);

         if(currentSource)
         {
            long upperBound = (long)(*iter).second.size();
            for(long index = 0; index < upperBound; ++index)
            {
               if((*iter).second[index].getId() > -1)
               {
                  ossimConnectableObject* inputSource = PTR_CAST(ossimConnectableObject, findObject((*iter).second[index]));
                  currentSource->connectMyInputTo(index, inputSource);
                  
               }
               else
               {
                  currentSource->disconnectMyInput((ossim_int32)index);
               }
            }
         }
         else
         {
            cerr << "Could not find " << (*iter).first << " for source: ";
            return false;
         }
         ++iter;
      }
   }

   return true;
}

bool ossimImageChain::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   bool result = true;
   
   result = ossimImageSource::saveState(kwl, prefix);

   if(!result)
   {
      return result;
   }
   ossim_uint32 upper = (ossim_uint32)theImageChainList.size();
   ossim_uint32 counter = 1;

   if (upper)
   {
      ossim_int32 idx = upper-1;
      // start with the tail and go to the head fo the list.
      for(;((idx >= 0)&&result);--idx, ++counter)
      {
         ossimString newPrefix = prefix;
         
         newPrefix += (ossimString("object") +
                       ossimString::toString(counter) +
                       ossimString("."));
         result = theImageChainList[idx]->saveState(kwl, newPrefix.c_str());
      }
   }

   return result;
}

bool ossimImageChain::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   static const char* MODULE = "ossimImageChain::loadState(kwl, prefix)";
   deleteList();

   ossimImageSource::loadState(kwl, prefix);
   
   
   theLoadStateFlag = true;
   bool result = true;
   
   map<ossimId, vector<ossimId> > idMapping;
   result = addAllSources(idMapping, kwl, prefix);
   if(!result)
   {
      CLOG << "problems adding sources" << std::endl;
   }
   result = connectAllSources(idMapping);
   if(!result)
   {
      CLOG << "problems connecting sources" << std::endl;
   }
   
   theLoadStateFlag = false;
   return result;
}


void ossimImageChain::initialize()
{
   static const char* MODULE = "ossimImageChain::initialize()";
   if (traceDebug()) CLOG << " Entered..." << std::endl;
   
   long upper = (ossim_uint32)theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      if(traceDebug())
      {
         CLOG << "initilizing source: "
              << theImageChainList[index]->getClassName()
              << std::endl;
      }
      if(theImageChainList[index].valid())
      {
         ossimImageSource* interface =
            PTR_CAST(ossimImageSource, theImageChainList[index].get());

         if(interface)
         {
            // make sure we initialize in reverse order.
            // some source may depend on the initialization of
            // its inputs
            interface->initialize();
         }
      }
   }
   if (traceDebug()) CLOG << " Exited..." << std::endl;
}

void ossimImageChain::enableSource()
{
   ossim_int32 upper = static_cast<ossim_int32>(theImageChainList.size());
   ossim_int32 index = 0;
   for(index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index].get());
     if(source)
     {
        source->enableSource();
     }
   }
   
   theEnableFlag = true;
}

void ossimImageChain::disableSource()
{
   long upper = (ossim_uint32)theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index].get());
     if(source)
     {
        source->disableSource();
     }
   }
   
   theEnableFlag = false;
}

bool ossimImageChain::deleteFirst()
{
   if (theImageChainList.size() == 0) return false;

   // Clear any listeners, memory.
   ossim_uint32 index = 0;
   theImageChainList[index]->
      removeListener((ossimConnectableObjectListener*)this);
   theImageChainList[index]->removeListener(theChildListener);
   theImageChainList[index]->changeOwner(NULL);
   theImageChainList[index] = 0;
   
   // Remove from the vector.
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator i = theImageChainList.begin();
   theImageChainList.erase(i);
   return true;
}

bool ossimImageChain::deleteLast()
{
   if (theImageChainList.size() == 0) return false;

   // Clear any listeners, memory.
   ossim_uint32 index = (ossim_uint32)theImageChainList.size() - 1;
   theImageChainList[index]->
      removeListener((ossimConnectableObjectListener*)this);
   theImageChainList[index]->removeListener(theChildListener);
   theImageChainList[index]->changeOwner(NULL);
   theImageChainList[index] = 0;
   
   // Remove from the vector.
   theImageChainList.pop_back();
   return true; 
}

void ossimImageChain::deleteList()
{
   long upper = (long)theImageChainList.size();
   for(long index = 0; index < upper; ++index)
   {
      theImageChainList[index]->removeListener((ossimConnectableObjectListener*)this);
      theImageChainList[index]->removeListener(theChildListener);
      theImageChainList[index]->disconnect();
   }
   
   theImageChainList.clear();
}

void ossimImageChain::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfOldObjects(); ++i)
            {
               theImageChainList[theImageChainList.size()-1]->disconnectMyInput(event.getOldObject(i));
            }
         }
      }
   }
}

void ossimImageChain::disconnectOutputEvent(ossimConnectionEvent& event)
{
}

void ossimImageChain::connectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfNewObjects(); ++i)
            {
               ossimConnectableObject* obj = event.getNewObject(i);
               theImageChainList[theImageChainList.size()-1]->connectMyInputTo(findInputIndex(obj),
                                                                               obj,
                                                                               false);
            }
         }
      }
      else if(event.getObject() == theImageChainList[0].get())
      {
         if(!theLoadStateFlag)
         {
//            theInputObjectList = theImageChainList[0]->getInputList();
         }
      }
      initialize();
   }
}

void ossimImageChain::connectOutputEvent(ossimConnectionEvent& event)
{
} 

// void ossimImageChain::propertyEvent(ossimPropertyEvent& event)
// {
//    if(theImageChainList.size())
//    {
//       ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
//                                              event.getObject());
      
//       if(obj)
//       {
//          ossimImageSource* interface = findSource(obj->getId());
         
//          if(interface)
//          {
//             ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
//                                                    interface.getObject());
//             if(obj)
//             {
               
//             }
//          }
//       }
//    }
// }

void ossimImageChain::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if(!event.getObject()) return;

   if(theImageChainList.size()&&(event.getObject()!=this))
   {
      removeChild(PTR_CAST(ossimConnectableObject,
                           event.getObject()));
   }
}
void ossimImageChain::propagateEventToOutputs(ossimEvent& event)
{
   //if(thePropagateEventFlag) return;

   //thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[theImageChainList.size()-1].valid())
      {
         theImageChainList[theImageChainList.size()-1]->fireEvent(event);
         theImageChainList[theImageChainList.size()-1]->propagateEventToOutputs(event);
      }
   }
   //ossimConnectableObject::propagateEventToOutputs(event);
  // thePropagateEventFlag = false;
}
void ossimImageChain::propagateEventToInputs(ossimEvent& event)
{
//   if(thePropagateEventFlag) return;

//   thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[0].valid())
      {
         theImageChainList[0]->fireEvent(event);
         theImageChainList[0]->propagateEventToInputs(event);
      }
   }
//   thePropagateEventFlag = false;
}

ossimConnectableObject* ossimImageChain::operator[](ossim_uint32 index)
{
   return getConnectableObject(index);
}

ossimConnectableObject* ossimImageChain::getConnectableObject(
   ossim_uint32 index)
{
   if(theImageChainList.size() && (index < theImageChainList.size()))
   {
      return theImageChainList[index].get();
   }
   
   return 0; 
}

ossim_int32 ossimImageChain::indexOf(ossimConnectableObject* obj)const
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < theImageChainList.size();++idx)
   {
      if(theImageChainList[idx] == obj)
      {
         return (ossim_int32)idx;
      }
   }
   
   return -1;
}
