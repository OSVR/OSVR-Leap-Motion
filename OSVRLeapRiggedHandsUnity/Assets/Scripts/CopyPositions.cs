using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CopyPositions : MonoBehaviour {

    public Transform parentObject;

	// Use this for initialization
	void Start () {

        //for each child of this transform, find a child of the parent object transform with a matching name
        //copy the position and rotation of the parent object's child to our child with the same name
        int numMatches = 0;
        int numMisses = 0;
        Debug.Log("I have " + this.transform.childCount + " children");
		for(int i = this.transform.childCount - 1; i > -1; i--)
        {
            string childName = this.transform.GetChild(i).name;
            Transform otherTransform = parentObject.FindChild(childName);
            if(otherTransform != null)
            {
                numMatches++;
                this.transform.GetChild(i).localPosition = otherTransform.localPosition;
                this.transform.GetChild(i).localRotation = otherTransform.localRotation;

            }
            else
            {
                if(parentObject && childName != null)
                {
                    Debug.Log("Child " + childName + " was not found in parent object " + parentObject.name);
                }
                numMisses++;
            }
        }
        Debug.Log("Traversed, misses " + numMisses + " hits, " + numMatches);
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
