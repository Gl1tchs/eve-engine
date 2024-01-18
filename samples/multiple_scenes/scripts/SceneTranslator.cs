using System;
using EveEngine;

namespace MultipleScenes
{
  public class SceneTranslator : Entity
  {
    public bool isScene1 = false;
    private bool _keyPressed = false;

    protected override void OnCreate()
    {
    }

    protected override void OnUpdate(float ds)
    {
      if (isScene1 && !_keyPressed && Input.IsKeyPressed(KeyCode.Num2))
      {
        SceneManager.SetActive(1);
        _keyPressed = true;
      }
      else if (!isScene1 && !_keyPressed && Input.IsKeyPressed(KeyCode.Num1))
      {
        SceneManager.SetActive(0);
        _keyPressed = true;
      }

      Transform.Rotation += Vector3.Up * 45 * ds;
    }

    protected override void OnDestroy()
    {
      Console.WriteLine("Destroyed!");
    }
  }
}