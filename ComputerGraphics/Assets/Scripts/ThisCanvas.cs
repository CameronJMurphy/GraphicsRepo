using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ThisCanvas : MonoBehaviour
{
    static ThisCanvas instance = null;
    // Start is called before the first frame update
    private void Awake()
    {
        if (instance != null)
        {
            Destroy(gameObject);
        }
        else
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
    }
}
