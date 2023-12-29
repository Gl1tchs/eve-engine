using System;
using EveEngine;

namespace Simple
{
    public class Sphere : ScriptEntity
    {
        private ScriptEntity m_cube;

        private void OnCreate()
        {
            m_cube = FindEntityByName("Cube");

            Material material = m_cube.GetComponent<Material>();
            Console.WriteLine(material.Albedo);
            material.Albedo = new Vector3(0.0f, 1.0f, 0.0f);
            Console.WriteLine(material.Albedo);
        }

        private void OnUpdate(float ds)
        {
            GetComponent<Transform>().Rotation += new Vector3(0, 20 * ds, 0);
        }
    }
}