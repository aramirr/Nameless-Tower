cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		waypoints type:#stringTab tabSize:0 tabSizeVariable:true
		plattform_speed type: #float ui:p_speed default:1
		plattform_delay type: #float ui:p_delay default:0.1
	)
	rollout params "Linear Patrol Parameters"
	( 
		spinner p_speed "Speed" type: #float
		spinner p_delay "Delay" type: #float
		
		button add_position_btn "Add Waypoints"
		dropdownlist ddl_pos_indexes "Waypoints" items:#()
		button prev_waypoint_button "< Prev" across:3 width:50 align:#left
		label lbl_waypoint "Waypoint" align:#center
		button next_waypoint_button "Next >" width:50 align:#right
		button remove_sel_index_button "Remove sel" align:#center width:100
		
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
			selection_position = waypoints[index]
			selection_position_as_array = filterString selection_position " || "
			$.position.x = selection_position_as_array[1] as float
			$.position.y = selection_position_as_array[2] as float
			$.position.z = selection_position_as_array[3] as float
		)
		
		on params open do
		(
			update_ddl_pos_indexes()
		)
		
		on add_position_btn pressed do (
			current_mesh = $
			pos_as_string = $.pos.x as string + " || " + $.pos.y as string + " || " +$.pos.z as string
			append waypoints  pos_as_string
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
		
	)
)

struct linear_patrol_controller_cattr
(
	public
	name = "linear_patrol_comp",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
		
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
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "ai_linear_patrol" 
		fs.beginObj()
			fs.writeKeyValue "speed" mod_data.plattform_speed
			fs.writeComma()
			fs.writeKeyValue "delay" mod_data.plattform_delay
			fs.writeComma()
			fs.writeKey "waypoints"
			fs.arrayOfStrings (get_waypoints_as_mcv mod_data)
		fs.endObj()
	)
)

linear_patrol_controller_cattr()