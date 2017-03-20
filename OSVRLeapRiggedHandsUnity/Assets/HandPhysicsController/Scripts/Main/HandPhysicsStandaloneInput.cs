using UnityEngine;

namespace HandPhysicsExtenstions
{
    [RequireComponent(typeof(HandPhysicsController))]
    public class HandPhysicsStandaloneInput : MonoBehaviour
    {
        public bool LockCursor = true;
        public bool ControlsEnabled = true;

        [Header("Input Keys")]
        public string MoveForearmAxisX = "Mouse X";
        public string MoveForearmAxisY = "Mouse ScrollWheel";
        public string MoveForearmAxisZ = "Mouse Y";

        [Space]
        public string RotateForearmAxis = "Mouse X";
        public string RotateWristAxis = "Mouse Y";

        [Space]
        public KeyCode BendAllFingers = KeyCode.Mouse0;

        [Space]
        public KeyCode BendThumb = KeyCode.Space;
        public KeyCode BendIndex = KeyCode.F;
        public KeyCode BendMiddle = KeyCode.D;
        public KeyCode BendRing = KeyCode.S;
        public KeyCode BendPinky = KeyCode.A;

        [Space]
        public KeyCode HoldRotation = KeyCode.Mouse1;

        public HandPhysicsController Controller
        {
            get
            {
                if (_controller == null)
                    _controller = GetComponent<HandPhysicsController>();
                return _controller;
            }
        }
        private HandPhysicsController _controller;

        public void OnApplicationFocus(bool focus)
        {
            if (focus && LockCursor)
                Cursor.lockState = CursorLockMode.Locked;
        }

        void Update()
        {
            if (!ControlsEnabled)
                return;


            if (Input.GetKeyDown(BendAllFingers))
                Controller.StartBendFingers();
            else if (Input.GetKeyUp(BendAllFingers))
                Controller.StopBendFingers();


            if (Input.GetKeyDown(BendThumb))
                Controller.StartBendFinger(FingerType.Thumb);
            else if (Input.GetKeyUp(BendThumb))
                Controller.StopBendFinger(FingerType.Thumb);

            if (Input.GetKeyDown(BendIndex))
                Controller.StartBendFinger(FingerType.Index);
            else if (Input.GetKeyUp(BendIndex))
                Controller.StopBendFinger(FingerType.Index);

            if (Input.GetKeyDown(BendMiddle))
                Controller.StartBendFinger(FingerType.Middle);
            else if (Input.GetKeyUp(BendMiddle))
                Controller.StopBendFinger(FingerType.Middle);

            if (Input.GetKeyDown(BendRing))
                Controller.StartBendFinger(FingerType.Ring);
            else if (Input.GetKeyUp(BendRing))
                Controller.StopBendFinger(FingerType.Ring);

            if (Input.GetKeyDown(BendPinky))
                Controller.StartBendFinger(FingerType.Pinky);
            else if (Input.GetKeyUp(BendPinky))
                Controller.StopBendFinger(FingerType.Pinky);


            if (!Input.GetKey(HoldRotation))
                Controller.MoveForearm(new Vector3(Input.GetAxis(MoveForearmAxisX), Input.GetAxis(MoveForearmAxisY),
                    Input.GetAxis(MoveForearmAxisZ)));
            else
            {
                Controller.RotateWrist(Input.GetAxis(RotateWristAxis));
                Controller.RotateForearm(Input.GetAxis(RotateForearmAxis));
            }
        }
    }
}

