using UnityEngine;
using System.Collections;

namespace HandPhysicsExtenstions
{
    [RequireComponent(typeof(Rigidbody))]
    public class AttachedObject : MonoBehaviour
    {
        public ObjectDetached OnObjectDetached { get; set; }

        /// <summary>
        /// Returns controller which is holding this object
        /// </summary>
        public HandPhysicsController Controller { get; private set; }
        public FixedJoint Joint { get; private set; }

        public Rigidbody Rigidbody
        {
            get
            {
                if (_rigidbody == null)
                    _rigidbody = GetComponent<Rigidbody>();
                return _rigidbody;
            }
        }
        private Rigidbody _rigidbody;

        public void Attach(HandPhysicsController controller, ObjectDetached onObjectDetached)
        {
            Controller = controller;

            Joint = gameObject.AddComponent<FixedJoint>();
            Joint.connectedBody = Controller.Parts.Wrist.Rigidbody;
            Joint.breakForce = Controller.Fingers.ForceToDetachObject;
            
            OnObjectDetached = onObjectDetached;
        }
        
        public void Detach()
        {
            Destroy(Joint);
            Destroy(this);
        }

        void OnJointBreak(float breakForce)
        {
            if (OnObjectDetached != null)
                OnObjectDetached();
        }

        void OnDisable()
        {
            if (OnObjectDetached != null)
                OnObjectDetached();
        }
    }
}

