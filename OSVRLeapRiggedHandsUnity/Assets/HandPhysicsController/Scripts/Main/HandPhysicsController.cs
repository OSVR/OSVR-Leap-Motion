//Hand Physics Controller 
//v1.1

using System;
using System.Collections.Generic;
using UnityEngine;
using HandPhysicsExtenstions;

#if UNITY_EDITOR
using UnityEditor;
#endif

[RequireComponent(typeof(Rigidbody))]
public class HandPhysicsController : MonoBehaviour
{
    /// <summary>
    /// Called when one of any finger part triggers collides with any other <see cref="Rigidbody"/>
    /// </summary>
    public ObjectTouched OnObjectTouched = () => {};
    /// <summary>
    /// Called when <see cref="Rigidbody"/> has been attached to hand
    /// </summary>
    public ObjectAttached OnObjectAttached = rigidbody => { };
    /// <summary>
    /// Called when attached object was dropped
    /// </summary>
    public ObjectDetached OnObjectDetached = () => { };

    #region Public Fields
    public ForearmConfig Forearm;
    public WristConfig Wrist;
    public FingersConfig Fingers;

    public HandParts Parts;

    #if UNITY_EDITOR
    [Header("Editor")]
    public bool DrawGizmos = true;
    #endif
    #endregion

    #region Public Properties

    /// <summary>
    /// Is forearm currently moving in specified direction?
    /// </summary>
    public bool IsForearmMoving { get; private set; }
    /// <summary>
    /// Is forearm currently rotating around its base joint axis?
    /// </summary>
    public bool IsForearmRotating { get; private set; }
    /// <summary>
    /// Is wrist currently rotating around its base joint axis?
    /// </summary>
    public bool IsWristRotating { get; private set; }

    /// <summary>
    /// Returns currently attached <see cref="Rigidbody"/>. Otherwise returns null 
    /// </summary>
    public AttachedObject AttachedObject { get; private set; }

    #endregion

    #region Private Fields
    private Vector3 _lastForearmPosition;
    private Vector3 _curForearmDirection;
    private float _curForearmTargetRotation;
    private float _curWristTargetRotation;

    private Vector3 _minForearmPos = Vector3.zero;
    private Vector3 _maxForearmPos = Vector3.zero;
    #endregion


    void Awake()
    {
        Init();
    }

    void Init()
    {
        Parts.SetController(this);
        Parts.DisableAllCollisions();
        UpdatePositionLimits();

        Parts.Forearm.Joint.slerpDrive = GetJointDrive(
            Parts.Forearm.Joint.slerpDrive,
            Mathf.Lerp(0, 10000*Parts.Forearm.Rigidbody.mass, Forearm.RotationHardness));
        Parts.Wrist.Joint.slerpDrive = GetJointDrive(
            Parts.Wrist.Joint.slerpDrive,
            Mathf.Lerp(0, 10000*Parts.Wrist.Rigidbody.mass, Wrist.RotationHardness));
    }

    void OnEnable()
    {
        OnObjectTouched += TryAttachObject;
    }

    void OnDisable()
    {
        OnObjectTouched -= TryAttachObject;
    }

    void Start()
    {
        //Dummy
    }

    void Update()
    {
        ControlForearm();
        ControlWrist();
        TryFixForearmAxes();
        ApplyPositionLimits();
    }

    void ControlForearm()
    {
        if (IsForearmMoving)
        {
            Parts.Forearm.Rigidbody.velocity = Vector3.Lerp(
                Parts.Forearm.Rigidbody.velocity,
                GetForearmTargetVelocity(_curForearmDirection),
                Forearm.MovementHardness);

            Vector3 forearmPos = Parts.Forearm.transform.position;
            if (!Mathf.Approximately(_curForearmDirection.x, 0))
                _lastForearmPosition = new Vector3(forearmPos.x, _lastForearmPosition.y, _lastForearmPosition.z);
            if (!Mathf.Approximately(_curForearmDirection.y, 0))
                _lastForearmPosition = new Vector3(_lastForearmPosition.x, forearmPos.y, _lastForearmPosition.z);
            if (!Mathf.Approximately(_curForearmDirection.z, 0))
                _lastForearmPosition = new Vector3(_lastForearmPosition.x, _lastForearmPosition.y, forearmPos.z);

            IsForearmMoving = false;
        }
        else
        {
            Parts.Forearm.Rigidbody.velocity = Vector3.Lerp(
                Parts.Forearm.Rigidbody.velocity,
                Vector3.zero,
                Forearm.MovementHardness);
        }

        if (IsForearmRotating)
        {
            Parts.Forearm.Joint.targetRotation =
                Quaternion.Euler(_curForearmTargetRotation, 0, 0);
            
            IsForearmRotating = false;
        }
    }

