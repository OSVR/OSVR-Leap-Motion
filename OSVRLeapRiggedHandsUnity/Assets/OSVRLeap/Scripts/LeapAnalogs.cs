using OSVR.ClientKit;
using UnityEngine;
using Leap;
using AnalogInterface = OSVR.ClientKit.AnalogInterface;

namespace LeapOsvrTest {

  /*================================================================================================*/
  public class LeapAnalogs : MonoBehaviour {

    public struct DigitInfo {
      public InterfaceState<Pose3>[] poseStates;
      public PoseInterface[] poseInterfaces;
    }

    public Vector3 RiggedHandAlignment = new Vector3 (270, 180, 0);

    public struct HandInfo {
      public Vector3 RiggedHandAlignmentRotation;
      public bool IsLeft;
      public DigitInfo[] Digits;

      public bool IsAvailable;
      public float Confidence;
      public float GrabStrength;
      public float PinchStrength;
      public float PalmWidth;

      public AnalogInterface IsAvailableInterface;
      public AnalogInterface ConfidenceInterface;
      public AnalogInterface GrabStrengthInterface;
      public AnalogInterface PinchStrengthInterface;
      public AnalogInterface PalmWidthInterface;

      public InterfaceState<Pose3> PalmState;
      public PoseInterface PalmInterface;
    }

    public OsvrContextProvider ContextProvider;

    public HandInfo LeftHand { get; private set; }
    public HandInfo RightHand { get; private set; }

    private AnalogInterface vDeviceConnectionInterface;
    private AnalogInterface vServiceConnectionInterface;
    private AnalogInterface vHandLInterface;
    private AnalogInterface vHandRInterface;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    /*--------------------------------------------------------------------------------------------*/
    public void Start() {
      ClientContext c = ContextProvider.Context;

      vDeviceConnectionInterface = AnalogInterface.GetInterface(c, "/connection/device");
      vServiceConnectionInterface = AnalogInterface.GetInterface(c, "/connection/service");

      LeftHand = BuildHandInfo("/arms/left/", true);
      RightHand = BuildHandInfo("/arms/right/", false);
    }

