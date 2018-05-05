
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

function OnTriggerEnterSierra()
	lm:printdbg("joselito")
end

function OnTriggerEnterRoom1()
	val1, val2, val3 = lm:test_vec3()
end

function OnTriggerEnterTriggerWallEntry01()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall01")
end

function OnTriggerEnterTriggerWallExit01()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall01")
end

function OnTriggerEnterTriggerWallEntry02()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall02")
end

function OnTriggerEnterTriggerWallExit02()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall02")
end

function OnTriggerEnterTriggerWallEntry03()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall03")
end

function OnTriggerEnterTriggerWallExit03()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall03")
end

function OnTriggerEnterTriggerWallEntry03_2()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall03")
end

function OnTriggerEnterTriggerWallExit03_2()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall03")
end