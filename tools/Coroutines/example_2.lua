print('-------------------')

function dialog(name)
	print("Hello, "..name)
	wait(1.0)
	print("Hi ?")
	wait(1.0)
	print("Are you there, " .. name .. "?")
	wait(1.0)
	print("Bye, then, "..name.. ", dude")
end

local co = coroutine.create(dialog)
local status = coroutine.status(co)
print(status)

local ok, state = coroutine.resume(co, "David")
print("Coroutine status: "..tostring(ok))
print("Internal state: "..state)

ok = coroutine.resume(co, "Charlie", "man")
print("Coroutine status: "..tostring(ok))
print(coroutine.status(co))

ok = coroutine.resume(co)
print("Coroutine status: "..tostring(ok))

print('-------------------')
function movieTitleGenerator()
	local titles = {"Matrix", "Jurassic Park", "Star Wars"}
	local subtitles = {"Reloaded", "Origins", "Payback", "El Documental"}

	while(true) do
		local title = titles[math.random(3)]
		local subtitle = subtitles[math.random(4)]
		local movieTitle = title .. " " .. math.random(10) .. ": " .. subtitle;
		coroutine.yield(movieTitle)
	end
end

--local movies = coroutine.create(movieTitleGenerator)

local movies = coroutine.wrap(movieTitleGenerator)
print(movies())
print(movies())
print(movies())
print(movies())
print(movies())
print(movies())
print(movies())

print('-------------------')
print("RUNNING: " .. tostring(coroutine.running()))
