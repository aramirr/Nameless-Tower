print('-------------------')

exitVariable = false
function exit()
	return exitVariable
end

function isAnimationActive(animationName)
	--local anims = getAnimationsList()
	return false
end

function dialog(name)
	print("Hello, "..name)
	wait(1.0)
	print("Hi ?")
	wait(1.0)
	print("Are you there, " .. name .. "?")
	waitCondition(isAnimationActive, "")
	print("Bye, then, "..name.. ", dude")
end

dumpCoroutines()
startCoroutine("initial dialog", dialog, "David")
