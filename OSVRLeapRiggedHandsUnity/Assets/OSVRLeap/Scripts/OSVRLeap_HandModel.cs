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
* The base class for all hand models, both graphics and physics.
* 
* This class serves as the interface between the HandController object
* and the concrete hand object containing the graphics and physics of a hand.
*
* Subclasses of HandModel must implement InitHand() and UpdateHand(). The UpdateHand()
* function is called in the Unity Update() phase for graphics HandModel instances;
* and in the Unity FixedUpdate() phase for physics objects. InitHand() is called once,
* when the hand is created and is followed by a call to UpdateHand().
*/
public abstract class OSVRLeap_HandModel : MonoBehaviour {

  /** The number of fingers on a hand.*/
  public const int NUM_FINGERS = 5;

  /** The model width of the hand in meters. This value is used with the measured value 
  * of the user's hand to scale the model proportionally.
  */
  public float handModelPalmWidth = 0.085f;
  /** The array of finger objects for this hand. The array is ordered from thumb (element 0) to pinky (element 4).*/
  public OSVRLeap_FingerModel[] fingers = new OSVRLeap_FingerModel[NUM_FINGERS];

  // Unity references
  /** Transform object for the palm object of this hand. */
  public Transform palm;
  /** Transform object for the forearm object of this hand. */
  public Transform forearm;
  /** Transform object for the wrist joint of this hand. */
  public Transform wristJoint;
  /** Transform object for the elbow joint of this hand. */
  public Transform elbowJoint;
  
  //OSVR References
  public OsvrContextProvider ContextProvider;
  public LeapAnalogs Analogs;
  public LeapAnalogs.HandInfo handInfo_;
  public bool IsLeft;

  private bool isVisible_;
  public bool IsVisible {
    get { return isVisible_; }
    set {
      if (value == true){
        GetComponentInChildren<Renderer>().enabled = true;
      }
      if (value == false) {
        GetComponentInChildren<Renderer>().enabled = false;
      }
      
      isVisible_ = value; }
  }

  /** Whether the parent HandController instance has been set to mirror across the z axis.*/
  protected bool mirror_z_axis_ = false;

  void Start() {
  }
  void Update() {
    handInfo_ = (IsLeft ? Analogs.LeftHand : Analogs.RightHand);
    if (handInfo_.Confidence > .5) {
      IsVisible = true;
    }
    else { 
      IsVisible = false; 
    }
    UpdateHand();
  }

  /** 
  * Calculates the offset between the wrist position and the controller based
  * on the HandController.handMovementScale property and the Leap hand wrist position.
  */
  public Vector3 GetHandOffset() {
      return Vector3.zero;

  }

  /** Calculates the position of the palm in global coordinates.
  * @returns A Vector3 containing the Unity coordinates of the palm position.
  */
  public Vector3 GetPalmPosition() {
    return Quaternion.Euler(handInfo_.RiggedHandAlignmentRotation) * Math.ConvertPosition(handInfo_.PalmState.Value.translation) + GetHandOffset(); ;

  }

  /** Calculates the rotation of the hand in global coordinates.
  * @returns A Quaternion representing the rotation of the hand. 
  */
  public Quaternion GetPalmRotation() {

    return Quaternion.Euler(handInfo_.RiggedHandAlignmentRotation) * Math.ConvertOrientation(handInfo_.PalmState.Value.rotation);

  }

  /** Calculates the direction vector of the hand in global coordinates.
  * @returns A Vector3 representing the direction of the hand.
  */
 
  //Assign a OSVR-Leap Analogs Hand
  public void SetOsvrLeapHandInfo(LeapAnalogs.HandInfo handInfo) {

    handInfo_ = handInfo;
    //Debug.Log("HandModel.SetOsvrLeapHandInfo()" + Math.ConvertOrientation(handInfo_.Digits[0].poseStates[0].Value.rotation));

    for (int i = 0; i < fingers.Length; ++i) {
      if (fingers[i] != null) {

        fingers[i].SetOsvrLeapHandInfo(handInfo);
        fingers[i].SetOffset(GetHandOffset());
      }
    }
  }

 
  /** 
  * Sets the mirror z-axis flag for this Hand Model and its fingers.
  * Mirroring the z axis reverses the hand so that they face the opposite direction -- as if in a mirror.
  * @param mirror Set true, the default value to mirror; false for normal rendering. 
  */
  public void MirrorZAxis(bool mirror = true) {
    mirror_z_axis_ = mirror;
    for (int i = 0; i < fingers.Length; ++i) {
      if (fingers[i] != null)
        fingers[i].MirrorZAxis(mirror);
    }
  }

  /** Whether this hand is currently mirrored.*/
  public bool IsMirrored() {
    return mirror_z_axis_;
  }



  /** 
  * Implement this function to initialise this hand after it is created.
  * This function is called by the HandController during the Unity Update() phase when a new hand is detected
  * by the Leap Motion device.
  */
  public virtual void InitHand() {
    for (int f = 0; f < fingers.Length; ++f) {
      if (fingers[f] != null) {
        fingers[f].fingerType = (Finger.FingerType)f;
        fingers[f].InitFinger();
      }
    }

    UpdateHand ();
  }



  /** 
  * Implement this function to update this hand once every game loop.
  * For HandModel instances assigned to the HandController graphics hand list, the HandController calls this 
  * function during the Unity Update() phase. For HandModel instances in the physics hand list, the HandController
  * calls this function in the FixedUpdate() phase.
  */
  public abstract void UpdateHand();
}
