using System;
using EveEngine;

namespace Simple
{
    public class Sphere : Entity
    {
        private Entity m_cube;

        public Color color;

        protected override void OnCreate()
        {
            m_cube = TryGetEntityByName("Cube");
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

        protected override void OnUpdate(float ds)
        {
            GetComponent<Transform>().Rotation
                += Vector3.Up * 20 * ds;
        }
    }
}