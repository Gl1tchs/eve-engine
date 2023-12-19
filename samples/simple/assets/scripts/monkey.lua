transform = GetTransform();

--@serializable
speed = 45;

--@serializable
foo = "Hello, World"

function OnStart()
end

function OnUpdate(ds)
  transform.rotation.y = transform.rotation.y + speed * ds;
end

function OnDestroy()
end
 