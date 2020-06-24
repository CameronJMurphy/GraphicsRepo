using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{
    Animator animator;
    public float jumpHeight;
    bool onGround;
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
        if(Input.GetKey(KeyCode.LeftShift) && onGround)
        {
            move *= 2;
        }


        if (Input.GetKey(KeyCode.LeftControl) && onGround)
        {
            animator.SetFloat("Crouch", 1);
        }
        else
        {
            animator.SetFloat("Crouch", -1);
        }
        if(Input.GetKeyDown(KeyCode.Space) && onGround)
        {
            animator.SetFloat("Jump", 1);
            GetComponent<Rigidbody>().AddForce(new Vector3(GetComponent<Rigidbody>().velocity.x, GetComponent<Rigidbody>().velocity.y + jumpHeight, GetComponent<Rigidbody>().velocity.z));
        }
        animator.SetFloat("Turn", turn);
        animator.SetFloat("Move", move);
    }

    private void OnCollisionEnter(Collision collision)
    {
        onGround = true;
        animator.SetFloat("Jump", 0);
    }
    private void OnCollisionStay(Collision collision)
    {
        onGround = true;
    }
    private void OnCollisionExit(Collision collision)
    {
        onGround = false;
    }

}
