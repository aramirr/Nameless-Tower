
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

-- Debug

function OnTriggerEnterDebugTrigger()
	lm:set_dir_light_intensity("LightDir00", 0)
end


function OnLevel1Start()
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir02", 0)
	lm:set_dir_light_intensity("LightDir03", 0)
	lm:set_dir_light_intensity("LightInteriorS4", 0)
	lm:set_dir_light_intensity("LightInteriorS4L", 0)

end

function OnLevel2Start()
	lm:set_exposure(0.36)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir01", 0)
	lm:set_dir_light_intensity("LightDir02", 0)
	lm:set_dir_light_intensity("LightDir03", 0)	
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 1)

end

function OnWindstrikeActivateTDWindstrike()
	lm:open_door("Puerta_Abatible_Madera_01")
end

function OnWindstrikeActivateTDEscaleras2()
	lm:open_door("Puerta_Abatible_Madera_003")
end

function OnWindstrikeActivateTDTechos()
	lm:open_door("Puerta_Abatible_Madera_002")
end

function OnWindstrikeActivateTAPuertasPrimera()
	lm:open_door("Puerta_Reja_01")
	lm:activate_anim("Activador_Movil03")
end

function OnWindstrikeActivateTAPuertasAbajo()
	lm:open_door("Puerta_Reja_005")
	lm:activate_anim("Activador_Movil02")
end

function OnWindstrikeActivateTAPuertasArriba()
	lm:open_door("Puerta_Reja_04")
	lm:activate_anim("Activador_Movil04")
end

function OnWindstrikeActivateTARunner()
	lm:open_door("Puerta_Reja_02")
	lm:activate_anim("Activador_Movil01")
end


-- Sala 1
function OnTriggerEnterTSCabeza()
	-- body
end

-- Sala 2
function OnTriggerEnterTPTutorialIzq()
	-- body
end

function OnTriggerEnterTPTutorialDer()
	lm:set_dir_light_intensity("LightDir01", 10)
end

-- Sala 3
function OnTriggerEnterTSEstatuaAbajo()
	-- body
end

function OnTriggerEnterTSEstatuaArriba()
	lm:unhide_mesh("ParedTapada4")
	lm:hide_mesh("ParedDestapada4")
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir00", 10)
	lm:set_dir_light_intensity("LightDir01", 10)
	lm:set_dir_light_intensity("LightInteriorS4", 0)
	lm:set_dir_light_intensity("LightInteriorS4L", 0)
	lm:set_dir_light_intensity("LightInteriorS5", 0)
	lm:deactivate_cinematic("cinematic_first_door")
end

-- Sala 4
function OnTriggerEnterTSWindstrikeDer()
	lm:hide_mesh("ParedTapada4")
	lm:unhide_mesh("ParedDestapada4")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir01", 0)
	lm:set_dir_light_intensity("LightInteriorS4", 1)
	lm:set_dir_light_intensity("LightInteriorS4L", 1)
	lm:set_dir_light_intensity("LightInteriorS5", 1)
	lm:activate_cinematic("cinematic_first_door")
end

function OnTriggerEnterTSWindstrikeIzq()
	lm:hide_mesh("ParedTapada4")
	lm:unhide_mesh("ParedDestapada4")

	lm:hide_mesh("ParedDestapada5")
	lm:unhide_mesh("ParedTapada5")
	lm:set_ambient(0.2)
	lm:activate_cinematic("cinematic_first_door")
	lm:set_dir_light_intensity("LightInteriorS4L", 0)
	lm:set_point_light_intensity("Luces_Fuego_01", 0)
	lm:set_point_light_intensity("Luces_Fuego_02", 0)
end

-- Sala 5
function OnTriggerEnterTSEscaleras1Izq()
	lm:unhide_mesh("ParedTapada4")
	lm:hide_mesh("ParedDestapada4")

	lm:unhide_mesh("ParedDestapada5")
	lm:hide_mesh("ParedTapada5")
	lm:set_ambient(0.2)
	lm:deactivate_cinematic("cinematic_first_door")
	lm:set_point_light_intensity("Luces_Fuego_01", 10)
	lm:set_point_light_intensity("Luces_Fuego_02", 10)
