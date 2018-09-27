
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

-- Debug

function OnTriggerEnterDebugTrigger()
	lm:set_dir_light_intensity("LightDir00", 10, false)
	lm:set_dir_light_intensity("LightDir01", 10, false)
end


function OnLevel1Start()
	lm:set_ambient(1)
	lm:set_dir_light_intensity("LightDir00", 11, false)
	lm:set_dir_light_intensity("LightDir01", 11, false)
	lm:play_ambient_sound(true)
	lm:play_ambient_sound(false)
	lm:start_emiter("hojas_inicio", false)
	
	lm:stop_emiter("hojas_puente", false)
	lm:stop_emiter("hojas_estatua", false)
	lm:stop_emiter("hojas_jardin_1", false)
	lm:stop_emiter("hojas_jardin_2", false)
	lm:stop_emiter("hojas_viento", false)
	lm:stop_emiter("particulas_top_cascada", false)
end

function OnLevel2Start()
	lm:set_exposure(0.36)
	lm:set_dir_light_intensity("LightDir00", 0, false)
	lm:set_dir_light_intensity("LightDir01", 0, false)
	lm:set_dir_light_intensity("LightDir02", 0, false)
	lm:set_dir_light_intensity("LightDir03", 0, false)	
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 1, false)

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
function OnTriggerExitTSCabezaTutorial()
	lm:set_dir_light_intensity("LightDir02", 11, false)
	lm:start_emiter("hojas_estatua", false)


	lm:set_dir_light_intensity("LightDir02", 0, true)
	lm:stop_emiter("hojas_estatua", true)
end

-- Sala 2
function OnTriggerExitTPTutorialEstatua()
	lm:set_dir_light_intensity("LightDir00", 11, true)
	lm:start_emiter("hojas_inicio", false)

	lm:set_dir_light_intensity("LightDir00", 0, false)
	lm:stop_emiter("hojas_inicio", true)
end

function OnTriggerExitTSEstatuaWindstrike()
	lm:render_everything("ParedTapada4", false)
	--lm:render_only_shadows("ParedDestapada4", false)	
	lm:set_dir_light_intensity("LightDir01", 11, false)
	lm:set_dir_light_intensity("LightDir02", 11, false)
	lm:set_dir_light_intensity("LightDir03", 0, false)
	lm:set_dir_light_intensity("LightInteriorS4", 0, false)
	lm:set_dir_light_intensity("LightInteriorS4L", 0, false)
	lm:set_dir_light_intensity("LightInteriorS5", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, false)
	lm:deactivate_cinematic("cinematic_first_door", false)
	lm:play_ambient_sound(false)
	lm:start_emiter("hojas_estatua", false)	
	lm:stop_emiter("hojas_puente", false)

	lm:play_interior_sound(true)
	--lm:render_only_shadows("ParedTapada4", true)
	--lm:render_everything("ParedDestapada4", true)
	lm:set_dir_light_intensity("LightDir01", 0, true)
	lm:set_dir_light_intensity("LightDir02", 0, true)
	lm:set_dir_light_intensity("LightDir03", 11, true)
	lm:set_dir_light_intensity("LightInteriorS4", 1, true)
	lm:set_dir_light_intensity("LightInteriorS4L", 11, true)
	lm:set_dir_light_intensity("LightInteriorS5", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, true)
	lm:activate_cinematic("cinematic_first_door", true)
	lm:stop_emiter("hojas_estatua", true)	
	lm:start_emiter("hojas_puente", true)
end

function OnTriggerExitTSWindstrikeEscaleras1()
	--lm:render_only_shadows("ParedTapada4", false)
	--lm:render_everything("ParedDestapada4", false)
	--lm:render_only_shadows("ParedDestapada5", false)
	lm:render_everything("ParedTapada5", false)	
	lm:activate_cinematic("cinematic_first_door", false)	

	lm:render_everything("ParedTapada4", true)
	--lm:render_only_shadows("ParedDestapada4", true)
	--lm:render_everything("ParedDestapada5", true)
	--lm:render_only_shadows("ParedTapada5", true)
	lm:deactivate_cinematic("cinematic_first_door", true)
end		

function OnTriggerExitTSEscaleras1Escaleras2()
	--lm:render_everything("ParedDestapada5", false)
	--lm:render_only_shadows("ParedTapada5", false)
	lm:set_dir_light_intensity("LightDir03", 10, false)
	lm:set_dir_light_intensity("LightDir04", 0, false)
	lm:set_dir_light_intensity("LightInteriorS4", 1, false)
	lm:set_dir_light_intensity("LightInteriorS4L", 11, false)
	lm:set_dir_light_intensity("LightInteriorS5", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, false)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_interior_sound(false)

	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada5", true)
	lm:render_everything("ParedTapada5", true)
	lm:set_dir_light_intensity("LightDir03", 0, true)
	lm:set_dir_light_intensity("LightDir04", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, true)
	lm:set_dir_light_intensity("LightInteriorS4", 0, true)
	lm:set_dir_light_intensity("LightInteriorS4L", 0, true)
	lm:set_dir_light_intensity("LightInteriorS5", 0, true)	
	lm:activate_cinematic("cinematic_sala_escaleras_2", true)
end

