using UnityEngine;
using System.Collections.Generic;

namespace HandPhysicsExtenstions
{
    public class FingerPart : HandPart
    {
        #region Public Fields
        [Tooltip("Target rotation quaternion in local space")]
        public Quaternion TargetRotation = new Quaternion(0, 0, 0, 1);
        [Tooltip("Rotation process from start to target quaternion will be exposed by this curve")]
        public AnimationCurve RotationCurve = new AnimationCurve(new Keyframe(0, 0), new Keyframe(1, 1));

        [Space]
        public FingerPartTrigger Trigger;
        #endregion

        #region Public Properties
        /// <summary>
        /// Start local rotation defined in <see cref="Transform"/> component in editor
        /// </summary>
        public Quaternion StartRotation
        {
            get
            {
                if (Application.isPlaying)
                    return _startRotation;
                return transform.localRotation;
            }
            private set { _startRotation = value; }
        }
        private Quaternion _startRotation;

        /// <summary>
        /// Returns current rotation value clamped between 0 and 1
        /// </summary>
        public float RotationValue
        {
            get { return _rotationValue; }
            private set{ _rotationValue = Mathf.Clamp01(value); }
        }
        private float _rotationValue;

        /// <summary>
        /// Is this finger part currently rotating?
        /// </summary>
        public bool IsRotating { get; internal set; }
        /// <summary>
        /// Is rotation currently allowed?
        /// </summary>
        public bool IsRotationAllowed { get; internal set; }

        /// <summary>
        /// Returns true if <see cref="Trigger"/> collides with any other <see cref="Rigidbody"/>
        /// </summary>
        public bool IsTouchingAnyObject
        {
            get
            {
                for (int i = 0; i < TouchingObjects.Count; i++)
                    if (TouchingObjects[i] == null)
                        TouchingObjects.RemoveAt(i);
                if (TouchingObjects.Count == 0)
                    _isTouchingAnyObject = false;

                return _isTouchingAnyObject;
            }
            internal set
            {
                _isTouchingAnyObject = value;
                if (value && ParentFinger.IsBending)
                    Controller.OnObjectTouched();
            }
        }
        private bool _isTouchingAnyObject;

        /// <summary>
        /// Returns all objects which collides with <see cref="Trigger"/>
        /// </summary>
        public List<Rigidbody> TouchingObjects { get; internal set; }
        
        public Finger ParentFinger { get; internal set; }
        public FingerPart PrevPart { get; internal set; }
        public FingerPart NextPart { get; internal set; }
        #endregion


        void Start()
        {
            Init();
        }

        void Init()
        {
            TouchingObjects = new List<Rigidbody>();
            Trigger.ParentFingerPart = this;
            StartRotation = transform.localRotation;
        }

        void Update()
        {
            ControlPart();
        }

        void ControlPart()
        {
            if (IsTouchingAnyObject)
                DisallowRotationRecursively();

            if (IsRotating)
            {
                if (IsRotationAllowed)
                {
                    if (NextPart == null)
                    {
                        if (PrevPart.IsRotationAllowed)
                            IncreaseRotationValue(GetRotationAmount());
                    }
                    else IncreaseRotationValue(GetRotationAmount());
                }

            }
            else
            {
                DecreaseRotationValue(GetRotationAmount());
                IsRotationAllowed = true;
            }

            transform.localRotation = Quaternion.Lerp(StartRotation, TargetRotation, RotationCurve.Evaluate(RotationValue));
        }

        void IncreaseRotationValue(float amount)
        {
            switch (Controller.Fingers.RotationMode)
            {
                case FingerRotationMode.Linear:
                    RotationValue += amount;
                    break;
                case FingerRotationMode.Smooth:
                    RotationValue = Mathf.SmoothStep(RotationValue, 1, amount);
                    break;
            }
        }

        void DecreaseRotationValue(float amount)
        {
            switch (Controller.Fingers.RotationMode)
            {
                case FingerRotationMode.Linear:
                    RotationValue -= amount;
                    break;
                case FingerRotationMode.Smooth:
                    RotationValue = Mathf.SmoothStep(RotationValue, 0, amount);
                    break;
            }
        }
        
        float GetRotationAmount()
        {
            return Time.deltaTime * Controller.Fingers.BendSpeed;
        }


        internal void DisallowRotationRecursively()
        {
            IsRotationAllowed = false;
            if (PrevPart != null)
                PrevPart.DisallowRotationRecursively();
        }
    }
}

