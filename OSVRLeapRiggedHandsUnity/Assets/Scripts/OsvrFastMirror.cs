using UnityEngine;

public class OsvrFastMirror : MonoBehaviour
{
    #region Serialized Fields
    [SerializeField]
    [Tooltip("Corrects the aspect ratio of the mirror and draws the fill texture behind it.")]
    private bool fixAspectRatio = true;
    [SerializeField]
    [Tooltip("The VR Eye to duplicate in the mirror. Leave empty to find the first VR Eye in the scene.")]
    private Camera eyeCamera;
    [SerializeField]
    [Tooltip("The texture to draw behind the mirror. Leave empty to use a plain black texture.")]
    private Texture2D fillTexture;
    #endregion

    #region Public Properties
    /// <summary>
    /// The camera attached to a VR Eye that we would like to mirror (usually a child.)
    /// </summary>
    public Camera EyeCamera
    {
        get { return eyeCamera; }
        set { eyeCamera = value; }
    }
    /// <summary>
    /// The texture to draw behind the mirror.
    /// </summary>
    public Texture2D FillTexture
    {
        get { return fillTexture; }
        set { fillTexture = value; }
    }
    #endregion

    #region Private Variables
    private Rect renderRect;
    private Rect fillRect;
    #endregion

    #region Magic Methods
    void Awake()
    {
        if (eyeCamera == null)
        {
            eyeCamera = FindObjectOfType<OSVR.Unity.VREye>().GetComponentInChildren<Camera>();
        }
        if (fillTexture == null)
        {
            fillTexture = new Texture2D(1, 1);
            fillTexture.SetPixel(1, 1, Color.black);
            fillTexture.wrapMode = TextureWrapMode.Repeat;
            fillTexture.Apply();
        }
    }

    private void OnGUI()
    {
        if (Event.current.type.Equals(EventType.Repaint))
        {
            RefreshRectangles();
            if (fixAspectRatio)
            {
                DrawFill();
            }
            DrawTexture();
        }
    }
    #endregion

    #region Private Methods
    private void DrawTexture()
    {
        Graphics.DrawTexture(renderRect, eyeCamera.targetTexture);
    }

    private void DrawFill()
    {
        Graphics.DrawTexture(fillRect, fillTexture);
    }

    private void RefreshRectangles()
    {
        // Get rendering width/height
        // Using Display.renderingWidth seems not to work properly outside of the editor
        // It appears to report the systemWidth or something weird instead. For now, use
        // Screen.width, which surprisingly works just fine in Extended Mode with a single
        // monitor + HMD. Might cause issues for users with multi-monitor + HMD?
        int rw = Screen.width;
        int rh = Screen.height;

        if (fixAspectRatio)
        {
            int w = eyeCamera.targetTexture.width;
            int h = eyeCamera.targetTexture.height;

            // When HDK 1.4 or 2.0 are in "portrait" mode, the Render Texture
            // is apparently rendered taller than it is wide, but is inexplicably NOT
            // rotated. This creates a "squished" effect that is simply fixed by
            // swapping width and height. Unknown if this would create a problem
            // for headsets that do not exhibit this behavior.
            if (h > w)
            {
                int swap = w;
                w = h;
                h = swap;
            }

            // Calculate the necessary scaling to fit the renderTarget on the
            // display while maintaining constant aspect ratio
            float minW = Mathf.Min(w, rw);
            float minH = Mathf.Min(h, rh);
            float scaleW = minW / (float)w;
            float scaleH = minH / (float)h;
            float scalar = Mathf.Min(scaleW, scaleH);

            w = Mathf.FloorToInt(w * scalar);
            h = Mathf.FloorToInt(h * scalar);
            int x = (rw - w) / 2;
            int y = (rh - h) / 2;
            renderRect = new Rect(x, y, w, h);
            fillRect = new Rect(0, 0, rw, rh);
        }
        else
        {
            renderRect = new Rect(0, 0, rw, rh);
        }
    }
    #endregion
}