end

function OnTriggerEnterTSEscaleras1Der()
	lm:unhide_mesh("ParedDestapada5")
	lm:hide_mesh("ParedTapada5")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir01", 0)
	lm:set_dir_light_intensity("LightInteriorS4", 1)
	lm:set_dir_light_intensity("LightInteriorS4L", 1)
	lm:set_dir_light_intensity("LightInteriorS5", 1)
	lm:set_point_light_intensity("Luces_Fuego_01", 10)
	lm:set_point_light_intensity("Luces_Fuego_02", 10)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2")
end

-- Sala 6
function OnTriggerEnterTSEscaleras2Izq()
	lm:hide_mesh("ParedDestapada5")
	lm:unhide_mesh("ParedTapada5")
	lm:set_ambient(1)
	lm:set_point_light_intensity("Luces_Fuego_01", 0)
	lm:set_point_light_intensity("Luces_Fuego_02", 0)
	lm:set_dir_light_intensity("LightDir00", 10)	
	lm:set_dir_light_intensity("LightInteriorS4", 0)
	lm:set_dir_light_intensity("LightInteriorS4L", 0)
	lm:set_dir_light_intensity("LightInteriorS5", 0)	
	lm:activate_cinematic("cinematic_sala_escaleras_2")
end

function OnTriggerEnterTSEscaleras2Der()
	lm:hide_mesh("ParedDestapada7")
	lm:unhide_mesh("ParedTapada7")
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir00", 10)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 0)
	lm:set_point_light_intensity("Luces_Fuego_03", 0)
	lm:set_point_light_intensity("Luces_Fuego_04", 0)
	lm:set_point_light_intensity("Luces_Fuego_05", 0)
	lm:set_point_light_intensity("Luces_Fuego_06", 0)
	lm:activate_cinematic("cinematic_sala_escaleras_2")
end

-- Sala 7
function OnTriggerEnterTPTechosIzq()
	lm:unhide_mesh("ParedDestapada7")
	lm:hide_mesh("ParedTapada7")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir01", 10)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 1)
	lm:set_point_light_intensity("Luces_Fuego_03", 10)
	lm:set_point_light_intensity("Luces_Fuego_04", 10)
	lm:set_point_light_intensity("Luces_Fuego_05", 10)
	lm:set_point_light_intensity("Luces_Fuego_06", 10)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2")
end

function OnTriggerEnterTPTechosDer()
	lm:unhide_mesh("ParedDestapada7")
	lm:hide_mesh("ParedTapada7")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir03", 0)
	lm:set_dir_light_intensity("LightDir02", 0)
	lm:set_dir_light_intensity("LightDir01", 10)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 1)
	lm:set_point_light_intensity("Luces_Fuego_03", 10)
	lm:set_point_light_intensity("Luces_Fuego_04", 10)
	lm:set_point_light_intensity("Luces_Fuego_05", 10)
	lm:set_point_light_intensity("Luces_Fuego_06", 10)
	lm:set_point_light_intensity("Luces_Fuego_08", 0)	
end

-- Sala 8
function OnTriggerEnterTSPuertasIzq()
	lm:hide_mesh("ParedDestapada7")
	lm:unhide_mesh("ParedTapada7")
	lm:set_ambient(1)
	lm:deactivate_cinematic("cinematic_vientos")
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 0)
	lm:set_dir_light_intensity("LightDir02", 10)
	lm:set_point_light_intensity("Luces_Fuego_03", 0)
	lm:set_point_light_intensity("Luces_Fuego_04", 0)
	lm:set_point_light_intensity("Luces_Fuego_05", 0)
	lm:set_point_light_intensity("Luces_Fuego_06", 0)
	lm:set_point_light_intensity("Luces_Fuego_08", 10)
end

function OnTriggerEnterTSPuertasZoom()
	-- body
end

function OnTriggerEnterTSPuertasDer()
	lm:set_point_light_intensity("Luces_Fuego_08", 10)
	lm:deactivate_cinematic("cinematic_vientos")
