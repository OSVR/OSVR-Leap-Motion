using System.Runtime.InteropServices;
using OSVR.ClientKit;
using UnityEngine;
using Quaternion = UnityEngine.Quaternion;

namespace LeapOsvrTest {

	/*================================================================================================*/
	public class LeapImagingDistortion : MonoBehaviour {

		public const int ImageWidth = 64;
		public const int ImageHeight = 64;
		public const int ImageChannels = 3;

		public OsvrContextProvider ContextProvider;
		public bool IsLeft;

		private ImagingInterface vInterface;

		private GameObject vGameObj;
		private Texture2D vTex;

		private readonly float[] vImgFloats;
		private readonly Color32[] vImgColors;
		private bool vIsChanged;


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public LeapImagingDistortion() {
			vImgFloats = new float[ImageWidth*ImageHeight*ImageChannels];
			vImgColors = new Color32[ImageWidth*ImageHeight];
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		/*--------------------------------------------------------------------------------------------*/
		public void Start() {
			vInterface = ImagingInterface.GetInterface(ContextProvider.Context,
				"/camera/"+(IsLeft ? "left" : "right")+"/distortion");
			vInterface.StateChanged += HandleChanged;

			vTex = new Texture2D(ImageWidth, ImageHeight);
			
			var mat = new Material(Shader.Find("Diffuse"));
			mat.mainTexture = vTex;

			vGameObj = GameObject.CreatePrimitive(PrimitiveType.Quad);
			vGameObj.transform.SetParent(gameObject.transform, false);
			vGameObj.transform.localRotation = Quaternion.FromToRotation(Vector3.down, Vector3.forward);
			vGameObj.transform.localScale = Vector3.one*0.03f;
			vGameObj.GetComponent<MeshRenderer>().material = mat;
		}

		/*--------------------------------------------------------------------------------------------*/
		public void Update() {
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

			Marshal.Copy(pReport.data, vImgFloats, 0, vImgFloats.Length);

			for ( int x = 0 ; x < ImageWidth ; x++ ) {
				for ( int y = 0 ; y < ImageHeight ; y++ ) {
					int floatI = (x + y*ImageWidth)*ImageChannels;
					int colorI = (ImageWidth-x-1) + y*ImageWidth;

					vImgColors[colorI] = new Color(
						vImgFloats[floatI+2],
						vImgFloats[floatI+1],
						vImgFloats[floatI]
					);
				}
			}

			vIsChanged = true;
		}

	}

}
