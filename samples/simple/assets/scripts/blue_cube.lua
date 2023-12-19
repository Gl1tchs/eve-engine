transform = GetTransform();

function OnUpdate(ds)
  transform.rotation.x = transform.rotation.x + 45 * ds;
  transform.rotation.y = transform.rotation.y + 45 * ds;
end
