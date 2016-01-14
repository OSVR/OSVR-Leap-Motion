using OSVR.ClientKit;
using UnityEngine;
using Math = OSVR.Unity.Math;
using Quaternion = UnityEngine.Quaternion;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class VrHead : MonoBehaviour {

		public OsvrContextProvider ContextProvider;

		private OrientationInterface vInterface;


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Start() {
			vInterface = OrientationInterface.GetInterface(ContextProvider.Context, "/me/head");
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
			Quaternion rot = Math.ConvertOrientation(vInterface.GetState().Value);
			gameObject.transform.localRotation = rot;
		}

	}

}
