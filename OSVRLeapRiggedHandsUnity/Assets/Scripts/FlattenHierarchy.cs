using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlattenHierarchy : MonoBehaviour {

	// Use this for initialization
	void Start () {
		foreach(Transform c in this.transform)
        {
            c.transform.parent = this.transform;
            foreach(Transform gc in c)
            {
                gc.transform.parent = this.transform;
                foreach (Transform gcc in gc)
                {
                    gcc.transform.parent = this.transform;
                }
            }
        }
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
