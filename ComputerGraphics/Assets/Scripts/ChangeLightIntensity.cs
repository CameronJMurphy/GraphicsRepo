using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChangeLightIntensity : MonoBehaviour
{
    public void ChangeBrightness(float val)
    {
        GetComponent<Light>().intensity = val + 2;
    }
}