    /*--------------------------------------------------------------------------------------------*/
    public void Update() {
      LeftHand = UpdateHandInfo(LeftHand);
      RightHand = UpdateHandInfo(RightHand);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    /*--------------------------------------------------------------------------------------------*/
    private HandInfo BuildHandInfo(string pPath, bool isLeft) {
      ClientContext c = ContextProvider.Context;
      var info = new HandInfo();
      info.IsLeft = isLeft;
      info.RiggedHandAlignmentRotation = RiggedHandAlignment;
      info.IsAvailableInterface = AnalogInterface.GetInterface(c, pPath+"available");
      info.ConfidenceInterface = AnalogInterface.GetInterface(c, pPath+"hand/confidence"); 
      info.GrabStrengthInterface = AnalogInterface.GetInterface(c, pPath+"hand/grabStrength");
      info.PinchStrengthInterface = AnalogInterface.GetInterface(c, pPath+"hand/pinchStrength");
      info.PalmWidthInterface = AnalogInterface.GetInterface(c, pPath+"hand/palmWidth");
      
      info.Digits = new DigitInfo[5];
      for (int i = 0; i < info.Digits.Length; ++i) {
        info.Digits[i].poseStates = new InterfaceState<Pose3>[4];
        info.Digits[i].poseInterfaces = new PoseInterface[4];
      }

      info.PalmInterface = PoseInterface.GetInterface(c, pPath + "hand/palm");
      info.Digits[0].poseInterfaces[0] = PoseInterface.GetInterface(c, pPath + "hand/thumb/metacarpal");
      info.Digits[0].poseInterfaces[1] = PoseInterface.GetInterface(c, pPath + "hand/thumb/proximal");
      info.Digits[0].poseInterfaces[2] = PoseInterface.GetInterface(c, pPath + "hand/thumb/intermediate");
      info.Digits[0].poseInterfaces[3] = PoseInterface.GetInterface(c, pPath + "hand/thumb/distal");
      info.Digits[1].poseInterfaces[0] = PoseInterface.GetInterface(c, pPath + "hand/index/metacarpal");
      info.Digits[1].poseInterfaces[1] = PoseInterface.GetInterface(c, pPath + "hand/index/proximal");
      info.Digits[1].poseInterfaces[2] = PoseInterface.GetInterface(c, pPath + "hand/index/intermediate");
      info.Digits[1].poseInterfaces[3] = PoseInterface.GetInterface(c, pPath + "hand/index/distal");
      info.Digits[2].poseInterfaces[0] = PoseInterface.GetInterface(c, pPath + "hand/middle/metacarpal");
      info.Digits[2].poseInterfaces[1] = PoseInterface.GetInterface(c, pPath + "hand/middle/proximal");
      info.Digits[2].poseInterfaces[2] = PoseInterface.GetInterface(c, pPath + "hand/middle/intermediate");
      info.Digits[2].poseInterfaces[3] = PoseInterface.GetInterface(c, pPath + "hand/middle/distal");
      info.Digits[3].poseInterfaces[0] = PoseInterface.GetInterface(c, pPath + "hand/ring/metacarpal");
      info.Digits[3].poseInterfaces[1] = PoseInterface.GetInterface(c, pPath + "hand/ring/proximal");
      info.Digits[3].poseInterfaces[2] = PoseInterface.GetInterface(c, pPath + "hand/ring/intermediate");
      info.Digits[3].poseInterfaces[3] = PoseInterface.GetInterface(c, pPath + "hand/ring/distal");
      info.Digits[4].poseInterfaces[0] = PoseInterface.GetInterface(c, pPath + "hand/pinky/metacarpal");
      info.Digits[4].poseInterfaces[1] = PoseInterface.GetInterface(c, pPath + "hand/pinky/proximal");
      info.Digits[4].poseInterfaces[2] = PoseInterface.GetInterface(c, pPath + "hand/pinky/intermediate");
      info.Digits[4].poseInterfaces[3] = PoseInterface.GetInterface(c, pPath + "hand/pinky/distal");
      return info;
    }

    /*--------------------------------------------------------------------------------------------*/
    private static HandInfo UpdateHandInfo(HandInfo pInfo) {
      pInfo.IsAvailable = (pInfo.IsAvailableInterface.GetState().Value > 0);
      pInfo.Confidence = (float)pInfo.ConfidenceInterface.GetState().Value;
      pInfo.GrabStrength = (float)pInfo.GrabStrengthInterface.GetState().Value;
      pInfo.PinchStrength = (float)pInfo.PinchStrengthInterface.GetState().Value;
      pInfo.PalmWidth = (float)pInfo.PalmWidthInterface.GetState().Value;

      pInfo.PalmState = (InterfaceState<Pose3>)pInfo.PalmInterface.GetState();
      pInfo.Digits[0].poseStates[0] = (InterfaceState<Pose3>)pInfo.Digits[0].poseInterfaces[0].GetState();
      pInfo.Digits[0].poseStates[1] = (InterfaceState<Pose3>)pInfo.Digits[0].poseInterfaces[1].GetState();
      pInfo.Digits[0].poseStates[2] = (InterfaceState<Pose3>)pInfo.Digits[0].poseInterfaces[2].GetState();
      pInfo.Digits[0].poseStates[3] = (InterfaceState<Pose3>)pInfo.Digits[0].poseInterfaces[3].GetState();
      pInfo.Digits[1].poseStates[0] = (InterfaceState<Pose3>)pInfo.Digits[1].poseInterfaces[0].GetState();
      pInfo.Digits[1].poseStates[1] = (InterfaceState<Pose3>)pInfo.Digits[1].poseInterfaces[1].GetState();
      pInfo.Digits[1].poseStates[2] = (InterfaceState<Pose3>)pInfo.Digits[1].poseInterfaces[2].GetState();
      pInfo.Digits[1].poseStates[3] = (InterfaceState<Pose3>)pInfo.Digits[1].poseInterfaces[3].GetState();
      pInfo.Digits[2].poseStates[0] = (InterfaceState<Pose3>)pInfo.Digits[2].poseInterfaces[0].GetState();
      pInfo.Digits[2].poseStates[1] = (InterfaceState<Pose3>)pInfo.Digits[2].poseInterfaces[1].GetState();
      pInfo.Digits[2].poseStates[2] = (InterfaceState<Pose3>)pInfo.Digits[2].poseInterfaces[2].GetState();
      pInfo.Digits[2].poseStates[3] = (InterfaceState<Pose3>)pInfo.Digits[2].poseInterfaces[3].GetState();
      pInfo.Digits[3].poseStates[0] = (InterfaceState<Pose3>)pInfo.Digits[3].poseInterfaces[0].GetState();
      pInfo.Digits[3].poseStates[1] = (InterfaceState<Pose3>)pInfo.Digits[3].poseInterfaces[1].GetState();
      pInfo.Digits[3].poseStates[2] = (InterfaceState<Pose3>)pInfo.Digits[3].poseInterfaces[2].GetState();
      pInfo.Digits[3].poseStates[3] = (InterfaceState<Pose3>)pInfo.Digits[3].poseInterfaces[3].GetState();
      pInfo.Digits[4].poseStates[0] = (InterfaceState<Pose3>)pInfo.Digits[4].poseInterfaces[0].GetState();
      pInfo.Digits[4].poseStates[1] = (InterfaceState<Pose3>)pInfo.Digits[4].poseInterfaces[1].GetState();
      pInfo.Digits[4].poseStates[2] = (InterfaceState<Pose3>)pInfo.Digits[4].poseInterfaces[2].GetState();
      pInfo.Digits[4].poseStates[3] = (InterfaceState<Pose3>)pInfo.Digits[4].poseInterfaces[3].GetState();

      return pInfo;
    }

  }

}
