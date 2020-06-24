using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Win : MonoBehaviour
{
    bool win = false;
    float timer = 0;
    float timerMax = 5;
    public TextMeshProUGUI winText;
    private void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("Player"))
        {
            winText.gameObject.SetActive(true);
            win = true;
        }
    }

    private void Update()
    {
        if(win)
        {
            if(timer < timerMax)
            {
                timer += Time.deltaTime;
            }
            else
            {
                Application.Quit();
                Debug.Log("Quitting");
            }
        }
    }
}
