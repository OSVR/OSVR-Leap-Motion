using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace HandPhysicsExtenstions
{
    public class Finger : MonoBehaviour
    {
        public FingerPart[] Parts = new FingerPart[3];

        /// <summary>
        /// Is this finger currrently bending?
        /// </summary>
        public bool IsBending { get; private set; }

        /// <summary>
        /// Is currently attached object was being attached by this finger?
        /// </summary>
        public bool IsHoldingObject { get; set; }

        void Awake()
        {
            Init();
        }

        void Init()
        {
            for (int i = 0; i < Parts.Length; i++)
            {
                Parts[i].ParentFinger = this;
                if (i > 0)
                    Parts[i].PrevPart = Parts[i - 1];
                if (i < Parts.Length - 1)
                    Parts[i].NextPart = Parts[i + 1];
            }
        }

        public void StartBending()
        {
            if (IsBending)
                return;

            for (int i = 0; i < Parts.Length; i++)
                Parts[i].IsRotating = true;

            IsBending = true;
        }

        public void StopBending()
        {
            if (!IsBending)
                return;

            for (int i = 0; i < Parts.Length; i++)
                Parts[i].IsRotating = false;

            IsBending = false;
            IsHoldingObject = false;
        }
        
        /// <summary>
        /// Returns all rigidbodies which collides with this finger
        /// </summary>
        /// <returns></returns>
        public List<Rigidbody> GetAllTouchingObjects()
        {
            List<Rigidbody> allTouchingObjects = new List<Rigidbody>();
            for (int i = 0; i < Parts.Length; i++)
            {
                if (Parts[i].IsTouchingAnyObject)
                    allTouchingObjects.AddRange(Parts[i].TouchingObjects);
            }

            return allTouchingObjects;
        } 
    }
}

