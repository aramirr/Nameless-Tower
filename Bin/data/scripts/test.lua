
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

function OnTriggerEnterSierra()
	lm:printdbg()
	lm:printdbg()
	lm:printdbg()
end

function OnTriggerEnterRoom1()
	lm:hide_mesh("wall_room_1")
end