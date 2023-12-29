using System;
using EveEngine;

namespace Simple
{
    public class Cube : ScriptEntity
    {
        private Transform m_transform;
        private bool m_space_pressed = false;

        public float speed = 10.0f;

        private void OnCreate()
        {
            Console.WriteLine("Hello, World!");

            m_transform = GetComponent<Transform>();
        }

        private void OnUpdate(float ds)
        {
            Vector3 direction = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
            {
                direction += m_transform.GetForward();
            }
            if (Input.IsKeyPressed(KeyCode.S))
            {
                direction -= m_transform.GetForward();
            }

            if (Input.IsKeyPressed(KeyCode.D))
            {
                direction += m_transform.GetRight();
            }
            if (Input.IsKeyPressed(KeyCode.A))
            {
                direction -= m_transform.GetRight();
            }

            if (!m_space_pressed && Input.IsKeyPressed(KeyCode.Space))
            {
                SceneManager.SetActive(1);
                m_space_pressed = true;
            }

            if (direction != Vector3.Zero)
            {
                m_transform.Position += Mathf.Normalize(direction) * speed * ds;
            }
        }
    }
}
