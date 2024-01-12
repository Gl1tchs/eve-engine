using System;
using EveEngine;

namespace Simple
{
    public class Cube : Entity
    {
        private bool _spacePressed = false;

        public float speed = 10.0f;

        protected override void OnCreate()
        {
            for (int i = 0; i < 10; i++)
            {
                Debug.Log(String.Format("Hello, World! {0}", i));
            }
        }

        protected override void OnUpdate(float ds)
        {
            Vector3 direction = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
            {
                direction += Transform.GetForward();
            }
            if (Input.IsKeyPressed(KeyCode.S))
            {
                direction -= Transform.GetForward();
            }

            if (Input.IsKeyPressed(KeyCode.D))
            {
                direction += Transform.GetRight();
            }
            if (Input.IsKeyPressed(KeyCode.A))
            {
                direction -= Transform.GetRight();
            }

            if (!_spacePressed && Input.IsKeyPressed(KeyCode.Space))
            {
                SceneManager.SetActive(1);
                _spacePressed = true;
            }

            if (direction != Vector3.Zero)
            {
                Transform.Position += Mathf.Normalize(direction) * speed * ds;
            }
        }
    }
}