end

-- Sala 9
function OnTriggerEnterTSArbolAbajo()
	lm:activate_cinematic("cinematic_vientos")
	lm:set_point_light_intensity("Luces_Fuego_08", 0)
end

function OnTriggerEnterTSArbolArriba()
	lm:hide_mesh("ParedDestapada10")
	lm:unhide_mesh("ParedTapada10")
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir02", 10)
	lm:set_point_light_intensity("Luces_Fuego_07", 0)
	lm:set_point_light_intensity("Luces_Fuego_09", 0)
	lm:set_point_light_intensity("Luces_Fuego_010", 0)
	lm:set_dir_light_intensity("LightInteriorSCaldero", 0)
	lm:activate_cinematic("cinematic_vientos")
end

-- Sala 10
function OnTriggerEnterTSRunnerIzq()
	lm:unhide_mesh("ParedDestapada10")
	lm:hide_mesh("ParedTapada10")
	lm:deactivate_cinematic("cinematic_vientos")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir00", 0)
	lm:set_dir_light_intensity("LightDir02", 0)
	lm:set_point_light_intensity("Luces_Fuego_07", 10)
	lm:set_point_light_intensity("Luces_Fuego_09", 10)
	lm:set_point_light_intensity("Luces_Fuego_010", 10)
	lm:set_dir_light_intensity("LightInteriorSCaldero", 1)
end

function OnTriggerEnterTSRunnerDer()
	lm:unhide_mesh("ParedDestapada10")
	lm:hide_mesh("ParedTapada10")
	lm:set_ambient(0.2)
	lm:set_dir_light_intensity("LightDir03", 0)
	lm:set_point_light_intensity("Luces_Fuego_07", 10)
	lm:set_point_light_intensity("Luces_Fuego_09", 10)
	lm:set_point_light_intensity("Luces_Fuego_010", 10)
	lm:set_dir_light_intensity("LightInteriorSCaldero", 2)
end

-- Sala 11
function OnTriggerEnterTSCascadaAbajo()
	lm:hide_mesh("ParedDestapada10")
	lm:unhide_mesh("ParedTapada10")
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir00", 10)
	lm:set_dir_light_intensity("LightInteriorSCaldero", 0)
	lm:set_point_light_intensity("Luces_Fuego_07", 0)
	lm:set_point_light_intensity("Luces_Fuego_09", 0)
	lm:set_point_light_intensity("Luces_Fuego_010", 0)
end

function OnTriggerEnterTSCascadaArriba()
	lm:hide_mesh("ParedDestapada12")
	lm:unhide_mesh("ParedTapada12")
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir03", 10)
	lm:set_point_light_intensity("Luces_Fuego_011", 0)
	lm:set_point_light_intensity("Luces_Fuego_012", 0)
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 0)

end

-- Sala 12
function OnTriggerEnterTSAntorchasIzq()
	lm:unhide_mesh("ParedDestapada12")
	lm:hide_mesh("ParedTapada12")
	lm:change_level("level_2")
	lm:set_point_light_intensity("Luces_Fuego_011", 10)
	lm:set_point_light_intensity("Luces_Fuego_012", 10)
end

function OnTriggerEnterTSAntorchasDer()
	lm:unhide_mesh("ParedDestapada12")
	lm:hide_mesh("ParedTapada12")
	lm:set_ambient(0.2)	
	lm:hide_mesh("ParedDestapada13")
	lm:unhide_mesh("ParedTapada13")
	lm:set_point_light_intensity("Luces_Fuego_011", 10)
	lm:set_point_light_intensity("Luces_Fuego_012", 10)
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 1)
end

-- Sala 13
function OnTriggerEnterTTSalidaAbajo()
	lm:hide_mesh("ParedDestapada12")
	lm:unhide_mesh("ParedTapada12")
	lm:set_ambient(0.2)

	lm:unhide_mesh("ParedDestapada13")
	lm:hide_mesh("ParedTapada13")
end

function OnTriggerEnterTTSalidaArriba()
	-- body
end

function OnTriggerEnterCinematica()
	lm:play_last_cinematic()
end