    void ControlWrist()
    {
        if (IsWristRotating)
        {
            Parts.Wrist.Joint.targetRotation =
                Quaternion.Euler(_curWristTargetRotation, 0, 0);

            IsWristRotating = false;
            return;
        }
        
        if (!Wrist.HoldRotation)
        {
            Parts.Wrist.Joint.targetRotation = Quaternion.Lerp(Parts.Wrist.Joint.targetRotation, Quaternion.identity, Wrist.RotationHardness);
            _curWristTargetRotation = Parts.Wrist.Joint.targetRotation.eulerAngles.x;
        }
    }

    void TryAttachObject()
    {
        if (AttachedObject != null || !Parts.Fingers[0].IsBending)
            return;
        
        List<Rigidbody> thumbTouches = Parts.Fingers[0].GetAllTouchingObjects();
        for (int i = 0; i < thumbTouches.Count; i++)
        {
            for (int j = 1; j < Parts.Fingers.Length; j++)
            {
                List<Rigidbody> touches = Parts.Fingers[j].GetAllTouchingObjects();
                for (int k = 0; k < touches.Count; k++)
                {
                    if (thumbTouches.Contains(touches[k]) && Parts.Fingers[j].IsBending)
                    {
                        AttachObject(touches[k], 0, (FingerType) j);
                        return;
                    }
                }
            }
        }
    }

    void AttachObject(Rigidbody objectToAttach, FingerType holdingFinger1, FingerType holdingFinger2)
    {
        _lastForearmPosition = Parts.Forearm.transform.position;

        Parts.Fingers[(int) holdingFinger1].IsHoldingObject = Parts.Fingers[(int)holdingFinger2].IsHoldingObject = true;
        AttachedObject = objectToAttach.gameObject.AddComponent<AttachedObject>();
        AttachedObject.Attach(this, DetachObject);
        Parts.IgnoreCollisions(objectToAttach, true);

        OnObjectAttached(objectToAttach);
    }

    void DetachObject()
    {
        if (Parts.CheckIfNull())
            return;

        for (int i = 0; i < Parts.Fingers.Length; i++)
        {
            Parts.Fingers[i].IsHoldingObject = false;
        }

        if (AttachedObject == null)
            return;

        if (AttachedObject.Rigidbody != null)
            Parts.IgnoreCollisions(AttachedObject.Rigidbody, false);
        AttachedObject.Detach();
        AttachedObject = null;

        OnObjectDetached();
    }

    void TryFixForearmAxes()
    {
        if (AttachedObject == null)
            return;

        Vector3 forearmPos = Parts.Forearm.transform.position;
        float hardness = Forearm.FixAxis.Hardness;

        if (Forearm.FixAxis.X && Mathf.Approximately(_curForearmDirection.x, 0))
            Parts.Forearm.Rigidbody.MovePosition(
                forearmPos = Vector3.Lerp(
                    forearmPos,
                    new Vector3(_lastForearmPosition.x, forearmPos.y, forearmPos.z),
                    hardness));

        if (Forearm.FixAxis.Y && Mathf.Approximately(_curForearmDirection.y, 0))
            Parts.Forearm.Rigidbody.MovePosition(
                forearmPos = Vector3.Lerp(
                    forearmPos,
                    new Vector3(forearmPos.x, _lastForearmPosition.y, forearmPos.z),
                    hardness));


        if (Forearm.FixAxis.Z && Mathf.Approximately(_curForearmDirection.z, 0))
            Parts.Forearm.Rigidbody.MovePosition(
                Vector3.Lerp(
                    forearmPos,
                    new Vector3(forearmPos.x, forearmPos.y, _lastForearmPosition.z),
                    hardness));
    }

