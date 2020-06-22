using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateObject : MonoBehaviour
{
    bool rotateRight;
    bool rotateLeft;
    public void OnRightClick()
    {
        rotateRight = true;
    }
    public void OnRightClickUp()
    {
        rotateRight = false;
    }
    public void OnLeftClick()
    {
        rotateLeft = true;
    }
    public void OnLeftClickUp()
    {
        rotateLeft = false;
    }

    private void Update()
    {
        if(rotateRight)
        {
            gameObject.transform.Rotate(new Vector3(gameObject.transform.rotation.x + 0.5f, gameObject.transform.rotation.y, gameObject.transform.rotation.z + 1));
        }
        if(rotateLeft)
        {
            gameObject.transform.Rotate(new Vector3(gameObject.transform.rotation.x , gameObject.transform.rotation.y - 0.5f, gameObject.transform.rotation.z - 1));
        }
    }
}
