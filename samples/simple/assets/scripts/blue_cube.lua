transform = GetTransform();

--@serializable
speed = 20.0

function OnUpdate(ds)
  local direction = Vec3:new()

  if IsKeyPressed(KeyCode.W) then
    direction.z = direction.z + speed
  end
  if IsKeyPressed(KeyCode.S) then
    direction.z = direction.z - speed
  end

  if IsKeyPressed(KeyCode.D) then
    direction.x = direction.x + speed
  end
  if IsKeyPressed(KeyCode.A) then
    direction.x = direction.x - speed
  end

  transform.position = transform.position + direction * ds;
end
