using OSVR.ClientKit;
using OSVR.Unity;
using UnityEngine;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class OsvrContextProvider : MonoBehaviour {

		public ClientContext Context { get; set; }


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Awake() {
			DLLSearchPathFixer.fix();
			Context = new ClientContext("com.osvr.LeapMotion.Test");
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
			Context.update();
		}

	}

}
