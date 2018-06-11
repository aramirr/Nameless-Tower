
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

function OnTriggerEnterSierra()
	lm:printdbg("joselito")
end

function OnLevel1Start()
	lm:set_ambient(1)
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
	lm:disactivate_text("text_pista")
end

function OnTriggerEnterTriggerWallEntry03_2()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall03")
end

function OnTriggerEnterTriggerWallExit03_2()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall03")
	lm:disactivate_text("text_pista")
end

function OnTriggerEnterTriggerWallEntry04()
	lm:set_ambient(0.1)
	lm:hide_mesh("Wall04")
	lm:activate_text("text_sala_eleccion")
end

function OnTriggerEnterTriggerWallExit04()
	lm:set_ambient(1)
	lm:unhide_mesh("Wall04")
	lm:disactivate_text("text_sala_eleccion")
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

function OnTriggerEnterATextPista()
	
	lm:activate_text("text_pista")
end

function OnTriggerEnterHTextPista()
	lm:disactivate_text("text_pista")
end

function OnTriggerEnterATextSalaEleccion()
	
	lm:activate_text("text_sala_eleccion")
end

function OnTriggerEnterHTextSalaEleccion()
	lm:disactivate_text("text_sala_eleccion")
end

function OnTriggerEnterATextPlatActivable()
	
	lm:activate_text("text_plataforma_activable")
end

function OnTriggerEnterHTextPlatActivable()
	lm:disactivate_text("text_plataforma_activable")
end

function OnTriggerEnterATextPlanear()
	
	lm:activate_text("text_planear")
end

function OnTriggerEnterHTextPlanear()
	lm:disactivate_text("text_planear")
end

function OnTriggerEnterCinematica()
	lm:play_last_cinematic()
end