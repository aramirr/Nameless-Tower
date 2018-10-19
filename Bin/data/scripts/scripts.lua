
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
	lm:set_dir_light_intensity("LightDir00", 0, false)
	lm:set_dir_light_intensity("LightDir01", 0, false)
	lm:set_dir_light_intensity("LightDir02", 0, false)
	lm:set_dir_light_intensity("LightDir03", 0, false)	
	lm:set_dir_light_intensity("LightDir13", 10, false)	
	lm:set_dir_light_intensity("LightDir14", 10, false)	
	lm:set_point_light_intensity("Luces_Fuego_011", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, false)

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
	lm:play_positional_sound("activador_movil", "Activador_Movil03")
end

function OnWindstrikeActivateTAPuertasAbajo()
	lm:open_door("Puerta_Reja_005")
	lm:activate_anim("Activador_Movil02")
	lm:play_positional_sound("activador_movil", "Activador_Movil02")
	lm:play_positional_sound("puerta_reja", "Puerta_Reja_005")
end

function OnWindstrikeActivateTAPuertasArriba()
	lm:open_door("Puerta_Reja_04")
	lm:activate_anim("Activador_Movil04")
	lm:play_positional_sound("activador_movil", "Activador_Movil04")
	lm:play_positional_sound("puerta_reja", "Puerta_Reja_04")
end

function OnWindstrikeActivateTARunner()
	lm:open_door("Puerta_Reja_02")
	lm:activate_anim("Activador_Movil01")
	lm:play_positional_sound("activador_movil", "Activador_Movil01")
	lm:play_positional_sound("puerta_reja", "Puerta_Reja_02")
end

-- Puzzle antorchas
function OnTriggerExitTSRejaAntorchas()
	lm:activate_torch("TFAntorcha1")
	lm:activate_torch("TFAntorcha02")
	lm:activate_torch("TFAntorcha03")
	lm:close_door("Puerta_Reja_02")
	lm:scare_player()
end


-- Sala 1-2
function OnTriggerExitTSCabezaTutorial()
	lm:set_dir_light_intensity("LightDir02", 11, false)
	lm:start_emiter("hojas_estatua", false)


	lm:set_dir_light_intensity("LightDir02", 0, true)
	lm:stop_emiter("hojas_estatua", true)

	lm:insert_grass(5, false)
	lm:insert_grass(6, false)
	lm:insert_grass(7, false)
	lm:delete_grass(5, true)
	lm:delete_grass(6, true)
	lm:delete_grass(7, true)
	lm:update_grass_render()

end

-- Sala 2-3
function OnTriggerExitTPTutorialEstatua()
	lm:set_dir_light_intensity("LightDir00", 11, true)
	lm:start_emiter("hojas_inicio", false)
	lm:set_dir_light_intensity("LightDir03", 0, true)

	lm:set_dir_light_intensity("LightDir00", 0, false)
	lm:stop_emiter("hojas_inicio", true)
	lm:set_dir_light_intensity("LightDir03", 11, false)


	lm:insert_grass(0, true)
	lm:insert_grass(1, true)
	lm:insert_grass(2, true)
	lm:insert_grass(3, true)
	lm:delete_grass(0, false)
	lm:delete_grass(1, false)
	lm:delete_grass(2, false)
	lm:delete_grass(3, false)
	lm:update_grass_render()

end

-- Sala 3-4
function OnTriggerExitTSEstatuaWindstrike()

	--Exterior
	lm:set_exposure(0.9, false)
	lm:set_naja_ext(false)
	lm:render_everything("ParedTapada4", false)
	--lm:render_only_shadows("ParedDestapada4", false)	
	lm:set_dir_light_intensity("LightDir01", 11, false)
	--lm:set_dir_light_intensity("LightDir03", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4L", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS5", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, false)
	lm:deactivate_cinematic("cinematic_first_door", false)
	lm:play_ambient_sound(false)
	lm:start_emiter("hojas_estatua", false)	
	lm:stop_emiter("hojas_puente", false)

	lm:insert_grass(4, false)
	lm:insert_grass(5, false)
	lm:insert_grass(6, false)
	lm:insert_grass(7, false)

	--Interior
	lm:set_exposure(0.05, true)
	lm:play_interior_sound(true)
	lm:set_naja_int(true)
	lm:render_only_shadows("ParedTapada4", true)
	--lm:render_everything("ParedDestapada4", true)
	lm:set_dir_light_intensity("LightDir01", 0, true)
	--lm:set_dir_light_intensity("LightDir03", 11, true)
	--lm:set_dir_light_intensity("LightInteriorS4", 1, true)
	--lm:set_dir_light_intensity("LightInteriorS4L", 11, true)
	--lm:set_dir_light_intensity("LightInteriorS5", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, true)
	lm:activate_cinematic("cinematic_first_door", true)
	lm:stop_emiter("hojas_estatua", true)	
	lm:start_emiter("hojas_puente", true)

	lm:delete_grass(4, true)
	lm:delete_grass(5, true)
	lm:delete_grass(6, true)
	lm:delete_grass(7, true)

	--both
	lm:update_grass_render()
