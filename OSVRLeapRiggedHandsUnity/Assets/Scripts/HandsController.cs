using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandsController : MonoBehaviour {

    public SkeletonConfidenceAnalog leftHandConfidenceAnalog;
    public SkeletonConfidenceAnalog rightHandConfidenceAnalog;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKeyDown(KeyCode.RightArrow))
        {
            leftHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex + 1);
            rightHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex + 1);

        }
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            leftHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex - 1);
            rightHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex - 1);

        }
    }

    void OnGUI()
    {
        if (GUILayout.Button("Next Hands (right arrow)"))
        {
            leftHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex + 1);
            rightHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex + 1);
        }
        if (GUILayout.Button("Prev Hands (left arrow)"))
        {
            leftHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex - 1);
            rightHandConfidenceAnalog.SetCurrentIndex(leftHandConfidenceAnalog.CurrentIndex - 1);
        }
    }
}
