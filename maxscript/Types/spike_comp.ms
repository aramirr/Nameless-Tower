cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		is_fixed type: #string default: "false"
		time_to_trigger type:#float ui:spn_time_to_trigger default:1.0
		time_to_deactivate type:#float ui:spn_time_to_deactivate default:1.0
		
	)
	rollout params "Spike Component Parameters"
	( 
		dropdownlist ddl_is_fixed "Fixed Spikes" items:#("false","true")
		spinner spn_time_to_trigger "Time to Trigger" type:#float
		spinner spn_time_to_deactivate "Time to Deactivate" type:#float
		
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
		
		on ddl_is_fixed selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#spikes_comp].is_fixed = ddl_is_fixed.items[i]
			if ddl_is_fixed.items[i] == "true" then
			(
				spn_time_to_trigger.enabled = false
				spn_time_to_deactivate.enabled = false
			)
			else if ddl_is_fixed.items[i] == "false" then
			(
				spn_time_to_trigger.enabled = true
				spn_time_to_deactivate.enabled = true
			)
		)
		
		on params open do
		(
			select_dropdown_by_text ddl_is_fixed is_fixed
		)
	)
)

struct spikes_comp_cattr
(
	public
	name = "spikes_comp",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
	),
	
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "ai_spikes" 
		fs.beginObj()
			fs.writeKeyValue "fixed_spikes" (mod_data.is_fixed as booleanClass) 
			if ((mod_data.is_fixed as booleanClass) == false) then
			(
				fs.writeComma()
				fs.writeKeyValue "time_to_trigger" mod_data.time_to_trigger
				fs.writeComma()
				fs.writeKeyValue "time_to_return_to_off" mod_data.time_to_deactivate
			)
		fs.endObj()
	)
)

spikes_comp_cattr()