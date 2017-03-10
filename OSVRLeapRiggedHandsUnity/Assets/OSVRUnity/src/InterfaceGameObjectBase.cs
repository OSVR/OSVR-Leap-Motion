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

namespace OSVR.Unity
{
    /// <summary>
    /// A script component to add to a GameObject in order to access an interface, managing lifetime and centralizing the path specification.
    /// </summary>
    public class InterfaceGameObjectBase : MonoBehaviour
    {
        /// <summary>
        /// The interface path you want to connect to. If you end up with a parent-child relationship between two InterfaceGameObjects
        /// in your scene, there is the ability to inherit the path. There are a few instances where this might be useful, but typically
        /// each instance would have the path explicitly specified.
        /// </summary>
        [Tooltip("The interface path you want to access. If left blank, the path of the nearest ancestor with a path will be used.")]
        public string path;
        protected string usedPath;
        #region Private implementation
        
        private class PathHolder : MonoBehaviour
        {
            [HideInInspector]
            public string path;
            
            void Awake()
            {
                hideFlags = HideFlags.HideAndDontSave;
            }
        }
        #endregion
        
        #region Methods for Derived Classes
        /// <summary>
        /// Call from your Awake method to advertise the presence or absence of a path specification on this game object.
        /// </summary>
        protected void AdvertisePath()
        {
            PathHolder holder = GetComponent<PathHolder>();
            if (path.Length > 0) {
                /// If we have a path, be sure we advertise it.
                if (null == holder) {
                    holder = gameObject.AddComponent<PathHolder>();
                }
                holder.path = path;
            } else {
                /// Don't advertise a path that is empty
                if (null != holder) {
                    Object.Destroy(holder);
                }
            }
        }
        
        /// <summary>
        /// Call from your Start method
        /// </summary>
        protected virtual void Start()
        {
            AdvertisePath();
            GameObject go = this.gameObject;
            PathHolder holder = null;
            while (null != go && System.String.IsNullOrEmpty(usedPath))
            {
                usedPath = path;
                holder = go.GetComponent<PathHolder>();
                if (null != holder)
                {
                    usedPath = holder.path;
                    //Debug.Log("[OSVR-Unity] " + name + ": Found path " + usedPath + " in ancestor " + go.name);
                }
                go = GetParent.Get(go);
            }
            
            if (0 == usedPath.Length)
            {
                Debug.LogError("[OSVR-Unity] Missing path for " + name + " - no path found in this object's InterfaceGameObject or any ancestor!");
                return;
            }
        }
        
        protected virtual void Stop()
        {
            PathHolder holder = GetComponent<PathHolder>();
            if (null != holder)
            {
                Object.Destroy(holder);
            }
        }
        #endregion
        
        #region Event Methods        
        void OnDestroy()
        {
            Stop();
        }
        
        void OnApplicationQuit()
        {
            Stop();
        }
        
        #endregion
        
    }

}
