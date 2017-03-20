using UnityEngine;
using System.Collections;

namespace HandPhysicsExtenstions
{
    [RequireComponent(typeof(Camera))]
    public class HandPhysicsCamera : MonoBehaviour
    {
        public Transform CameraTarget;

        [Range(0f, 1f)]
        public float FollowTargetSpeed = 0.2f;
        public Vector3 PositionOffset = new Vector3(0, 8, 5);

        void FixedUpdate()
        {
            MoveCamera();
        }

        void MoveCamera()
        {
            if (CameraTarget == null)
                return;

            transform.position = Vector3.Lerp(
                transform.position, 
                CameraTarget.position + PositionOffset,
                FollowTargetSpeed);
        }

    }
}

