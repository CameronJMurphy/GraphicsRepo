using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChangeSkin : MonoBehaviour
{
    public Texture redSkin, greenSkin, blueSkin;

    public void ChangeSkinColour(int val)
    {
        if(val == 0)
        {
            GetComponent<Renderer>().material.SetTexture("_MainTex", redSkin);
        }
        if (val == 1)
        {
            GetComponent<Renderer>().material.SetTexture("_MainTex", greenSkin);
        }
        if (val == 2)
        {
            GetComponent<Renderer>().material.SetTexture("_MainTex", blueSkin);
        }
    }
}