end

-- Sala 4-5
function OnTriggerExitTSWindstrikeEscaleras1()
	lm:render_only_shadows("ParedTapada4", false)
	--lm:render_everything("ParedDestapada4", false)
	--lm:render_only_shadows("ParedDestapada5", false)
	lm:render_everything("ParedTapada5", false)	
	lm:activate_cinematic("cinematic_first_door", false)	
	lm:set_dir_light_intensity("LightDir04", 0, false)
	lm:set_dir_light_intensity("LightDir02", 11, false)
	--lm:set_dir_light_intensity("LightDir03", 11, false)

	lm:render_everything("ParedTapada4", true)
	--lm:render_only_shadows("ParedDestapada4", true)
	--lm:render_everything("ParedDestapada5", true)
	lm:render_only_shadows("ParedTapada5", true)
	lm:deactivate_cinematic("cinematic_first_door", true)
	lm:set_dir_light_intensity("LightDir04", 11, true)
	lm:set_dir_light_intensity("LightDir02", 0, true)
	--lm:set_dir_light_intensity("LightDir03", 0, true)
end		

-- Sala 5-6
function OnTriggerExitTSEscaleras1Escaleras2()
	--Interior
	lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	--lm:render_everything("ParedDestapada5", false)
	lm:render_only_shadows("ParedTapada5", false)
	lm:set_dir_light_intensity("LightDir05", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4", 1, false)
	--lm:set_dir_light_intensity("LightInteriorS4L", 11, false)
	--lm:set_dir_light_intensity("LightInteriorS5", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, false)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_interior_sound(false)

	--Exterior
	lm:set_exposure(0.9, true)
	lm:set_naja_ext(true)
	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada5", true)
	lm:render_everything("ParedTapada5", true)
	lm:set_dir_light_intensity("LightDir05", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS4", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS4L", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS5", 0, true)	
	lm:activate_cinematic("cinematic_sala_escaleras_2", true)
end

-- Sala 6-7
function OnTriggerExitTSEscaleras2Pasillo()
	--Exterior
	lm:set_exposure(0.9, false)
	lm:set_naja_ext(false)
	--lm:render_only_shadows("ParedDestapada7", false)
	lm:render_everything("ParedTapada7", false)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, false)
	lm:set_dir_light_intensity("LightDir06", 0, false)
	lm:set_dir_light_intensity("LightDir07", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, false)
	lm:activate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_ambient_sound(false)
	lm:stop_emiter("hojas_jardin_1", false)

	--Interior
	lm:set_exposure(0.05, true)
	lm:set_naja_int(true)
	lm:play_interior_sound(true)
	--lm:render_everything("ParedDestapada7", true)
	lm:render_only_shadows("ParedTapada7", true)
	lm:set_dir_light_intensity("LightDir06", 11, true)
	lm:set_dir_light_intensity("LightDir07", 11, true)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, true)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", true)
	lm:start_emiter("hojas_jardin_1", true)
end

-- Mitad Pasillo Compresoras
function OnTriggerExitTSLucesCompresoras()
	lm:insert_grass(8, true)
	lm:insert_grass(9, true)
	lm:insert_grass(10, true)
	lm:insert_grass(11, true)

	lm:delete_grass(8, false)
	lm:delete_grass(9, false)
	lm:delete_grass(10, false)
	lm:delete_grass(11, false)
	lm:update_grass_render()
end	

