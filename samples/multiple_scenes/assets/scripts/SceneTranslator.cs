using System;
using EveEngine;

namespace MultipleScenes
{
  public class SceneTranslator : ScriptEntity
  {
    public bool isScene1 = false;

    private Transform m_transform;
    private bool m_keyPressed = false;

    private void OnCreate()
    {
      m_transform = GetComponent<Transform>();
    }

    private void OnUpdate(float ds)
    {
      if (isScene1 && !m_keyPressed && Input.IsKeyPressed(KeyCode.Num2))
      {
        SceneManager.SetActive(1);
        m_keyPressed = true;
      }
      else if (!isScene1 && !m_keyPressed && Input.IsKeyPressed(KeyCode.Num1))
      {
        SceneManager.SetActive(0);
        m_keyPressed = true;
      }

      m_transform.Rotation += new Vector3(0, 45 * ds, 0);
    }

    private void OnDestroy()
    {
      Console.WriteLine("Destroyed!");
    }
  }
}