    void ApplyPositionLimits()
    {
        if (!Forearm.PositionLimits.UseLimits)
            return;

        Rigidbody forearm = Parts.Forearm.Rigidbody;
        Vector3 forearmPos = forearm.transform.position;

        if (forearm.transform.position.x > _maxForearmPos.x)
            forearm.transform.position = forearmPos = new Vector3(_maxForearmPos.x, forearmPos.y, forearmPos.z);
        if (forearm.transform.position.x < _minForearmPos.x)
            forearm.transform.position = forearmPos = new Vector3(_minForearmPos.x, forearmPos.y, forearmPos.z);

        if (forearm.transform.position.y > _maxForearmPos.y)
            forearm.transform.position = forearmPos = new Vector3(forearmPos.x, _maxForearmPos.y, forearmPos.z);
        if (forearm.transform.position.y < _minForearmPos.y)
            forearm.transform.position = forearmPos = new Vector3(forearmPos.x, _maxForearmPos.y, forearmPos.z);

        if (forearm.transform.position.z > _maxForearmPos.z)
            forearm.transform.position = forearmPos = new Vector3(forearmPos.x, forearmPos.y, _maxForearmPos.z);
        if (forearm.transform.position.z < _minForearmPos.z)
            forearm.transform.position = new Vector3(forearmPos.x, forearmPos.y, _minForearmPos.z);
    }

    JointDrive GetJointDrive(JointDrive jointDrive, float spring)
    {
        jointDrive.positionSpring = spring;
        return jointDrive;
    }

    Vector3 GetForearmTargetVelocity(Vector3 direction)
    {
        if (Forearm.MovementRelativity == Space.Self)
            return Parts.Forearm.transform.TransformDirection(new Vector3(
                direction.x * Forearm.MovementSpeed.x,
                direction.y * Forearm.MovementSpeed.y,
                direction.z * Forearm.MovementSpeed.z));

        return new Vector3(
                direction.x * Forearm.MovementSpeed.x,
                direction.y * Forearm.MovementSpeed.y,
                direction.z * Forearm.MovementSpeed.z);
    }


    /// <summary>
    /// Updates current position limits represented in <see cref="HandPhysicsExtenstions.ForearmConfig.PositionLimits"/>
    /// </summary>
    public void UpdatePositionLimits()
    {
        var limits = Forearm.PositionLimits;

        Vector3 boundsHalfSize = limits.Bounds * 0.5f;
        _minForearmPos = limits.BoundsOffset - boundsHalfSize;
        _maxForearmPos = limits.BoundsOffset + boundsHalfSize;

        if (limits.Relativity == Space.Self)
        {
            _minForearmPos += transform.position;
            _maxForearmPos += transform.position;
        }
    }

    /// <summary>
    /// Applies velocity to forearm at <see cref="direction"/> based on <see cref="HandPhysicsExtenstions.ForearmConfig.MovementSpeed"/> and <see cref="HandPhysicsExtenstions.ForearmConfig.MovementHardness"/>
    /// </summary>
    /// <param name="direction"></param>
    public void MoveForearm(Vector3 direction)
    {
        if (Mathf.Approximately(direction.magnitude, 0))
        {
            _curForearmDirection = Vector3.zero;
            return;
        }

        _curForearmDirection = direction;
        IsForearmMoving = true;
    }

    /// <summary>
    /// Rotates forearm around base joint axis by <see cref="value"/> between min and max joint limits based on <see cref="HandPhysicsExtenstions.ForearmConfig.RotationSpeed"/> and <see cref="HandPhysicsExtenstions.ForearmConfig.RotationHardness"/>
    /// </summary>
    /// <param name="value"></param>
    public void RotateForearm(float value)
    {
        _curForearmTargetRotation += Mathf.Clamp(value, -1, 1) * Forearm.RotationSpeed;
        _curForearmTargetRotation = Mathf.Clamp(
            _curForearmTargetRotation,
            Parts.Forearm.Joint.lowAngularXLimit.limit, 
            Parts.Forearm.Joint.highAngularXLimit.limit);

        IsForearmRotating = true;
    }

    /// <summary>
    /// Rotates wrist around base joint axis by <see cref="value"/> between min and max joint limits based on <see cref="HandPhysicsExtenstions.WristConfig.RotationSpeed"/> and <see cref="HandPhysicsExtenstions.WristConfig.RotationHardness"/>
    /// </summary>
    /// <param name="value"></param>
    public void RotateWrist(float value)
    {
        _curWristTargetRotation += Mathf.Clamp(value, -1, 1) * Wrist.RotationSpeed;
        _curWristTargetRotation = Mathf.Clamp(
            _curWristTargetRotation,
            Parts.Wrist.Joint.lowAngularXLimit.limit,
            Parts.Wrist.Joint.highAngularXLimit.limit);
        
        IsWristRotating = true;
    }

