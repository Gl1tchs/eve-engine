using System;
using EveEngine;

namespace Simple
{
    public class Sphere : ScriptEntity
    {
        private ScriptEntity m_cube;

        public Vector3 color;

        private void OnCreate()
        {
            m_cube = FindEntityByName("Cube");

            Material material = m_cube.GetComponent<Material>();
            material.Albedo = color;
        }

        private void OnUpdate(float ds)
        {
            GetComponent<Transform>().Rotation
                += new Vector3(0, 20 * ds, 0);
        }
    }
}