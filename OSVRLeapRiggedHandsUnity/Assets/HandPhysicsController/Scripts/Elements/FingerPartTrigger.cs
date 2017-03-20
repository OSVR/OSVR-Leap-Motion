using System.Collections.Generic;
using UnityEngine;

namespace HandPhysicsExtenstions
{
    [RequireComponent(typeof(Collider), typeof(Rigidbody))]
    public class FingerPartTrigger : MonoBehaviour
    {
        public FingerPart ParentFingerPart { get; internal set; }

        void OnTriggerEnter(Collider col)
        {
            if (col.GetComponent<FingerPartTrigger>() || col.GetComponent<HandPart>())
                return;

            if (ParentFingerPart.Controller.Fingers.AttachPickableOnly && !col.GetComponent<PickableObject>())
                return;

            Rigidbody colRigidbody = col.GetComponent<Rigidbody>();
            if (!colRigidbody)
                return;

            if (!colRigidbody.isKinematic && !ParentFingerPart.TouchingObjects.Contains(colRigidbody))
            {
                ParentFingerPart.TouchingObjects.Add(colRigidbody);
                ParentFingerPart.IsTouchingAnyObject = true;
            }
        }

        void OnTriggerExit(Collider col)
        {
            Rigidbody colRigidbody = col.GetComponent<Rigidbody>();
            if (!colRigidbody)
                return;

            if (ParentFingerPart.TouchingObjects.Contains(colRigidbody))
            {
                ParentFingerPart.TouchingObjects.Remove(colRigidbody);

                if (ParentFingerPart.TouchingObjects.Count == 0)
                    ParentFingerPart.IsTouchingAnyObject = false;
            }
        }

#if UNITY_EDITOR
        [ContextMenu("Configure")]
        private void Configure()
        {
            if (transform.parent == null)
                return;

            FingerPart fp = transform.parent.GetComponent<FingerPart>();
            if (fp == null)
                return;

            transform.localPosition = Vector3.zero;
            transform.localRotation = Quaternion.identity;
            GetComponent<Collider>().isTrigger = true;
            GetComponent<Rigidbody>().isKinematic = true;

            if (fp.Collider is CapsuleCollider && GetComponent<CapsuleCollider>())
            {
                var myCollider = GetComponent<CapsuleCollider>();
                var fpCollider = fp.GetComponent<CapsuleCollider>();

                myCollider.center = fpCollider.center;
                myCollider.direction = fpCollider.direction;
                myCollider.height = fpCollider.height;
                myCollider.radius = fpCollider.radius;
            }

            if (fp.Collider is BoxCollider && GetComponent<BoxCollider>())
            {
                var myCollider = GetComponent<BoxCollider>();
                var fpCollider = fp.GetComponent<BoxCollider>();

                myCollider.center = fpCollider.center;
                myCollider.size = fpCollider.size;
            }
        }
#endif
    }
}

