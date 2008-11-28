//****************************************************************************//
// corebone.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/error.h"
#include "cal3d/corebone.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coremodel.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"


CalCoreBone::CalCoreBone()
  : m_pCoreSkeleton(0)
  , m_parentId(-1)
{
}


CalCoreBone::~CalCoreBone()
{
  assert(m_listChildId.empty());
}

 /*****************************************************************************/
/** Adds a child ID.
  *
  * This function adds a core bone ID to the child ID list of the core bone
  * instance.
  *
  * @param childId The ID of the core bone ID that shoud be added to the child
  *                ID list.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreBone::addChildId(int childId)
{
  m_listChildId.push_back(childId);

  return true;
}

 /*****************************************************************************/
/** Calculates the current state.
  *
  * This function calculates the current state (absolute translation and
  * rotation) of the core bone instance and all its children.
  *****************************************************************************/

void CalCoreBone::calculateState()
{
  if(m_parentId == -1)
  {
    // no parent, this means absolute state == relative state
    m_translationAbsolute = m_translation;
    m_rotationAbsolute = m_rotation;
  }
  else
  {
    // get the parent bone
    CalCoreBone *pParent;
    pParent = m_pCoreSkeleton->getCoreBone(m_parentId);

    // transform relative state with the absolute state of the parent
    m_translationAbsolute = m_translation;
    m_translationAbsolute *= pParent->getRotationAbsolute();
    m_translationAbsolute += pParent->getTranslationAbsolute();

    m_rotationAbsolute = m_rotation;
    m_rotationAbsolute *= pParent->getRotationAbsolute();
  }

  // calculate all child bones
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = m_listChildId.begin(); iteratorChildId != m_listChildId.end(); ++iteratorChildId)
  {
    m_pCoreSkeleton->getCoreBone(*iteratorChildId)->calculateState();
  }
}

 /*****************************************************************************/
/** Creates the core bone instance.
  *
  * This function creates the core bone instance.
  *
  * @param strName A string that should be used as the name of the core bone
  *                instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreBone::createInternal(const std::string& strName)
{
  m_strName = strName;
  return true;
}

bool CalCoreBone::createWithName( char const * strName )
{
  std::string name = strName;
  return createInternal( name );
}


char const *
CalCoreBone::getName()
{
  char const * name = m_strName.c_str();
  return name;
}

 /*****************************************************************************/
/** Destroys the core bone instance.
  *
  * This function destroys all data stored in the core bone instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalCoreBone::destroy()
{
  // clear children id list
  m_listChildId.clear();

  m_parentId = -1;
  m_strName.erase();
}

 /*****************************************************************************/
/** Returns the child ID list.
  *
  * This function returns the list that contains all child IDs of the core bone
  * instance.
  *
  * @return A reference to the child ID list.
  *****************************************************************************/

std::list<int>& CalCoreBone::getListChildId()
{
  return m_listChildId;
}

 /*****************************************************************************/
/** Returns the name.
  *
  * This function returns the name of the core bone instance.
  *
  * @return The name as string.
  *****************************************************************************/

const std::string& CalCoreBone::getNameInternal()
{
  return m_strName;
}

 /*****************************************************************************/
/** Returns the parent ID.
  *
  * This function returns the parent ID of the core bone instance.
  *
  * @return One of the following values:
  *         \li the \b ID of the parent
  *         \li \b -1 if the core bone instance is a root core bone
  *****************************************************************************/

int CalCoreBone::getParentId()
{
  return m_parentId;
}

 /*****************************************************************************/
/** Returns the rotation.
  *
  * This function returns the relative rotation of the core bone instance.
  *
  * @return The relative rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalCoreBone::getRotation()
{
  return m_rotation;
}

 /*****************************************************************************/
/** Returns the absolute rotation.
  *
  * This function returns the absolute rotation of the core bone instance.
  *
  * @return The absolute rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalCoreBone::getRotationAbsolute()
{
  return m_rotationAbsolute;
}

 /*****************************************************************************/
/** Returns the bone space rotation.
  *
  * This function returns the rotation to bring a point into the core bone
  * instance space.
  *
  * @return The rotation to bring a point into bone space.
  *****************************************************************************/

