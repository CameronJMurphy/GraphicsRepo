using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChangeMusic : MonoBehaviour
{
    public void EditPitch(float val)
    {
        GetComponent<AudioSource>().pitch = val + 1;
    }
}
