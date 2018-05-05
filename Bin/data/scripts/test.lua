
print('This is lua')
SLB.using(SLB)

print(LogicManager)

lm = LogicManager();

lm:printdbg()


val=lm:GetPlayerLife();

print(val);

lm:RespawnPlayer();

val=lm:GetPlayerLife();

print(val);


lm:TeleportPlayer(1,2,3);


print('Numero de agentes');
print(lm.numagents);



function OnTriggerEnterSierra()
lm:printdbg()
lm:printdbg()
lm:printdbg()
end