    /// <summary>
    /// Starts bending all fingers based on <see cref="HandPhysicsExtenstions.FingersConfig.BendSpeed"/>
    /// </summary>
    public void StartBendFingers()
    {
        for (int i = 0; i < Parts.Fingers.Length; i++)
            Parts.Fingers[i].StartBending();
    }

    /// <summary>
    /// Stops bending all fingers
    /// </summary>
    public void StopBendFingers()
    {
        for (int i = 0; i < Parts.Fingers.Length; i++)
            Parts.Fingers[i].StopBending();

        DetachObject();
    }

    /// <summary>
    /// Starts bending certain finger based on <see cref="HandPhysicsExtenstions.FingersConfig.BendSpeed"/>
    /// </summary>
    public void StartBendFinger(FingerType fingerType)
    {
        Parts.Fingers[(int)fingerType].StartBending();
    }

    /// <summary>
    /// Stops bending certain finger
    /// </summary>
    public void StopBendFinger(FingerType fingerType)
    {
        Parts.Fingers[(int)fingerType].StopBending();

        int holdersCount = 0;
        for (int i = 1; i < Parts.Fingers.Length; i++)
            if (Parts.Fingers[i].IsHoldingObject)
                holdersCount++;

        if (holdersCount == 0)
            DetachObject();
    }


    #if UNITY_EDITOR
    void OnDrawGizmos()
    {
        if (!DrawGizmos)
            return;
        
        DrawPartsLinks();
        DrawPositionLimits();
    }

    void DrawPartsLinks()
    {
        if (!Parts.CheckIfNull())
        {
            Color hc = Color.green;
            hc.a = 0.2f;
            Handles.color = hc;

            float cubeSize = 0.025f * transform.localScale.magnitude;

            Handles.CubeCap(-1, Parts.Forearm.transform.position, Parts.Forearm.transform.rotation, cubeSize);
            Handles.CubeCap(-1, Parts.Wrist.transform.position, Parts.Wrist.transform.rotation, cubeSize);
            Handles.DrawLine(Parts.Forearm.transform.position, Parts.Wrist.transform.position);

            for (int i = 0; i < Parts.Fingers.Length; i++)
            {
                FingerPart[] fParts = Parts.Fingers[i].Parts;
                Handles.CubeCap(-1, fParts[0].transform.position, fParts[0].transform.rotation, cubeSize);
                Handles.DrawLine(Parts.Wrist.transform.position, fParts[0].transform.position);
                for (int j = 1; j < fParts.Length; j++)
                {
                    Handles.CubeCap(-1, fParts[j].transform.position, fParts[j].transform.rotation, cubeSize);
                    Handles.DrawLine(fParts[j - 1].transform.position, fParts[j].transform.position);
                }
            }
        }
    }

    void DrawPositionLimits()
    {
        if (!Forearm.PositionLimits.UseLimits)
            return;

        Color limitsColor = Color.red;
        limitsColor.a = 0.5f;
        Gizmos.color = limitsColor;

        var posLimits = Forearm.PositionLimits;

        Vector3 center = Vector3.zero;
        if (posLimits.Relativity == Space.Self)
            center = transform.position;

        Gizmos.DrawWireCube(center + posLimits.BoundsOffset, Forearm.PositionLimits.Bounds);
    }
    #endif
}

namespace HandPhysicsExtenstions
{
    #region Classes
    [Serializable]
    public sealed class ForearmConfig
    {
        [Tooltip("Determines coordinate system for movement. Use Space.Self if forearm rotation potentially will be different from Vector3.zero")]
        public Space MovementRelativity = Space.World;
        [Tooltip("Movement speed for each axis in local space")]
        public Vector3 MovementSpeed = new Vector3(8, 120, 8);

        [Range(0f, 1f)]
        [Tooltip("Determines movement responsivity. Set it to 1 for full control")]
        public float MovementHardness = 0.125f;

        [Space]
        [Tooltip("Rotation speed along joint X axis")]
        public float RotationSpeed = 8;
        [Range(0f, 1f)]
        [Tooltip("Determines rotation responsivity. Set it to 1 for full control")]
        public float RotationHardness = 0.6f;

