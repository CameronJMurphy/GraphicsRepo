using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{
    Animator animator;
    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        float turn = Input.GetAxis("Horizontal");
        float move = Input.GetAxis("Vertical");
        if(Input.GetKey(KeyCode.LeftShift))
        {
            move *= 2;
        }
        if (Input.GetKey(KeyCode.LeftControl))
        {
            animator.SetFloat("Crouch", 1);
        }
        else
        {
            animator.SetFloat("Crouch", -1);
        }
        animator.SetFloat("Turn", turn);
        animator.SetFloat("Move", move);
    }
}
