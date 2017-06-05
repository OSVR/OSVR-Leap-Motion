using System.Runtime.InteropServices;
using OSVR.ClientKit;
using UnityEngine;
using Quaternion = UnityEngine.Quaternion;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class LeapImagingCamera : MonoBehaviour {

		public const int ImageWidth = 640;
		public const int ImageHeight = 240;

		public OsvrContextProvider ContextProvider;
		public bool IsLeft;

		private ImagingInterface vInterface;

		private GameObject vGameObj;
		private Texture2D vTex;

		private readonly byte[] vImgBytes;
		private readonly Color32[] vImgColors;
		private bool vIsChanged;


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public LeapImagingCamera() {
			vImgBytes = new byte[ImageWidth*ImageHeight];
			vImgColors = new Color32[ImageWidth*ImageHeight];
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Start() {
			vInterface = ImagingInterface.GetInterface(ContextProvider.Context,
				"/camera/"+(IsLeft ? "left" : "right"));
			vInterface.StateChanged += HandleChanged;

			vTex = new Texture2D(ImageWidth, ImageHeight);
			
			var mat = new Material(Shader.Find("Diffuse"));
			mat.mainTexture = vTex;

			vGameObj = GameObject.CreatePrimitive(PrimitiveType.Quad);
			vGameObj.transform.SetParent(gameObject.transform, false);
			vGameObj.transform.localRotation = Quaternion.FromToRotation(Vector3.down, Vector3.forward);
			vGameObj.transform.localScale = new Vector3(1, (float)ImageHeight/ImageWidth, 1)*0.06f;
			vGameObj.GetComponent<MeshRenderer>().material = mat;
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
			//ImagingState state = vInterface.GetState().Value; //TODO: "state" is not implemented

			if ( vIsChanged ) {
				vTex.SetPixels32(vImgColors);
				vTex.Apply();
				vIsChanged = false;
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		private void HandleChanged(object pSender, TimeValue pTime, int pSensor, ImagingState pReport) {
			if ( vIsChanged ) {
				return;
			}

			for ( int x = 0 ; x < ImageWidth ; x++ ) {
				for ( int y = 0 ; y < ImageHeight ; y++ ) {
					int byteI = x+y*ImageWidth;
					int colorI = (ImageWidth-x-1) + y*ImageWidth;
					byte b = Marshal.ReadByte(pReport.data, byteI);

					vImgColors[colorI] = new Color32(b, b, b, 255);
				}
			}

			vIsChanged = true;
		}

	}

}
