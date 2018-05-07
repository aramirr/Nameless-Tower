
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

function OnTriggerEnterSierra()
	lm:printdbg("joselito")
end

function OnLevel1Start()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall01")
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

function OnTriggerEnterTriggerWallEntry04()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall04")
end

function OnTriggerEnterTriggerWallExit04()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall04")
end

function OnTriggerEnterTriggerWallEntry05()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall05")
end

function OnTriggerEnterTriggerWallExit05()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall05")
end

function OnTriggerEnterTriggerWallEntry06()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall06")
end

function OnTriggerEnterTriggerWallExit06()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall06")
end

function OnTriggerEnterTriggerWallEntry07()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall07")
end

function OnTriggerEnterTriggerWallExit07()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall07")
end

function OnTriggerEnterTriggerWallEntry08()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall08")
end

function OnTriggerEnterTriggerWallExit08()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall08")
end