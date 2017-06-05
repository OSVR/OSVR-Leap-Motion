/******************************************************************************\
* Copyright (C) Leap Motion, Inc. 2011-2014.                                   *
* Leap Motion proprietary. Licensed under Apache 2.0                           *
* Available at http://www.apache.org/licenses/LICENSE-2.0.html                 *
\******************************************************************************/

using UnityEngine;
using System.Collections;
using Leap;
using OSVR.ClientKit;
using OSVR.Unity;
using LeapOsvrTest;
using Quaternion = UnityEngine.Quaternion;
using AnalogInterface = OSVR.ClientKit.AnalogInterface;

/**
* The base class for all fingers.
* 
* This class serves as the interface between the HandController object,
* the parent Hand object and the concrete finger objects.
*
* Subclasses of FingerModel must implement InitFinger() and UpdateFinger(). The InitHand() function
* is typically called by the parent HandModel InitHand() method; likewise, the UpdateFinger()
* function is typically called by the parent HandModel UpdateHand() function.
*/

public abstract class OSVRLeap_FingerModel : MonoBehaviour {

  /** The number of bones in a finger. */
  public const int NUM_BONES = 4;

  /** The number of joints in a finger. */
  public const int NUM_JOINTS = 3;

  [HideInInspector]
  public Finger.FingerType fingerType = Finger.FingerType.TYPE_INDEX;

  // Unity references
  /** Bones positioned and rotated by FingerModel. */
  public Transform[] bones = new Transform[NUM_BONES];
  /** Joints positioned and rotated by FingerModel. */
  public Transform[] joints = new Transform[NUM_BONES - 1];

  //OSVR References
  public OsvrContextProvider ContextProvider;
  public LeapAnalogs Analogs;
  public LeapAnalogs.HandInfo handInfo_; 

  // Leap references
  /** The Leap Hand object. */
  protected Hand hand_;
  /** The Leap Finger object. */
  protected Finger finger_;
  /** An added offset vector. */
  protected Vector3 offset_ = Vector3.zero;
  /** Whether this finger is mirrored. */
  protected bool mirror_z_axis_ = false;

 
  public void SetOsvrLeapHandInfo(LeapAnalogs.HandInfo handInfo) {
    handInfo_ = handInfo;
  }


  /** Sets an offset vector to displace the finger from its normally calculated
  * position relative to the HandController. Typically, this offset is used to
  * give the virtual hands a larger range of motion then they would have based on their 
  * scaled size in the Unity scene.
  */
  public void SetOffset(Vector3 offset) {
    offset_ = offset;
  }

  /** 
  * Sets the mirror z-axis flag for this Finger Model.
  * Mirroring the z axis reverses the hand so that they face the opposite direction -- as if in a mirror.
  * @param mirror Set true, the default value to mirror; false for normal rendering. 
  */
  public void MirrorZAxis(bool mirror = true) {
    mirror_z_axis_ = mirror;
  }

  /** The Leap Hand object. */
  public Hand GetLeapHand() { return hand_; }
  /** The Leap Finger object. */
  public Finger GetLeapFinger() { return finger_; }

  /** 
  * Implement this function to initialize this finger after it is created.
  * Typically, this function is called by the parent HandModel object.
  */
  public virtual void InitFinger() {

    UpdateFinger();
  }

  /** 
  * Implement this function to update this finger once per game loop.
  * Typically, this function is called by the parent HandModel object's
  * UpdateHand() function, which is called in the Unity Update() phase for
  * graphics hand models and in the FixedUpdate() phase for physics hand
  * models.
  */
  public abstract void UpdateFinger();

  /** Returns any additional movement the finger needs because of non-relative palm movement.*/
  public Vector3 GetOffset() {
    return offset_;
  }

  

  /** Returns the rotation quaternion of the given bone in relation to the controller.*/
  public Quaternion GetBoneRotation(int bone_type) {

    return Quaternion.Euler(handInfo_.RiggedHandAlignmentRotation) * Math.ConvertOrientation(handInfo_.Digits[(int)fingerType].poseStates[bone_type].Value.rotation);

  }
  
  /** Returns the length of the finger bone.*/
  public float GetBoneLength(int bone_type) {
    return finger_.Bone ((Bone.BoneType)(bone_type)).Length * UnityVectorExtension.INPUT_SCALE;
  }
  
  /** Returns the width of the finger bone.*/
  public float GetBoneWidth(int bone_type) {
    return finger_.Bone((Bone.BoneType)(bone_type)).Width * UnityVectorExtension.INPUT_SCALE;
  }
  

 
 
}
