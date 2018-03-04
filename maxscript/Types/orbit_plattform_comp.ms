cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		waypoints type:#stringTab tabSize:0 tabSizeVariable:true
		rotations type:#stringTab tabSize:0 tabSizeVariable:true
		plattform_speed type: #float ui:p_speed default:0.05
		plattform_delay type: #float ui:p_delay default:1.0
		center type:#string default:""
	)
	rollout params "Orbit Patrol Parameters"
	( 
		spinner p_speed "Speed" type: #float
		spinner p_delay "Delay" type: #float
		
		button add_position_btn "Add Waypoints"
		dropdownlist ddl_pos_indexes "Waypoints" items:#()
		button prev_waypoint_button "< Prev" across:3 width:50 align:#left
		label lbl_waypoint "Waypoint" align:#center
		button next_waypoint_button "Next >" width:50 align:#right
		button remove_sel_index_button "Remove sel" align:#center width:100
		checkbutton btn_create_center "Create Center" width:100
		button btn_link "Link Center" across:2 width: 90
		button btn_unlink "Unlink Center" width: 90
		button btn_set_center "Set Center Pos" width:100
		editText edt_center "Center" readOnly:true
		
		fn update_ddl_pos_indexes =
		(
			temp_array = #()
			for n in waypoints do
			(
				append temp_array n
			)
			ddl_pos_indexes.items = temp_array
			ddl_pos_indexes.selection = temp_array.count
		)
		
		fn set_position_from_index index=
		(
			current_mesh = $
			selection_rotation = rotations[index]
			selection_rotation_as_array = filterString selection_rotation " || "
			my_rotation = eulerAngles (selection_rotation_as_array[1] as float) (selection_rotation_as_array[2] as float) (selection_rotation_as_array[3] as float)
			(in coordsys world $.rotation = my_rotation)
			selection_position = waypoints[index]
			selection_position_as_array = filterString selection_position " || "
			$.position.x = selection_position_as_array[1] as float
			$.position.y = selection_position_as_array[2] as float
			$.position.z = selection_position_as_array[3] as float
			
		)
		
		on add_position_btn pressed do (
			current_mesh = $
			pos_as_string = $.pos.x as string + " || " + $.pos.y as string + " || " +$.pos.z as string
			append waypoints  pos_as_string
			my_rotation = (in coordsys world $.rotation as eulerAngles)
			rot_as_string = my_rotation.x as string + " || " + my_rotation.y as string + " || " + my_rotation.z as string 
			append rotations rot_as_string
			update_ddl_pos_indexes()
		)
		
		on remove_sel_index_button pressed do (
			if ddl_pos_indexes.items.count == 0 then
			(
				return ()
			)
			current_mesh = $
			current_index = ddl_pos_indexes.selection
			deleteItem waypoints current_index
			deleteItem rotations current_index
			update_ddl_pos_indexes()
		)
		
		on ddl_pos_indexes selected i do
		(
			set_position_from_index i
		)
		
		on next_waypoint_button pressed do 
		(
			current_index = ddl_pos_indexes.selection
			current_index += 1
			if current_index > waypoints.count then
			(
				current_index = 1
			)
			ddl_pos_indexes.selection = current_index
			set_position_from_index current_index
		)
		
		on prev_waypoint_button pressed do
		(
			current_index = ddl_pos_indexes.selection
			current_index -= 1
			if current_index == 0 then
			(
				current_index = waypoints.count
			)
			ddl_pos_indexes.selection = current_index
			set_position_from_index current_index
		)
		on btn_create_center changed state do
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
					if center == "" then
					(
						bb = nodeLocalBoundingBox curr_mesh
						height =  (bb[1][3] + bb[2][3])/2
						aux_obj.pos.z = height
					)
					else
					(
						parts = filterString center " || "
						pos = Point3 
						pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
						aux_obj.pos = pos
						curr_mesh.parent = aux_obj
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
		on btn_link pressed do
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj == undefined then
			(
				messageBox "Inexistent Center, run Create Center Command"
				return()
			)
			curr_mesh.parent = aux_obj
		)
		on btn_unlink pressed do
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj == undefined then
			(
				return()
			)
			curr_mesh.parent = undefined
		)
		on btn_set_center pressed do 
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj == undefined then
			(
				messageBox "Inexistent Center, run Create Center Command"
				return()
			)
			pos_as_string = aux_obj.pos.x as string + " || " + aux_obj.pos.y as string + " || " +aux_obj.pos.z as string
			edt_center.text = pos_as_string
			center = pos_as_string
		)
		on params open do
		(
			update_ddl_pos_indexes()
			curr_mesh = $
			if center != "" then
			(
				edt_center.text = center
			)
			if (getNodeByName (curr_mesh.name + "_auxObj")) != undefined then
			(
				btn_create_center.checked=true
			)
		)
	)
)

struct orbit_patrol_controller_cattr
(
	public
	name = "orbit_patrol_comp",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
		
	),
	fn parse_string_as_pos pos_string=
	(
		parts = filterString pos_string " || "
		pos = Point3 
		pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
		return pos
	),
	fn get_waypoints_as_mcv mod_data=
	(
		pos_array = #()
		max2mcv = rotateXMatrix -90
		for i=1 to mod_data.waypoints.count do
		(
			parts = filterString mod_data.waypoints[i] " || "
			pos = Point3 
			pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
			final_pos = (pos * max2mcv)
			append pos_array (final_pos[1] as string + " " + final_pos[2] as string + " " + final_pos[3] as string)
		)
		return pos_array
	),
	fn get_center_as_mcv mod_data=
	(
		max2mcv = rotateXMatrix -90
		parts = filterString mod_data.center " || "
		pos = Point3 
		pos = [parts[1] as float , parts[2] as float,  parts[3] as float]
		final_pos = (pos * max2mcv)
		return (final_pos[1] as string + " " + final_pos[2] as string + " " + final_pos[3] as string)
	),
	fn get_radius_as_mcv mod_data=
	(
		center = get_center_as_mcv mod_data
		parsed_center = parse_string_as_pos center
		first_point = (get_waypoints_as_mcv mod_data)[1]
		parsed_first_point = parse_string_as_pos first_point
		return distance parsed_center parsed_first_point
	),
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "ai_orbit_patrol" 
		fs.beginObj()
			fs.writeKeyValue "speed" mod_data.plattform_speed
			fs.writeComma()
			fs.writeKeyValue "delay" mod_data.plattform_delay
			fs.writeComma()
			fs.writeKey "waypoints"
			fs.arrayOfStrings (get_waypoints_as_mcv mod_data)
			fs.writeComma()
			fs.writeKeyValue "center" (get_center_as_mcv mod_data)
			fs.writeComma()
			fs.writeKeyValue "radius" (get_radius_as_mcv mod_data)
		fs.endObj()
	)
)

orbit_patrol_controller_cattr()