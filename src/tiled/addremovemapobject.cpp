/*
 * addremovemapobject.cpp
 * Copyright 2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "addremovemapobject.h"

#include "mapdocument.h"
#include "mapobject.h"
#include "objectgroup.h"
#include "mapobjectmodel.h"
#include "layer.h"
#include <iostream>

#include <QCoreApplication>

using namespace Tiled;
using namespace Tiled::Internal;

AddRemoveMapObject::AddRemoveMapObject(MapDocument *mapDocument,
                                       ObjectGroup *objectGroup,
                                       MapObject *mapObject,
                                       bool ownObject,
                                       QUndoCommand *parent)
    : QUndoCommand(parent)
    , mMapDocument(mapDocument)
    , mMapObject(mapObject)
    , mObjectGroup(objectGroup)
    , mIndex(-1)
    , mOwnsObject(ownObject)
{
//	std::cout << "Constructor for addremovemapobject" << std::endl;
}

AddRemoveMapObject::~AddRemoveMapObject()
{
    if (mOwnsObject)
        delete mMapObject;
	
//	std::cout << "Deconstructed sucessfully!" << std::endl;
}

void AddRemoveMapObject::addObject()
{
	//std::cout << "Adding Object" << std::endl;

    mMapDocument->mapObjectModel()->insertObject(mObjectGroup, mIndex,
                                                 mMapObject);
    mOwnsObject = false;
}

void AddRemoveMapObject::removeObject()
{
    mIndex = mMapDocument->mapObjectModel()->removeObject(mObjectGroup,
                                                          mMapObject);
    mOwnsObject = true;
}


AddMapObject::AddMapObject(MapDocument *mapDocument, ObjectGroup *objectGroup,
                           MapObject *mapObject, QUndoCommand *parent)
    : AddRemoveMapObject(mapDocument,
                         objectGroup,
                         mapObject,
                         true,
                         parent)
{
    setText(QCoreApplication::translate("Undo Commands", "Add Object"));
}


RemoveMapObject::RemoveMapObject(MapDocument *mapDocument,
                                 MapObject *mapObject,
                                 QUndoCommand *parent)
    : AddRemoveMapObject(mapDocument,
                         mapObject->objectGroup(),
                         mapObject,
                         false,
                         parent)
{
    setText(QCoreApplication::translate("Undo Commands", "Remove Object"));
}

PreserveMapObject::PreserveMapObject(MapDocument *mapDocument, 
				     MapObject *mapObject, 
				     ObjectGroup *og, 
				     QUndoCommand *parent)
	: AddRemoveMapObject(mapDocument,
			     mapObject->objectGroup(),
			     mapObject,
			     false,
			     parent)
{
	setText(QCoreApplication::translate("Undo Commands", "Remove Object"));

	
	//add object to it's layer's preservation structure
	og->preserveObject(mapObject);
	
}
