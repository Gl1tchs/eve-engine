transform = GetTransform();

--@serializable
speed = 45

function OnStart()
  print("Red Monkey: " .. GetId());
end

function OnUpdate(ds)
  transform.rotation.x = transform.rotation.x + speed * ds;
end

function OnDestroy()
end