const CalQuaternion& CalCoreBone::getRotationBoneSpace()
{
  return m_rotationBoneSpace;
}

 /*****************************************************************************/
/** Returns the translation.
  *
  * This function returns the relative translation of the core bone instance.
  *
  * @return The relative translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalCoreBone::getTranslation()
{
  return m_translation;
}

 /*****************************************************************************/
/** Returns the absolute translation.
  *
  * This function returns the absolute translation of the core bone instance.
  *
  * @return The absolute translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalCoreBone::getTranslationAbsolute()
{
  return m_translationAbsolute;
}

 /*****************************************************************************/
/** Returns the bone space translation.
  *
  * This function returns the translation to bring a point into the core bone
  * instance space.
  *
  * @return The translation to bring a point into bone space.
  *****************************************************************************/

const CalVector& CalCoreBone::getTranslationBoneSpace()
{
  return m_translationBoneSpace;
}

 /*****************************************************************************/
/** Sets the core skeleton.
  *
  * This function sets the core skeleton to which the core bone instance is
  * attached to.
  *
  * @param pCoreSkeleton The core skeleton to which the core bone instance
  *                      should be attached to.
  *****************************************************************************/

void CalCoreBone::setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton)
{
  m_pCoreSkeleton = pCoreSkeleton;
}

 /*****************************************************************************/
/** Sets the parent ID.
  *
  * This function sets the parent ID of the core bone instance.
  *
  * @param parentId The ID of the parent that should be set.
  *****************************************************************************/

void CalCoreBone::setParentId(int parentId)
{
  m_parentId = parentId;
}

/*****************************************************************************/
/** Provides access to the core skeleton.
  *
  * This function returns the core skeleton.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSkeleton *CalCoreBone::getCoreSkeleton()
{
  return m_pCoreSkeleton;
}

 /*****************************************************************************/
/** Sets the rotation.
  *
  * This function sets the relative rotation of the core bone instance.
  *
  * @param rotation The relative rotation to the parent as quaternion.
  *****************************************************************************/

void CalCoreBone::setRotation(const CalQuaternion& rotation)
{
  m_rotation = rotation;
}

 /*****************************************************************************/
/** Sets the bone space rotation.
  *
  * This function sets the rotation that brings a point into the core bone
  * instance space.
  *
  * @param rotation The rotation that brings a point into bone space.
  *****************************************************************************/

void CalCoreBone::setRotationBoneSpace(const CalQuaternion& rotation)
{
  m_rotationBoneSpace = rotation;
}

 /*****************************************************************************/
/** Sets the translation.
  *
  * This function sets the relative translation of the core bone instance.
  *
  * @param translation The relative translation to the parent as vector.
  *****************************************************************************/

void CalCoreBone::setTranslation(const CalVector& translation)
{
  m_translation = translation;
}

 /*****************************************************************************/
/** Sets the bone space translation.
  *
  * This function sets the translation that brings a point into the core bone
  * instance space.
  *
  * @param translation The translation that brings a point into bone space.
  *****************************************************************************/

void CalCoreBone::setTranslationBoneSpace(const CalVector& translation)
{
  m_translationBoneSpace = translation;
}

 /*****************************************************************************/
/** Scale the core bone.
  *
  * This function rescale all the data that are in the core bone instance and
  * in his childs.
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreBone::scale(float factor)
{
	m_translation*=factor;
	m_translationAbsolute*=factor;
	m_translationBoneSpace*=factor;
	
	// calculate all child bones
	std::list<int>::iterator iteratorChildId;
	for(iteratorChildId = m_listChildId.begin(); iteratorChildId != m_listChildId.end(); ++iteratorChildId)
	{
		m_pCoreSkeleton->getCoreBone(*iteratorChildId)->scale(factor);
	}
}


//****************************************************************************//


bool
CalCoreBone::hasLightingData()
{
  return m_lightType != LIGHT_TYPE_NONE;
}


void
CalCoreBone::getLightColor( CalVector & c )
{
  c = m_lightColor;
}


void
CalCoreBone::setLightColor( CalVector const & c )
{
  m_lightColor = c;
}


CalLightType
CalCoreBone::getLightType()
{
  return m_lightType;
}


void
CalCoreBone::setLightType( CalLightType t )
{
  m_lightType = t;
}


