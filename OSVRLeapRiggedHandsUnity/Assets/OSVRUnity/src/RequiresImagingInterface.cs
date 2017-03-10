﻿/// OSVR-Unity Connection
///
/// http://sensics.com/osvr
///
/// <copyright>
/// Copyright 2016 Sensics, Inc.
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

using UnityEngine;
using System.Collections;
using OSVR.ClientKit;
using System;

namespace OSVR
{
    namespace Unity
    {

        [RequireComponent(typeof(ImagingInterface))]
        public class RequiresImagingInterface : MonoBehaviour, IRequiresInterface<OSVR.ClientKit.ImagingState>
        {
            /// <summary>
            /// Accessor for the sibling PoseInterface component.
            /// </summary>
            private ImagingInterface ifaceComponent;
            public OSVR.ClientKit.IInterface<OSVR.ClientKit.ImagingState> Interface
            {
                get
                {
                    if (ifaceComponent == null)
                    {
                        ifaceComponent = GetComponent<ImagingInterface>();
                    }
                    return ifaceComponent == null ? null : ifaceComponent.Interface;
                }
            }
        }
    }
}

