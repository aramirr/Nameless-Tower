cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		radius type: #float ui:radius default:10
		height type: #float ui:height default:10
		
		is_dynamic type: #string default: "false"
		is_trigger type: #string default: "false"
	)
	rollout params "Capsule Collider Parameters"
	( 
		spinner radius "Radius" type: #float
		spinner height "Height" type: #float
		checkbutton chbtn_create "CreateTemp"
		button center_command "Center"
		dropdownlist ddl_is_dynamic "is dynamic" items:#("false","true")
		dropdownlist ddl_is_trigger "is trigger" items:#("false","true")
		
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
			current_mesh = $
			
			if (getNodeByName (current_mesh.name + "_auxObj")) != undefined then
			(
				chbtn_create.checked=true
			)
			
			select_dropdown_by_text ddl_is_dynamic is_dynamic
			select_dropdown_by_text ddl_is_trigger is_trigger
		)
		
		on chbtn_create changed state do
		(
			if state == on then
			(
				curr_mesh = $
				aux_obj_name = curr_mesh.name + "_auxObj"
				aux_obj = getNodeByName aux_obj_name
				if aux_obj == undefined then
				(
					aux_obj =Capsule()
					aux_obj.name = aux_obj_name
					aux_obj.boxmode = on
				)
				aux_obj.center = curr_mesh.center
				aux_obj.transform = curr_mesh.transform
				
				paramWire.connect curr_mesh.modifiers[#capsule_collider].attributeTypeData[#radius] aux_obj.baseObject[#Radius] "radius"
				paramWire.connect curr_mesh.modifiers[#capsule_collider].attributeTypeData[#height] aux_obj.baseObject[#Height] "height"
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
		
		on center_command pressed do
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj != undefined then
			(
				aux_obj.center = curr_mesh.center
			)
			aux_obj.transform = curr_mesh.transform
		)
		
		on ddl_is_dynamic selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#capsule_collider].is_dynamic = ddl_is_dynamic.items[i]
		)
		
		on ddl_is_trigger selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#capsule_collider].is_trigger = ddl_is_trigger.items[i]
		)
	 )
)

struct capsule_collider_cattr
(
	public
	name = "capsule_collider",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
		
	),
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "collider" 
		fs.beginObj()
			fs.writeKeyValue "shape" "capsule"
			fs.writeComma()
			fs.writeKeyValue "radius" mod_data.radius
			fs.writeComma()
			fs.writeKeyValue "height" mod_data.height
			fs.writeComma()
			fs.writeKeyValue "is_dynamic" (mod_data.is_dynamic as booleanClass)
			fs.writeComma()
			fs.writeKeyValue "is_trigger" (mod_data.is_trigger as booleanClass)
		fs.endObj()
	)
)

capsule_collider_cattr()