using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FogCon : MonoBehaviour
{
    public void ChangeColour(float val)
    {
        RenderSettings.fogColor = new Vector4(val, 0 , 0, 1);
    }
}
