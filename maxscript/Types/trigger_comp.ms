cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		trigger_type type: #string default: "runner_appear"
		collider_entity type: #string default: "none"
		appearing_position type: #string default:""
		other_collider type:#string default:"none"
		has_render type:#string default:"true"
		b_appear_bool type:#string default:"true"
		object_position type:#string default:""
	)
	rollout params "Trigger Component Parameters"
	( 
		fn col_filt obj = 
		(
			found = false
			for _mod in obj.modifiers do
			(
				mod_parts = filterstring _mod.name "_"
				if mod_parts[mod_parts.count] == "collider" then
				(
					found=true
					return found
				)
			)
			return found
		)
		
		dropdownlist ddl_trigger_type "Trigger type" items:#("runner_appear","runner_stop", "checkpoint", "plattform", "spikes")
		checkbutton btn_create_aux_pos "Create Position Selector"
		button btn_set_aux_pos "Set Position From Helper"
		editText edt_pos readOnly:true
		dropdownlist ddl_b_appear "bAppear Enabled" items:#("true", "false")
		label lbl_external "External Collider"
		pickbutton pck_external_collider filter:col_filt width:100
		dropdownlist ddl_has_render "Render Enabled" items:#("true", "false")
		
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
		
		on pck_external_collider picked obj do
		(
			if obj != undefined do
			(
				pck_external_collider.text = obj.name
				other_collider = obj.name
			)
		)
		
		on pck_external_collider rightclick do 
		(
				if other_collider == "none" then
				(
					return()
				)
				else
				(
					other_collider = "none"
					pck_external_collider.text = ""
				)
		)
		
		on ddl_has_render selected i do 
		(
			has_render = ddl_has_render.items[i]
		)
		
		on ddl_b_appear selected i do 
		(
			b_appear_bool = ddl_b_appear.items[i]
		)
		
		on ddl_trigger_type selected i do
		(
			trigger_type = ddl_trigger_type.items[i]
			if (ddl_trigger_type.items[i] == "runner_appear") or  (ddl_trigger_type.items[i] == "runner_stop") or (ddl_trigger_type.items[i] == "checkpoint") then
			(
				btn_create_aux_pos.enabled = true
				btn_set_aux_pos.enabled = true
				if (ddl_trigger_type.items[i] == "runner_appear") then
				(
					ddl_b_appear.enabled = true
				)
				else
				(
					select_dropdown_by_text ddl_b_appear "true"
					ddl_b_appear.enabled = false
				)
			)
			else 
			(
				btn_create_aux_pos.enabled = false
				btn_set_aux_pos.enabled = false
				edt_pos.text = ""
				select_dropdown_by_text ddl_b_appear "true"
				ddl_b_appear.enabled = false
			)
		)
		
		on btn_create_aux_pos changed state do
		(
			if state == on then
			(
				curr_mesh = $
				aux_obj_name = curr_mesh.name + "_auxObj"
				aux_obj = getNodeByName aux_obj_name
				if aux_obj == undefined then
				(
					aux_obj =point()
					aux_obj.name = aux_obj_name
					if appearing_position != "" then
					(
						parts = filterString appearing_position " || "
						pos = Point3 
						pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
						aux_obj.pos = pos
					)
				)
			)
			else
			(
				curr_mesh = $
				aux_obj_name = curr_mesh.name + "_auxObj"
				aux_obj = getNodeByName aux_obj_name
				if aux_obj != undefined then
				(
					delete aux_obj
				)
			)
		)
		
		on btn_set_aux_pos pressed do
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj == undefined then
			(
				messageBox "Inexistent Center, run Create Position Selector Command"
				return()
			)
			pos_as_string = aux_obj.pos.x as string + " || " + aux_obj.pos.y as string + " || " +aux_obj.pos.z as string
			edt_pos.text = pos_as_string
			appearing_position = pos_as_string
		)
		
		on params open do
		(
			current_mesh = $
			if other_collider != "none" then
			(
				pck_external_collider.text = other_collider
			)
			
			select_dropdown_by_text ddl_trigger_type trigger_type
			select_dropdown_by_text ddl_has_render has_render
			select_dropdown_by_text ddl_b_appear b_appear_bool
			
			if appearing_position != "" then
			(
				edt_pos.text = appearing_position
			)
			
			if (getNodeByName (current_mesh.name + "_auxObj")) != undefined then
			(
				btn_create_aux_pos.checked=true
			)
			
			object_position = current_mesh.pos.x as string + " || " + current_mesh.pos.y as string + " || " +current_mesh.pos.z as string
		)
	)
)

struct trigger_comp_cattr
(
	public
	name = "trigger_comp",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
	),
	
	fn get_str_pos_as_mcv mod_data=
	(
		if mod_data.appearing_position != "" then
		(
			max2mcv = rotateXMatrix -90
			parts = filterString mod_data.appearing_position " || "
			pos = Point3 
			pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
			final_pos = (pos * max2mcv)
			return (final_pos[1] as string + " " + final_pos[2] as string + " " + final_pos[3] as string)
		)
		else
		(
			max2mcv = rotateXMatrix -90
			parts = filterString mod_data.object_position " || "
			pos = Point3 
			pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
			final_pos = (pos * max2mcv)
			return (final_pos[1] as string + " " + final_pos[2] as string + " " + final_pos[3] as string)
		)
	),
	
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "trigger" 
		fs.beginObj()
		fs.writeKeyValue "trigger_type" mod_data.trigger_type
		if ((mod_data.trigger_type =="runner_appear") or (mod_data.trigger_type =="runner_stop") or (mod_data.trigger_type =="checkpoint") ) then
		(
			if (mod_data.trigger_type =="runner_appear") then
			(
				fs.writeComma()
				fs.writeKeyValue "b_appear" (mod_data.b_appear_bool as booleanClass)
			)
			fs.writeComma()
			fs.writeKeyValue "appearing_position" (get_str_pos_as_mcv mod_data)
		)
		fs.writeComma()
		fs.writeKeyValue "collider_entity" mod_data.other_collider
		fs.writeComma()
		fs.writeKeyValue "render" (mod_data.has_render as booleanClass)
		fs.endObj()
	)
)

trigger_comp_cattr()