        [Space]
        [Tooltip("Fixing forearm position when holding an object to prevent falling down due to mass differences")]
        public AxesFixation FixAxis;
        [Tooltip("Sets forearm position limits so forearm will never be out of specified bounds")]
        public PositionLimits PositionLimits;
    }

    [Serializable]
    public sealed class WristConfig
    {
        [Tooltip("Rotation speed along joint X axis")]
        public float RotationSpeed = 8;

        [Range(0f, 1f)]
        [Tooltip("Determines rotation responsivity. Set it to 1 for full control")]
        public float RotationHardness = 0.2f;
        [Tooltip("If enabled, wrist will always return to its initial rotation")]
        public bool HoldRotation = true;
    }

    [Serializable]
    public sealed class FingersConfig
    {
        [Tooltip("Sets rotation mode for finger parts")]
        public FingerRotationMode RotationMode = FingerRotationMode.Smooth;
        [Tooltip("Determines rotation speed for all finger parts")]
        public float BendSpeed = 8;
        [Space]
        [Tooltip("If enabled, only objects with PickableObject component attached may be picked")]
        public bool AttachPickableOnly = false;
        [Tooltip("Determines joint break force for attached object")]
        public float ForceToDetachObject = Mathf.Infinity;
    }
    #endregion

    #region Structs
    [Serializable]
    public struct HandParts
    {
        public HandPart Forearm;
        public HandPart Wrist;
        public Finger[] Fingers;

        /// <summary>
        /// Assigns provided controller to all hand parts
        /// </summary>
        /// <param name="controller"></param>
        public void SetController(HandPhysicsController controller)
        {
            var allParts = GetAll();
            for (int i = 0; i < allParts.Length; i++)
            {
                allParts[i].Controller = controller;
            }
        }

        /// <summary>
        /// Enables or disable collisions between all <see cref="HandParts"/> and provided <see cref="rigidbody"/>
        /// </summary>
        /// <param name="rigidbody"></param>
        /// /// <param name="ignore"></param>
        public void IgnoreCollisions(Rigidbody rigidbody, bool ignore)
        {
            Collider rbCollider = rigidbody.GetComponent<Collider>();
            if (rbCollider == null)
                return;

            
            var allParts = GetAll();
            for (int i = 0; i < allParts.Length; i++)
                Physics.IgnoreCollision(rbCollider, allParts[i].Collider, ignore);
        }

        /// <summary>
        /// Turns off all collisions betweel all hand parts
        /// </summary>
        public void DisableAllCollisions()
        {
            var allParts = GetAll();
            for (int i = 0; i < allParts.Length; i++)
                for (int j = 0; j < allParts.Length; j++)
                    Physics.IgnoreCollision(allParts[i].Collider, allParts[j].Collider, true);
        }

        /// <summary>
        /// Returns array of all hand parts
        /// </summary>
        /// <returns></returns>
        public HandPart[] GetAll()
        {
            var allParts = new List<HandPart> { Forearm, Wrist };
            if (Fingers.Length == 0)
            {
                allParts.Add(null);
                return allParts.ToArray();
            }
                
            for (int i = 0; i < Fingers.Length; i++)
            {
                if (Fingers[i] != null)
                    allParts.AddRange(Fingers[i].Parts);
                else allParts.Add(null);
            }

            return allParts.ToArray();
        }

        /// <summary>
        /// Returns true if all parts are linked
        /// </summary>
        /// <returns></returns>
        public bool CheckIfNull()
        {
            HandPart[] allParts = GetAll();
            for (int i = 0; i < allParts.Length; i++)
            {
                if (allParts[i] == null)
                    return true;
            }

            return false;
        }
    }

    [Serializable]
    public struct PositionLimits
    {
        public bool UseLimits;
        public Space Relativity;

        public Vector3 Bounds;
        public Vector3 BoundsOffset;
    }

    [Serializable]
    public struct AxesFixation
    {
        [Range(0f, 1f)]
        public float Hardness;

        public bool X;
        public bool Y;
        public bool Z;
    }
    #endregion

    #region Enums
    public enum FingerType
    {
        Thumb = 0, Index, Middle, Ring, Pinky
    }

    public enum FingerRotationMode
    {
        Linear, Smooth
    }
    #endregion

    #region Delegates
    public delegate void ObjectTouched();
    public delegate void ObjectAttached(Rigidbody rigidbody);
    public delegate void ObjectDetached();
    #endregion
}