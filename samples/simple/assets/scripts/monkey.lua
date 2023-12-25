transform_ = GetTransform();

--@serializable
speed_ = 45;

--@serializable
local foo_ = "Hello, World"

function OnStart()
end

function OnUpdate(ds)
  transform_:Rotate(speed_ * ds, Vec3.new(0, 1, 0));
end

function OnDestroy()
end