function OnTriggerExitTSEscaleras2Pasillo()
	--lm:render_only_shadows("ParedDestapada7", false)
	lm:render_everything("ParedTapada7", false)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, false)
	lm:set_dir_light_intensity("LightDir04", 11, false)
	lm:set_dir_light_intensity("LightDir05", 0, false)
	lm:set_dir_light_intensity("LightDir06", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, false)
	lm:activate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_ambient_sound(false)
	lm:stop_emiter("hojas_jardin_1", false)

	lm:play_interior_sound(true)
	lm:render_everything("ParedDestapada7", true)
	--lm:render_only_shadows("ParedTapada7", true)
	lm:set_dir_light_intensity("LightDir04", 0, true)
	lm:set_dir_light_intensity("LightDir05", 11, true)
	lm:set_dir_light_intensity("LightDir06", 11, true)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, true)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", true)
	lm:start_emiter("hojas_jardin_1", true)
end

function OnTriggerExitTPTechosPuertas()
	--lm:render_everything("ParedDestapada7", false)
	--lm:render_only_shadows("ParedTapada7", false)
	lm:set_dir_light_intensity("LightDir05", 11, false)
	lm:set_dir_light_intensity("LightDir06", 11, false)
	lm:set_dir_light_intensity("LightDir07", 0, false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, false)
	lm:play_interior_sound(false)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, false)
	lm:start_emiter("hojas_puente", false)	
	
	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada7", true)
	lm:render_everything("ParedTapada7", true)
	lm:deactivate_cinematic("cinematic_vientos", true)
	lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, true)
	lm:set_dir_light_intensity("LightDir05", 0, true)
	lm:set_dir_light_intensity("LightDir06", 0, true)
	lm:set_dir_light_intensity("LightDir07", 11, true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 10, true)
	lm:stop_emiter("hojas_puente", true)
	lm:start_emiter("hojas_jardin_2", true)
	lm:start_emiter("hojas_viento", true)
end

function OnTriggerExitTSPuertasArbol()
	lm:set_point_light_intensity("Luces_Fuego_08", 10, false)
	lm:set_dir_light_intensity("LightDir07", 11, false)
	lm:set_dir_light_intensity("LightDir09", 0, false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:play_interior_sound(false)
	lm:start_emiter("hojas_jardin_1", false)
	
	lm:play_ambient_sound(true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:set_dir_light_intensity("LightDir07", 0, true)
	lm:set_dir_light_intensity("LightDir09", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, true)
	lm:stop_emiter("hojas_jardin_1", true)
end

function OnTriggerExitTSArbolRunner()
	--lm:render_only_shadows("ParedDestapada10", true)
	lm:render_everything("ParedTapada10", true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_dir_light_intensity("LightDir09", 11, true)
	lm:set_dir_light_intensity("LightDir10", 0, true)
	lm:set_dir_light_intensity("LightDir11", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:play_ambient_sound(true)
	lm:start_emiter("hojas_jardin_2", true)

	lm:play_interior_sound(false)
	lm:render_everything("ParedDestapada10", false)
	--lm:render_only_shadows("ParedTapada10", false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	lm:set_dir_light_intensity("LightDir09", 0, false)
	lm:set_dir_light_intensity("LightDir10", 11, false)
	lm:set_dir_light_intensity("LightDir11", 11, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, false)
	lm:stop_emiter("hojas_jardin_2", false)
end

function OnTriggerExitTSRunnerCascada()
	--lm:render_everything("ParedDestapada10", true)
	--lm:render_only_shadows("ParedTapada10", true)
	lm:set_dir_light_intensity("LightDir12", 0, true)
	lm:set_dir_light_intensity("LightDir13", 0, true)
	lm:set_dir_light_intensity("LightDir10", 11, true)
	lm:set_dir_light_intensity("LightDir11", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, true)
	lm:play_interior_sound(true)
	lm:start_emiter("hojas_viento", true)
	lm:stop_emiter("particulas_top_cascada", true)

	
	lm:play_ambient_sound(false)
	--lm:render_only_shadows("ParedDestapada10", false)
	lm:render_everything("ParedTapada10", false)
	lm:set_dir_light_intensity("LightDir12", 11, false)
	lm:set_dir_light_intensity("LightDir13", 11, false)
	lm:set_dir_light_intensity("LightDir10", 0, false)
	lm:set_dir_light_intensity("LightDir11", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, false)
	lm:stop_emiter("hojas_viento", false)
	lm:start_emiter("particulas_top_cascada", false)
end

function OnTriggerExitTSCascadaAntorchas()
	--lm:render_only_shadows("ParedDestapada12", true)
	lm:render_everything("ParedTapada12", true)
	lm:set_dir_light_intensity("LightDir03", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_011", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_012", 0, true)
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 0, true)
	lm:play_ambient_sound(true)

	lm:play_interior_sound(false)
	lm:render_everything("ParedDestapada12", false)
	--lm:render_only_shadows("ParedTapada12", false)
	lm:change_level("level_2")
	lm:set_point_light_intensity("Luces_Fuego_011", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, false)
end


function OnTriggerEnterTSCascadaArriba()
	--lm:render_everything("ParedDestapada12", true)
	--lm:render_only_shadows("ParedTapada12", true)
	--lm:hide_mesh("ParedDestapada13", true)
	lm:unhide_mesh("ParedTapada13", true)
	lm:set_point_light_intensity("Luces_Fuego_011", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, true)
	lm:set_dir_light_intensity("LightInteriorSSalaFinal", 1, true)
	lm:play_ambient_sound(true)

	lm:play_interior_sound(false)
	--lm:render_only_shadows("ParedDestapada12", false)
	lm:render_everything("ParedTapada12", false)
	--lm:render_everything("ParedDestapada13", false)
	--lm:render_only_shadows("ParedTapada13", false)
end