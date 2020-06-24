using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePlatform : MonoBehaviour
{
    float lastVal = 0;
    [Range(-1, 1)] public int direction;
    [Range(1, 4)] public int speed;

    public void Move(float val)
    {
        transform.position = new Vector3(transform.position.x, transform.position.y + ((val - lastVal) * direction * speed), transform.position.z);
        lastVal = val;
    }
}
