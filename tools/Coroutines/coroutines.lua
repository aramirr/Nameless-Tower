-- Coroutines code
all_co = {}

function startCoroutine(name, func, ...)
	print("New coroutine "..name)
	local co = coroutine.create(func)
	coroutine.resume(co, ...)

	all_co[name] = co
end

function updateCoroutines(elapsed)
	local name, co = next(all_co)
	while name ~= nil do
		local ok = coroutine.resume(co, elapsed)
		if ok == false then
			all_co[name] = nil
		end

		name, co = next(all_co, name)
	end
end

function clearCoroutines()
	local name, co = next(all_co)
	while name ~= nil do
		co = nil
		name, co = next(all_co, name)
	end
end

function dumpCoroutines()
	print( "Dumping coroutines" )
	local name, co = next(all_co)
	while name ~= nil do
		local status = coroutine.status(co)
		print("  " .. name .. ": " .. status)
		name, co = next(all_co, name)
	end
	print( "Current: " .. tostring(coroutine.running()) )
end

function wait( duration )
	--print("Waiting " .. tostring(duration))
	local time = 0.0
	while time < duration do
		local elapsed = coroutine.yield()
		time = time + elapsed
	end
end

function waitCondition(func, ...)
	while true do
		local result = func(...)
		if result == true then
			return
		end
		coroutine.yield()
	end
end

print('Coroutines ready')
