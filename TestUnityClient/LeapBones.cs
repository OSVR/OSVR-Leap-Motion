using OSVR.ClientKit;
using UnityEngine;
using Math = OSVR.Unity.Math;
using Quaternion = UnityEngine.Quaternion;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class LeapBones : MonoBehaviour {

		public const int TrackersPerHand = 23;
		//public const string RawTrackerPath = "/com_osvr_LeapMotion/Controller/tracker/";

		public OsvrContextProvider ContextProvider;
		public LeapAnalogs Analogs;
		public bool InitializePalmsOnly = true;

		private GameObject[] vGameObjects;
		private PoseInterface[] vInterfaces;


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Awake() {
			vGameObjects = new GameObject[TrackersPerHand*2];
			vInterfaces = new PoseInterface[vGameObjects.Length];

			for ( int i = 0 ; i < vGameObjects.Length ; i++ ) {
				var handGo = GameObject.CreatePrimitive(PrimitiveType.Cube);
				handGo.name = "Tracker"+i;
				handGo.transform.SetParent(gameObject.transform, false);
				handGo.transform.localScale = Vector3.one*0.01f;
				vGameObjects[i] = handGo;
			}
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Start() {
			int i = (InitializePalmsOnly ? 2 : 0);
			int inc = (InitializePalmsOnly ? TrackersPerHand : 1);
			int count = vGameObjects.Length;
			ClientContext cont = ContextProvider.Context;

			for (  ; i < count ; i += inc ) {
				string path = GetTrackerPath(i);
				vInterfaces[i] = PoseInterface.GetInterface(cont, path);
			}
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
			int count = vGameObjects.Length;

			for ( int i = 0 ; i < count ; i++ ) {
				GameObject handGo = vGameObjects[i];

				if ( InitializePalmsOnly && (i-2)%TrackersPerHand != 0 ) {
					handGo.SetActive(false);
					continue;
				}

				bool isLeft = (i < TrackersPerHand);
				LeapAnalogs.HandInfo handInfo = (isLeft ? Analogs.LeftHand : Analogs.RightHand);

				if ( !handInfo.IsAvailable ) {
					handGo.SetActive(false);
					continue;
				}

				InterfaceState<Pose3> poseState = vInterfaces[i].GetState();
				Vector3 pos = Math.ConvertPosition(poseState.Value.translation);
				Quaternion rot = Math.ConvertOrientation(poseState.Value.rotation);

				handGo.SetActive(true);
				handGo.transform.localPosition = pos;
				handGo.transform.localRotation = rot;
				handGo.transform.localScale = 
					Vector3.Lerp(Vector3.one, Vector3.one*0.5f, handInfo.GrabStrength)*0.01f;
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		private string GetTrackerPath(int pIndex) {
			int handI = pIndex%TrackersPerHand;
			string side = "/arms/"+(pIndex < TrackersPerHand ? "left" : "right")+"/";
			string elem = null;
			string bone = "";

			switch ( handI ) {
				case 0:
					return side+"elbow";

				case 1:
					return side+"wrist";

				case 2:
					elem = "palm";
					break;

				case 3:
				case 4:
				case 5:
				case 6:
					elem = "thumb";
					break;

				case 7:
				case 8:
				case 9:
				case 10:
					elem = "index";
					break;

				case 11:
				case 12:
				case 13:
				case 14:
					elem = "middle";
					break;

				case 15:
				case 16:
				case 17:
				case 18:
					elem = "ring";
					break;

				case 19:
				case 20:
				case 21:
				case 22:
					elem = "pinky";
					break;
			}

			if ( handI > 2 ) {
				switch ( (handI-3)%4 ) {
					case 0:
						bone = "metacarpal";
						break;
					case 1:
						bone = "proximal";
						break;
					case 2:
						bone = "intermediate";
						break;
					case 3:
						bone = "distal";
						break;
				}

				bone = "/"+bone;
			}

			return side+"hand/"+elem+bone;
		}

	}

}
