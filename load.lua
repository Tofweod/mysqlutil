function LOAD(o)
	if o.host == nil then
		error("have not set host")
	end
	return o
end

local path = _G["config-path"]

if string.sub(path, -4) == ".lua" then
	path = string.sub(path, 1, -5)
end

print(string.format("Load file to module: %s", path))

local config = require(path)
return LOAD(config)
