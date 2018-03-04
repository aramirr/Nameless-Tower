cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		trigger_actor type: #string default: "The Player"
		destroy_time type: #float ui:destroy_time default:1
		recover_time type: #float ui:recover_time default:5
	)
	rollout params "Linear Plattform Parameters"
	( 
		spinner destroy_time "Destroy Time" type: #float
		spinner recover_time "Recotver Time" type: #float
		dropdownlist ddl_trigger_actor "Trigger Actor" items:#("The Player", "Boss Runner")
		
		fn select_dropdown_by_text ddl search_string =
		(
			for i = 1 to ddl.items.count do 
			(
				if  ddl.items[i] == search_string do 
				(
					 ddl.selection=i
					break
				)
			)
		)
		
		on params open do
		(
			select_dropdown_by_text ddl_trigger_actor trigger_actor
		)
		
		on ddl_trigger_actor selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#destroyable_comp].trigger_actor = ddl_trigger_actor.items[i]
		)
	)
)

struct destoyable_cattr
(
	public
	name = "destroyable_comp",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
		
	),
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "ai_destroyable" 
		fs.beginObj()
			fs.writeKeyValue "trigger_actor" mod_data.trigger_actor 
			fs.writeComma()
			fs.writeKeyValue "destroy_time" mod_data.destroy_time
			fs.writeComma()
			fs.writeKeyValue "recover_time" mod_data.recover_time
		fs.endObj()
	)
)

destoyable_cattr()