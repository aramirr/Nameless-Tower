cattr_definition = attributes attributeTypeData
(
	parameters main rollout:params
	(
		halfExtendX type: #float ui:halfExtendX default:10
		halfExtendY type: #float ui:halfExtendY default:10
		halfExtendZ type: #float ui:halfExtendZ default:10
		
		is_dynamic type: #string default: "false"
		is_trigger type: #string default: "false"
	)
	rollout params "Box Collider Parameters"
	( 
		spinner halfExtendX "HalfExtendX" type: #float
		spinner halfExtendY "HalfExtendY" type: #float
		spinner halfExtendZ "HalfExtendZ" type: #float
		dropdownlist ddl_is_dynamic "is dynamic" items:#("false","true")
		dropdownlist ddl_is_trigger "is trigger" items:#("false","true")

		
		checkbutton chbtn_create "CreateTemp"
		button center_command "Center"
		
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
		
		fn getAABBoxPoints obj = (
			local aabbPoints = #()
			local bbMax = obj.max
			local bbMin = obj.min

			append aabbPoints (Point3 bbMax.x bbMax.y bbMax.z)	
			append aabbPoints (Point3 bbMax.x bbMin.y bbMax.z)	
			append aabbPoints (Point3 bbMin.x bbMin.y bbMax.z)	
			append aabbPoints (Point3 bbMin.x bbMax.y bbMax.z)	

			append aabbPoints (Point3 bbMax.x bbMax.y bbMin.z)	
			append aabbPoints (Point3 bbMax.x bbMin.y bbMin.z)	
			append aabbPoints (Point3 bbMin.x bbMin.y bbMin.z)	
			append aabbPoints (Point3 bbMin.x bbMax.y bbMin.z)	

			return aabbPoints
		)

		fn getOBBoxPoints obj = (
			local obbPoints = #()
			local worldSpaceXform = obj.transform
			local objectSpaceXform = inverse worldSpaceXform

			obj.transform *= objectSpaceXform
			aabbPoints = getAABBoxPoints obj
			obj.transform = worldSpaceXform

			for pt in aabbPoints do (
				append obbPoints (pt * worldSpaceXform)
			)

			return obbPoints
		)

		fn get_lenght_width_height_from_bbox obj=
		(
			result = getOBBoxPoints obj
			dA = distance result[1] result[2]
			dB = distance result[2] result[3]
			dC = distance result[2] result[6]
			return #(dA, dB, dC)
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
					aux_obj = Box()
					aux_obj.name = aux_obj_name
					aux_obj.boxmode = on
					curr_mesh_l_w_h = get_lenght_width_height_from_bbox curr_mesh
					halfExtendX.value = curr_mesh_l_w_h[1]/2
					halfExtendY.value = curr_mesh_l_w_h[2]/2
					halfExtendZ.value = curr_mesh_l_w_h[3]/2
					paramWire.connect curr_mesh.modifiers[#box_collider].attributeTypeData[#halfExtendX] aux_obj.baseObject[#Length] "halfExtendX*2"
					paramWire.connect curr_mesh.modifiers[#box_collider].attributeTypeData[#halfExtendY] aux_obj.baseObject[#Width] "halfExtendY*2"
					paramWire.connect curr_mesh.modifiers[#box_collider].attributeTypeData[#halfExtendZ] aux_obj.baseObject[#Height] "halfExtendZ*2"
					aux_obj.pivot = aux_obj.center
					aux_obj.transform = curr_mesh.transform
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
		
		on center_command pressed do
		(
			curr_mesh = $
			aux_obj_name = curr_mesh.name + "_auxObj"
			aux_obj = getNodeByName aux_obj_name
			if aux_obj != undefined then
			(
				aux_obj.transform = curr_mesh.transform
			)
		)
		
		on ddl_is_dynamic selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#box_collider].is_dynamic = ddl_is_dynamic.items[i]
		)
		
		on ddl_is_trigger selected i do
		(
			curr_mesh = $
			curr_mesh.modifiers[#box_collider].is_trigger = ddl_is_trigger.items[i]
		)
	 )
)

struct box_collider_cattr
(
	public
	name = "box_collider",
	custom_attribute = cattr_definition,
	
	fn connection_function = 
	(
	),
	
	fn export_function fs mod_data=
	(
		fs.writeComma()
		fs.writeKey "collider" 
		fs.beginObj()
			fs.writeKeyValue "shape" "box"
			fs.writeComma()
			vec = mod_data.halfExtendY as string + " " + mod_data.halfExtendZ as string + " " + mod_data.halfExtendX as string
			fs.writeKeyValue "halfExtent" vec 
			fs.writeComma()
			fs.writeKeyValue "is_dynamic" (mod_data.is_dynamic as booleanClass)
			fs.writeComma()
			fs.writeKeyValue "is_trigger" (mod_data.is_trigger as booleanClass)
		fs.endObj()
	)
)

box_collider_cattr()