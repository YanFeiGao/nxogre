/** File: NxOgrePhysXCallback.cpp
    Created on: 31-Jan-09
    Author: Robin Southern "betajaen"
    Last-Modified-For: PhysX 2.8.1
    SVN: $Id$

    � Copyright, 2008-2009 by Robin Southern, http://www.nxogre.org

    This file is part of NxOgre.

    NxOgre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NxOgre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with NxOgre.  If not, see <http://www.gnu.org/licenses/>.
*/

                                                                                       

#include "NxOgreStable.h"
#include "NxOgrePhysXCallback.h"
#include "NxOgreCallback.h"

#include "NxOgreVolume.h"
#include "NxOgreShape.h"
#include "NxOgreBox.h"
#include "NxOgreRigidBodyFunctions.h"
#include "NxOgreRigidBody.h"
#include "NxOgreContactPair.h"

                                                                                       

namespace NxOgre_Namespace
{

                                                                                       

PhysXCallback::PhysXCallback(Scene* scene) : mScene(scene)
{
}

PhysXCallback::~PhysXCallback()
{
}

void PhysXCallback::onTrigger(NxShape& triggerShape, NxShape& physxCollisionShape, NxTriggerFlag status)
{
 
 if (!triggerShape.getActor().userData && !physxCollisionShape.getActor().userData)
  return;
 
 PhysXPointer* ptrV = pointer_cast(triggerShape.userData);
 PhysXPointer* ptrC = pointer_cast(physxCollisionShape.userData);
 RigidBody* rbody = ptrV->getParent<RigidBody>();
 
 Volume* volume = static_cast<Volume*>(rbody);
 volume->getCallback()->onVolumeEvent(volume, ptrV->get<Shape>(), ptrC->getParent<RigidBody>(), ptrC->get<Shape>(), status);
 
}

void PhysXCallback::onContactNotify(NxContactPair &pair, NxU32 events)
{
 
 if (pair.isDeletedActor[0] || pair.isDeletedActor[1])
  return;
 
 if (!pair.actors[0]->userData && !pair.actors[1]->userData)
  return;
 
 RigidBody* rbody_a = pointer_representive_cast<RigidBody>(pair.actors[0]->userData);
 RigidBody* rbody_b = pointer_representive_cast<RigidBody>(pair.actors[1]->userData);
 
 ContactPair contact_pair;
 contact_pair.mFirst = rbody_a;
 contact_pair.mSecond = rbody_b;
 contact_pair.mSumFrictionForce = pair.sumFrictionForce;
 contact_pair.mSumNormalForce = pair.sumNormalForce;
 
 if (rbody_a->getContactCallback() != 0)
  rbody_a->getContactCallback()->onContact(contact_pair);
 
 if (rbody_b->getContactCallback() != 0)
  rbody_b->getContactCallback()->onContact(contact_pair);
 
}

                                                                                       

} // namespace NxOgre_Namespace

                                                                                       
