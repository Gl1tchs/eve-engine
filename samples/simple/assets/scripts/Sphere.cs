using System;
using EveEngine;

namespace Simple
{
    public class Sphere : Entity
    {
        private Entity _cube;

        public Color color;

        protected override void OnCreate()
        {
            _cube = TryGetEntityByName("Cube");
            if (!_cube)
            {
                Console.WriteLine("Unable to find cube!");
                return;
            }

            Material material = _cube.GetComponent<Material>();
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