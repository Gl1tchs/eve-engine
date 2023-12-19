transform = GetTransform();

--@serializable
speed = 45;

function OnUpdate(ds)
  transform.rotation.x = transform.rotation.x + speed * ds;
end
