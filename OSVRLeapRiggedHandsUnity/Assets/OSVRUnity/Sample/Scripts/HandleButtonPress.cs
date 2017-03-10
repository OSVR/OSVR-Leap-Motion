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

using UnityEngine;
using System.Collections;

/// <summary>
/// This is a manual way of accessing an interface game component. See SampleButtonScript for the recommended, more elegant solution.
/// </summary>
[RequireComponent(typeof(OSVR.Unity.ButtonInterface))]
public class HandleButtonPress : MonoBehaviour
{
    public void Start()
    {
        gameObject.GetComponent<OSVR.Unity.ButtonInterface>().Interface.StateChanged += handleButton;
    }

    public void handleButton(object sender, OSVR.ClientKit.TimeValue timestamp, int sensor, byte report)
    {
        Debug.Log("[OSVR-Unity-Samples] Got button: " + sensor.ToString() + " state is " + report.ToString());
    }
}
