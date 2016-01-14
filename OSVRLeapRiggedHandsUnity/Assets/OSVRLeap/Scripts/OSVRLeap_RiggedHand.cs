/******************************************************************************\
* Copyright (C) Leap Motion, Inc. 2011-2014.                                   *
* Leap Motion proprietary. Licensed under Apache 2.0                           *
* Available at http://www.apache.org/licenses/LICENSE-2.0.html                 *
\******************************************************************************/

using UnityEngine;
using System.Collections;
using Leap;

// Class to setup a rigged hand based on a model.
public class OSVRLeap_RiggedHand : OSVRLeap_HandModel {
	
  public Vector3 modelFingerPointing = Vector3.forward;
  public Vector3 modelPalmFacing = -Vector3.up;

  public override void InitHand() {
    UpdateHand();
  }

  public Quaternion Reorientation() {
    return Quaternion.Inverse(Quaternion.LookRotation(modelFingerPointing, -modelPalmFacing));
  }

  public override void UpdateHand() {
    handInfo_ = (IsLeft ? Analogs.LeftHand : Analogs.RightHand);
    SetOsvrLeapHandInfo(handInfo_);
    if (palm != null) {
      // vector = Quaternion.Euler(0, -45, 0) * vector;

      palm.position = GetPalmPosition();
      palm.rotation = GetPalmRotation() * Reorientation();
      if (IsLeft) {
        palm.rotation = palm.rotation * Quaternion.AngleAxis(180, modelPalmFacing);
      }
    }

    //if (forearm != null)
    //  forearm.rotation = GetArmRotation() * Reorientation();

    for (int i = 0; i < fingers.Length; ++i) {
      if (fingers[i] != null) {
				fingers[i].fingerType = (Finger.FingerType)i;
        fingers[i].UpdateFinger();
			}
		}
  }
}
