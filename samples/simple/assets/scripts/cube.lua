local transform_ = GetTransform();

--@serializable
speed_ = 25.0

space_pressed_ = false

function IsZeroVector3(vec)
  return vec.x == 0.0 and vec.y == 0.0 and vec.z == 0.0;
end

function OnStart()
  print("Hello from " .. GetName())
end

function OnUpdate(ds)
  local direction = Vec3:new()

  if IsKeyPressed(KeyCode.W) then
    direction = direction + transform_:GetForward();
  end
  if IsKeyPressed(KeyCode.S) then
    direction = direction - transform_:GetForward();
  end

  if IsKeyPressed(KeyCode.D) then
    direction = direction + transform_:GetRight();
  end
  if IsKeyPressed(KeyCode.A) then
    direction = direction - transform_:GetRight();
  end

  if not space_pressed_ and IsKeyPressed(KeyCode.Space) then
    SceneManager.SetActive(1);
    space_pressed_ = true;
  end

  if not IsZeroVector3(direction) then
    transform_:Translate(
      Mathf.Normalize(direction) * speed_ * ds
    );
  end
end
