using UnityEngine;
using System.Collections;

public class showcasecam : MonoBehaviour {
	
	public Transform rotationPoint;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.RotateAround(rotationPoint.position, Vector3.up, 20 * Time.deltaTime);
	}
}
