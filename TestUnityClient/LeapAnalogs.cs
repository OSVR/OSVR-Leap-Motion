using OSVR.ClientKit;
using UnityEngine;
using AnalogInterface = OSVR.ClientKit.AnalogInterface;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class LeapAnalogs : MonoBehaviour {

		public struct HandInfo {
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
		}

		public OsvrContextProvider ContextProvider;

		public HandInfo LeftHand { get; private set; }
		public HandInfo RightHand { get; private set; }

		private AnalogInterface vHandLInterface;
		private AnalogInterface vHandRInterface;
		private AnalogInterface vTest;


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Start() {
			LeftHand = BuildHandInfo("/arms/left/");
			RightHand = BuildHandInfo("/arms/right/");
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
			LeftHand = UpdateHandInfo(LeftHand);
			RightHand = UpdateHandInfo(RightHand);
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		private HandInfo BuildHandInfo(string pPath) {
			ClientContext c = ContextProvider.Context;

			var info = new HandInfo();
			info.IsAvailableInterface = AnalogInterface.GetInterface(c, pPath+"available");
			info.ConfidenceInterface = AnalogInterface.GetInterface(c, pPath+"hand/confidence");
			info.GrabStrengthInterface = AnalogInterface.GetInterface(c, pPath+"hand/grabStrength");
			info.PinchStrengthInterface = AnalogInterface.GetInterface(c, pPath+"hand/pinchStrength");
			info.PalmWidthInterface = AnalogInterface.GetInterface(c, pPath+"hand/palmWidth");
			return info;
		}

		/*--------------------------------------------------------------------------------------------*/
		private static HandInfo UpdateHandInfo(HandInfo pInfo) {
			pInfo.IsAvailable = (pInfo.IsAvailableInterface.GetState().Value > 0);
			pInfo.Confidence = (float)pInfo.ConfidenceInterface.GetState().Value;
			pInfo.GrabStrength = (float)pInfo.GrabStrengthInterface.GetState().Value;
			pInfo.PinchStrength = (float)pInfo.PinchStrengthInterface.GetState().Value;
			pInfo.PalmWidth = (float)pInfo.PalmWidthInterface.GetState().Value;
			return pInfo;
		}

	}

}
