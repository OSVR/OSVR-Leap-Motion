/// OSVR-Unity Connection
///
/// http://sensics.com/osvr
///
/// <copyright>
/// Copyright 2014 Sensics, Inc.
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
/// </copyright>

using System;
using UnityEngine;

namespace OSVR
{
    namespace Unity
    {
        /// <summary>
        /// Skeleton interface: continually (or rather, when OSVR updates) updates its position and orientation based on the incoming tracker data.
        ///
        /// Attach to a GameObject that you'd like to have updated in this way.
        /// </summary>
        public class SkeletonInterface : InterfaceGameObjectBase
        {
            PoseAdapter adapter;
            [SerializeField]
            private Vector3 modelFingerPointingDir;
            [SerializeField]
            private Vector3 modelPalmFacingDir;
            [SerializeField]
            private bool reorient = true;
            override protected void Start()
            {
                base.Start();
                if (adapter == null && !String.IsNullOrEmpty(usedPath))
                {
                    adapter = new PoseAdapter(
                        OSVR.ClientKit.PoseInterface.GetInterface(ClientKit.instance.context, usedPath));
                }
            }

            protected override void Stop()
            {
                base.Stop();
                if (adapter != null)
                {
                    adapter.Dispose();
                    adapter = null;
                }
            }

            void Update()
            {
                if (this.adapter != null)
                {
                    var state = this.adapter.GetState();
                    transform.localPosition = state.Value.Position;
                    if(reorient)
                    {
                        transform.localRotation = state.Value.Rotation * Reorientation();

                    }
                    else
                    {
                        transform.localRotation = state.Value.Rotation;

                    }
                    /*OSVR.ClientKit.Vec3 v;
                    v.x = state.Value.Position.x;

                    v.y = state.Value.Position.y;

                    v.z = state.Value.Position.z;
                    transform.localPosition = Math.ConvertPosition(v);
                    OSVR.ClientKit.Quaternion q;
                    q.x = state.Value.Rotation.x;
                    q.y = state.Value.Rotation.y;
                    q.z = state.Value.Rotation.z;
                    q.w = state.Value.Rotation.w;
                    transform.localRotation = Math.ConvertOrientation(q);*/
                }
            }

            public Quaternion Reorientation()
            {
                return Quaternion.Inverse(Quaternion.LookRotation(modelFingerPointingDir, -modelPalmFacingDir));
            }

            public OSVR.ClientKit.IInterface<OSVR.Unity.Pose3> Interface
            {
                get
                {
                    this.Start();
                    return adapter;
                }
            }
        }
    }
}
