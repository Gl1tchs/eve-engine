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
            if (!m_cube)
            {
                Console.WriteLine("Unable to find cube!");
                return;
            }

            Material material = m_cube.GetComponent<Material>();
            if (material != null)
            {
                material.Albedo = color;
            }
        }

        private void OnUpdate(float ds)
        {
            GetComponent<Transform>().Rotation
                += new Vector3(0, 20 * ds, 0);
        }
    }
}