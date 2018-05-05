
print('This is lua')
SLB.using(SLB)

lm = LogicManager();

function OnTriggerEnterSierra()
	lm:printdbg("joselito")
end

function OnTriggerEnterRoom1()
	val1, val2, val3 = lm:test_vec3()
end