-- Sala 7-8
function OnTriggerExitTPTechosPuertas()
	--Interior
	lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	--lm:render_everything("ParedDestapada7", false)
	lm:render_only_shadows("ParedTapada7", false)
	lm:set_dir_light_intensity("LightDir04", 11, false)
	lm:set_dir_light_intensity("LightDir05", 11, false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, false)
	lm:play_interior_sound(false)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, false)
	lm:start_emiter("hojas_puente", false)	

	lm:delete_grass(12, false)
	lm:delete_grass(13, false)
	lm:delete_grass(14, false)
	
	--Exterior
	lm:set_exposure(0.9, true)
	lm:set_naja_ext(true)
	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada7", true)
	lm:render_everything("ParedTapada7", true)
	lm:deactivate_cinematic("cinematic_vientos", true)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, true)
	lm:set_dir_light_intensity("LightDir04", 0, true)
	lm:set_dir_light_intensity("LightDir05", 0, true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_dir_light_intensity("LightDir09", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 10, true)
	lm:stop_emiter("hojas_puente", true)
	lm:start_emiter("hojas_jardin_2", true)
	lm:start_emiter("hojas_viento", true)

	lm:insert_grass(12, true)
	lm:insert_grass(13, true)
	lm:insert_grass(14, true)

	--both
	lm:update_grass_render()
end

-- Sala 8-9
function OnTriggerExitTSPuertasArbol()
	lm:set_point_light_intensity("Luces_Fuego_08", 10, false)
	lm:set_dir_light_intensity("LightDir06", 11, false)
	lm:set_dir_light_intensity("LightDir07", 11, false)
	lm:set_dir_light_intensity("LightDir10", 0, false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:play_interior_sound(false)
	lm:start_emiter("hojas_jardin_1", false)

	lm:insert_grass(8, false)
	lm:insert_grass(9, false)
	lm:insert_grass(10, false)
	lm:insert_grass(11, false)
	

	lm:play_ambient_sound(true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:set_dir_light_intensity("LightDir06", 0, true)
	lm:set_dir_light_intensity("LightDir07", 0, true)
	lm:set_dir_light_intensity("LightDir10", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, true)
	lm:stop_emiter("hojas_jardin_1", true)

	lm:delete_grass(8, true)
	lm:delete_grass(9, true)
	lm:delete_grass(10, true)
	lm:delete_grass(11, true)

	lm:update_grass_render()
end

-- Sala 9-10
function OnTriggerExitTSArbolRunner()
	--Exterior
	lm:set_exposure(0.9, true)
	lm:set_naja_ext(true)
	--lm:render_only_shadows("ParedDestapada10", true)
	lm:render_everything("ParedTapada10", true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_dir_light_intensity("LightDir11", 0, true)
	lm:set_dir_light_intensity("LightDir12", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:play_ambient_sound(true)
	lm:start_emiter("hojas_jardin_2", true)

	lm:insert_grass(12, true)
	lm:insert_grass(13, true)
	lm:insert_grass(14, true)
	lm:delete_grass(15, true)

	--Interior
	lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	lm:play_interior_sound(false)
	--lm:render_everything("ParedDestapada10", false)
	lm:render_only_shadows("ParedTapada10", false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	lm:set_dir_light_intensity("LightDir11", 11, false)
	lm:set_dir_light_intensity("LightDir12", 11, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, false)
	lm:stop_emiter("hojas_jardin_2", false)

	lm:delete_grass(12, false)
	lm:delete_grass(13, false)
	lm:delete_grass(14, false)
	lm:insert_grass(15, false)

	--both
	lm:update_grass_render()
end

function OnTriggerExitTSRunnerCascada()
	--Interior
	lm:set_exposure(0.05, true)
	lm:set_naja_int(true)
	--lm:render_everything("ParedDestapada10", true)
	lm:render_only_shadows("ParedTapada10", true)
	lm:set_dir_light_intensity("LightDir13", 0, true)
	lm:set_dir_light_intensity("LightDir14", 0, true)
	lm:set_dir_light_intensity("LightDir09", 10, true)
	lm:set_dir_light_intensity("LightDir10", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, true)
	lm:play_interior_sound(true)
	lm:start_emiter("hojas_viento", true)
	lm:stop_emiter("particulas_top_cascada", true)
	--lm:stop_sound(true, "cascade")

	lm:delete_grass(16, true)
	lm:delete_grass(17, true)

	--Exterior
	lm:set_exposure(0.9, false)
	lm:set_naja_ext(false)
	lm:play_ambient_sound(false)
	--lm:play_sound(false, "cascade")
	--lm:render_only_shadows("ParedDestapada10", false)
	lm:render_everything("ParedTapada10", false)
	lm:set_dir_light_intensity("LightDir13", 10, false)
	lm:set_dir_light_intensity("LightDir14", 10, false)
	lm:set_dir_light_intensity("LightDir09", 0, false)
	lm:set_dir_light_intensity("LightDir10", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, false)
	lm:stop_emiter("hojas_viento", false)
	lm:start_emiter("particulas_top_cascada", false)

	lm:insert_grass(16, false)
	lm:insert_grass(17, false)

	--both
	lm:update_grass_render()
end

function OnTriggerExitTSCascadaAntorchas()
	--lm:render_only_shadows("ParedDestapada12", true)
	lm:render_everything("ParedTapada12", true)
	lm:set_dir_light_intensity("LightDir11", 11, true)
	lm:set_dir_light_intensity("LightDir12", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_011", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_012", 0, true)
	--lm:set_dir_light_intensity("LightInteriorSSalaFinal", 0, true)
	lm:play_ambient_sound(true)
	--lm:play_sound(true, "cascade")

	lm:insert_grass(15, true)
	lm:insert_grass(16, true)
	lm:insert_grass(17, true)

	
	--lm:stop_sound(false, "cascade")
	lm:play_interior_sound(false)
	--lm:render_everything("ParedDestapada12", false)
	lm:render_only_shadows("ParedTapada12", false)
	lm:change_level("level_2")
	lm:set_dir_light_intensity("LightDir11", 0, false)
	lm:set_dir_light_intensity("LightDir12", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_011", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, false)

	lm:delete_grass(15, false)
	lm:delete_grass(16, false)
	lm:delete_grass(17, false)
	lm:update_grass_render()
end


function OnTriggerExitLuaCinematicTest()
	--lm:activate_cinematic_bands(false)
	--lm:pause_player()
	--If camera needs to be moved
	--lm:activate_cinematic("cinematic_first_door", false)
	--lm:play_animation(32)
	--lm:regain_control(25)
	--lm:kill_entity("LuaCinematicTest")
	lm:activate_cinematic_bands(false)
	lm:activate_cinematic("cinematica_inicial", false)
	lm:pause_player()
	lm:play_animation(32)
	lm:regain_control(25)
end
-- NIVEL BAJADA


-- Sala 3-4
function OnTriggerExitTSEstatuaWindstrikeB()

	--Exterior
	--lm:set_exposure(0.9, false)
	lm:set_naja_ext(false)
	lm:render_everything("ParedTapada4B", false)
	--lm:render_only_shadows("ParedDestapada4", false)	
	lm:set_dir_light_intensity("LightDir01", 11, false)
	lm:set_dir_light_intensity("LightDir03", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4L", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS5", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, false)
	lm:deactivate_cinematic("cinematic_first_door", false)
	lm:play_ambient_sound(false)
	--lm:start_emiter("hojas_estatua", false)	
	--lm:stop_emiter("hojas_puente", false)

	--Interior
	--lm:set_exposure(0.05, true)
	lm:play_interior_sound(true)
	lm:set_naja_int(true)
	lm:render_only_shadows("ParedTapada4B", true)
	--lm:render_everything("ParedDestapada4", true)
	lm:set_dir_light_intensity("LightDir01", 0, true)
	lm:set_dir_light_intensity("LightDir03", 11, true)
	--lm:set_dir_light_intensity("LightInteriorS4", 1, true)
	--lm:set_dir_light_intensity("LightInteriorS4L", 11, true)
	--lm:set_dir_light_intensity("LightInteriorS5", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, true)
	lm:activate_cinematic("cinematic_first_door", true)
	--lm:stop_emiter("hojas_estatua", true)	
	--lm:start_emiter("hojas_puente", true)
end

-- Sala 4-5
function OnTriggerExitTSWindstrikeEscaleras1B()
	lm:render_only_shadows("ParedTapada4B", false)
	--lm:render_everything("ParedDestapada4", false)
	--lm:render_only_shadows("ParedDestapada5", false)
	lm:render_everything("ParedTapada5B", false)	
	lm:activate_cinematic("cinematic_first_door", false)	
	lm:set_dir_light_intensity("LightDir04", 0, false)
	lm:set_dir_light_intensity("LightDir02", 11, false)

	lm:render_everything("ParedTapada4B", true)
	--lm:render_only_shadows("ParedDestapada4", true)
	--lm:render_everything("ParedDestapada5", true)
	lm:render_only_shadows("ParedTapada5B", true)
	lm:deactivate_cinematic("cinematic_first_door", true)
	lm:set_dir_light_intensity("LightDir04", 11, true)
	lm:set_dir_light_intensity("LightDir02", 0, true)
end		

-- Sala 5-6
function OnTriggerExitTSEscaleras1Escaleras2B()
	--Interior
	--lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	--lm:render_everything("ParedDestapada5", false)
	lm:render_only_shadows("ParedTapada5B", false)
	lm:set_dir_light_intensity("LightDir05", 0, false)
	--lm:set_dir_light_intensity("LightInteriorS4", 1, false)
	--lm:set_dir_light_intensity("LightInteriorS4L", 11, false)
	--lm:set_dir_light_intensity("LightInteriorS5", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_01", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_02", 10, false)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_interior_sound(false)

	--Exterior
	--lm:set_exposure(0.4, true)
	lm:set_naja_ext(true)
	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada5", true)
	lm:render_everything("ParedTapada5B", true)
	lm:set_dir_light_intensity("LightDir05", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_01", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_02", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS4", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS4L", 0, true)
	--lm:set_dir_light_intensity("LightInteriorS5", 0, true)	
	lm:activate_cinematic("cinematic_sala_escaleras_2", true)
end

-- Sala 6-7
function OnTriggerExitTSEscaleras2PasilloB()
	--Exterior
	--lm:set_exposure(0.4, false)
	lm:set_naja_ext(false)
	--lm:render_only_shadows("ParedDestapada7", false)
	lm:render_everything("ParedTapada7B", false)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, false)
	lm:set_dir_light_intensity("LightDir06", 0, false)
	lm:set_dir_light_intensity("LightDir07", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, false)
	lm:activate_cinematic("cinematic_sala_escaleras_2", false)
	lm:play_ambient_sound(false)
	--lm:stop_emiter("hojas_jardin_1", false)

	--Interior
	--lm:set_exposure(0.05, true)
	lm:set_naja_int(true)
	lm:play_interior_sound(true)
	--lm:render_everything("ParedDestapada7", true)
	lm:render_only_shadows("ParedTapada7B", true)
	lm:set_dir_light_intensity("LightDir06", 11, true)
	lm:set_dir_light_intensity("LightDir07", 11, true)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, true)
	lm:deactivate_cinematic("cinematic_sala_escaleras_2", true)
	--lm:start_emiter("hojas_jardin_1", true)
end

-- Sala 7-8
function OnTriggerExitTPTechosPuertasB()
	--Interior
	--lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	--lm:render_everything("ParedDestapada7", false)
	lm:render_only_shadows("ParedTapada7B", false)
	lm:set_dir_light_intensity("LightDir04", 11, false)
	lm:set_dir_light_intensity("LightDir05", 11, false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 1, false)
	lm:set_point_light_intensity("Luces_Fuego_03", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_04", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_05", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_06", 10, false)
	lm:play_interior_sound(false)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, false)
	--lm:start_emiter("hojas_puente", false)	
	
	--Exterior
	--lm:set_exposure(0.4, true)
	lm:set_naja_ext(true)
	lm:play_ambient_sound(true)
	--lm:render_only_shadows("ParedDestapada7", true)
	lm:render_everything("ParedTapada7B", true)
	lm:deactivate_cinematic("cinematic_vientos", true)
	--lm:set_dir_light_intensity("LightInteriorSCompresoras", 0, true)
	lm:set_dir_light_intensity("LightDir04", 0, true)
	lm:set_dir_light_intensity("LightDir05", 0, true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_dir_light_intensity("LightDir09", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_03", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_04", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_05", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_06", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 10, true)
	--lm:stop_emiter("hojas_puente", true)
	--lm:start_emiter("hojas_jardin_2", true)
	--lm:start_emiter("hojas_viento", true)
end

-- Sala 8-9
function OnTriggerExitTSPuertasArbolB()
	lm:set_point_light_intensity("Luces_Fuego_08", 10, false)
	lm:set_dir_light_intensity("LightDir06", 11, false)
	lm:set_dir_light_intensity("LightDir07", 11, false)
	lm:set_dir_light_intensity("LightDir10", 0, false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:play_interior_sound(false)
	--lm:start_emiter("hojas_jardin_1", false)
	
	lm:play_ambient_sound(true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:set_dir_light_intensity("LightDir06", 0, true)
	lm:set_dir_light_intensity("LightDir07", 0, true)
	lm:set_dir_light_intensity("LightDir10", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_08", 0, true)
	--lm:stop_emiter("hojas_jardin_1", true)
end

-- Sala 9-10
function OnTriggerExitTSJardinRunner()
	--Exterior
	--lm:set_exposure(0.4, true)
	lm:set_naja_ext(true)
	--lm:render_only_shadows("ParedDestapada10", true)
	lm:render_everything("ParedTapada10B", true)
	lm:set_dir_light_intensity("LightDir08", 11, true)
	lm:set_dir_light_intensity("LightDir11", 0, true)
	lm:set_dir_light_intensity("LightDir12", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, true)
	lm:activate_cinematic("cinematic_vientos", true)
	lm:play_ambient_sound(true)
	--lm:start_emiter("hojas_jardin_2", true)

	--Interior
	--lm:set_exposure(0.05, false)
	lm:set_naja_int(false)
	lm:play_interior_sound(false)
	--lm:render_everything("ParedDestapada10", false)
	lm:render_only_shadows("ParedTapada10B", false)
	lm:deactivate_cinematic("cinematic_vientos", false)
	lm:set_dir_light_intensity("LightDir08", 0, false)
	lm:set_dir_light_intensity("LightDir11", 11, false)
	lm:set_dir_light_intensity("LightDir12", 11, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, false)
	--lm:stop_emiter("hojas_jardin_2", false)
end

function OnTriggerExitTSRunnerLava()
	--Interior
	--lm:set_exposure(0.05, true)
	lm:set_naja_int(true)
	--lm:render_everything("ParedDestapada10", true)
	lm:render_only_shadows("ParedTapada10B", true)
	lm:set_dir_light_intensity("LightDir13", 0, true)
	lm:set_dir_light_intensity("LightDir14", 0, true)
	lm:set_dir_light_intensity("LightDir09", 10, true)
	lm:set_dir_light_intensity("LightDir10", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_07", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_09", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_010", 10, true)
	lm:play_interior_sound(true)
	--lm:start_emiter("hojas_viento", true)
	--lm:stop_emiter("particulas_top_cascada", true)
	lm:stop_sound(true, "lava")

	--Exterior
	--lm:set_exposure(0.4, false)
	lm:set_naja_ext(false)
	lm:play_ambient_sound(false)
	lm:play_sound(false, "lava")
	--lm:render_only_shadows("ParedDestapada10", false)
	lm:render_everything("ParedTapada10B", false)
	lm:set_dir_light_intensity("LightDir13", 10, false)
	lm:set_dir_light_intensity("LightDir14", 10, false)
	lm:set_dir_light_intensity("LightDir09", 0, false)
	lm:set_dir_light_intensity("LightDir10", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_07", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_09", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_010", 0, false)
	--lm:stop_emiter("hojas_viento", false)
	--lm:start_emiter("particulas_top_cascada", false)
end

function OnTriggerExitTSCascadaAntorchasB()
	--lm:render_only_shadows("ParedDestapada12", true)
	lm:render_everything("ParedTapada12B", true)
	lm:set_dir_light_intensity("LightDir11", 11, true)
	lm:set_dir_light_intensity("LightDir12", 11, true)
	lm:set_point_light_intensity("Luces_Fuego_011", 0, true)
	lm:set_point_light_intensity("Luces_Fuego_012", 0, true)
	--lm:set_dir_light_intensity("LightInteriorSSalaFinal", 0, true)
	lm:play_ambient_sound(true)
	lm:play_sound(true, "lava")

	
	lm:stop_sound(false, "lava")
	lm:play_interior_sound(false)
	--lm:render_everything("ParedDestapada12", false)
	lm:render_only_shadows("ParedTapada12B", false)
	lm:change_level("level_2")
	lm:set_dir_light_intensity("LightDir11", 0, false)
	lm:set_dir_light_intensity("LightDir12", 0, false)
	lm:set_point_light_intensity("Luces_Fuego_011", 10, false)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, false)
end


function OnTriggerEnterTSCascadaArribaB()
	--lm:render_everything("ParedDestapada12", true)
	lm:render_only_shadows("ParedTapada12B", true)
	--lm:hide_mesh("ParedDestapada13", true)
	lm:unhide_mesh("ParedTapada13B", true)
	lm:set_point_light_intensity("Luces_Fuego_011", 10, true)
	lm:set_point_light_intensity("Luces_Fuego_012", 10, true)
	--lm:set_dir_light_intensity("LightInteriorSSalaFinal", 1, true)
	lm:play_ambient_sound(true)

	lm:play_interior_sound(false)
	--lm:render_only_shadows("ParedDestapada12", false)
	lm:render_everything("ParedTapada12B", false)
	--lm:render_everything("ParedDestapada13", false)
	lm:render_only_shadows("ParedTapada13B", false)
end