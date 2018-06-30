
print('This is lua')
SLB.using(SLB)

lm = LogicManager();


function OnLevel1Start()
	-- body
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
	-- body
end

-- Sala 3
function OnTriggerEnterTSEstatuaAbajo()
	-- body
end

function OnTriggerEnterTSEstatuaArriba()
	lm:unhide_mesh("ParedTapada4")
	lm:hide_mesh("ParedDestapada4")
end

-- Sala 4
function OnTriggerEnterTSWindstrikeDer()
	lm:hide_mesh("ParedTapada4")
	lm:unhide_mesh("ParedDestapada4")
end

function OnTriggerEnterTSWindstrikeIzq()
	lm:hide_mesh("ParedTapada4")
	lm:unhide_mesh("ParedDestapada4")

	lm:hide_mesh("ParedDestapada5")
	lm:unhide_mesh("ParedTapada5")
end

-- Sala 5
function OnTriggerEnterTSEscaleras1Izq()
	lm:unhide_mesh("ParedTapada4")
	lm:hide_mesh("ParedDestapada4")

	lm:unhide_mesh("ParedDestapada5")
	lm:hide_mesh("ParedTapada5")
end

function OnTriggerEnterTSEscaleras1Der()
	lm:unhide_mesh("ParedDestapada5")
	lm:hide_mesh("ParedTapada5")
end

-- Sala 6
function OnTriggerEnterTSEscaleras2Izq()
	lm:hide_mesh("ParedDestapada5")
	lm:unhide_mesh("ParedTapada5")
end

function OnTriggerEnterTSEscaleras2Der()
	lm:hide_mesh("ParedDestapada7")
	lm:unhide_mesh("ParedTapada7")
end

-- Sala 7
function OnTriggerEnterTPTechosIzq()
	lm:unhide_mesh("ParedDestapada7")
	lm:hide_mesh("ParedTapada7")
end

function OnTriggerEnterTPTechosDer()
	lm:unhide_mesh("ParedDestapada7")
	lm:hide_mesh("ParedTapada7")
end

-- Sala 8
function OnTriggerEnterTSPuertasIzq()
	lm:hide_mesh("ParedDestapada7")
	lm:unhide_mesh("ParedTapada7")
end

function OnTriggerEnterTSPuertasZoom()
	-- body
end

function OnTriggerEnterTSPuertasDer()
	-- body
end

-- Sala 9
function OnTriggerEnterTSArbolAbajo()
	-- body
end

function OnTriggerEnterTSArbolArriba()
	lm:hide_mesh("ParedDestapada10")
	lm:unhide_mesh("ParedTapada10")
end

-- Sala 10
function OnTriggerEnterTSRunnerIzq()
	lm:unhide_mesh("ParedDestapada10")
	lm:hide_mesh("ParedTapada10")
end

function OnTriggerEnterTSRunnerDer()
	lm:unhide_mesh("ParedDestapada10")
	lm:hide_mesh("ParedTapada10")
end

-- Sala 11
function OnTriggerEnterTSCascadaAbajo()
	lm:hide_mesh("ParedDestapada10")
	lm:unhide_mesh("ParedTapada10")
end

function OnTriggerEnterTSCascadaArriba()
	lm:hide_mesh("ParedDestapada12")
	lm:unhide_mesh("ParedTapada12")
end

-- Sala 12
function OnTriggerEnterTSAntorchasIzq()
	lm:unhide_mesh("ParedDestapada12")
	lm:hide_mesh("ParedTapada12")
end

function OnTriggerEnterTSAntorchasDer()
	lm:unhide_mesh("ParedDestapada12")
	lm:hide_mesh("ParedTapada12")

	lm:hide_mesh("ParedDestapada13")
	lm:unhide_mesh("ParedTapada13")
end

-- Sala 13
function OnTriggerEnterTTSalidaAbajo()
	lm:hide_mesh("ParedDestapada12")
	lm:unhide_mesh("ParedTapada12")

	lm:unhide_mesh("ParedDestapada13")
	lm:hide_mesh("ParedTapada13")
end

function OnTriggerEnterTTSalidaArriba()
	-- body
end

function OnTriggerEnterCinematica()
	lm:play_last_cinematic()
end