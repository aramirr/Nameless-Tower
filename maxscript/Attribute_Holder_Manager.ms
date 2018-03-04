attribute_file_dir_path = (getDir #userStartupScripts) + "/../Types/"

try(DestroyDialog RL_Attribute_Dispatcher)catch()
rollout RL_Attribute_Dispatcher "Attr. Dispatcher"
(
	local list_items = #()
	local my_attributes = #()
	
	label lbl_attribute_type "Attribute Type"
	edittext edt_Filter "Filter"
	listbox lbx_attribute_types "Attribute types" items:#() 
	button btn_Add "Add to sel"
	
	fn initialize_files =
	(
		list_items = #()
		attribute_expression = attribute_file_dir_path + "*ms"
		dir_items = getFiles attribute_expression
		for i in dir_items do
		(
			result = fileIn i
			append list_items result.name
			append my_attributes result
		)
	)

    fn update_listbox=
	(
		lbx_attribute_types.items = #()
		for i in list_items do
		(
			result = findString i edt_Filter.text
			if result == undefined then 
			(
				continue
			)
			lbx_attribute_types.items = append lbx_attribute_types.items i
		)		
	)
	
	fn get_selected_custom_attribute=
	(
		selected_text = lbx_attribute_types.selected
		if selected_text == "" then
			return undefined
		else
			for attr in my_attributes do
			(
				if attr.name == selected_text do
				(
					return attr
				)
			)
		return undefined
	)
	
	fn has_attribute obj attribute_definition =
	(
		for modifier in obj.modifiers do
		(
			if modifier.name == attribute_definition.name do
			(
				return true
			)
		)
		return false
	)
	
	fn add_custom_attribute_to_obj obj type_definition=
	(
		abort = has_attribute obj type_definition
		if abort == true then
		(
			return()
		)
		else
		(
			attribute_holder = EmptyModifier()
			attribute_holder.name = type_definition.name
			attr_holder_modifier = addModifier obj attribute_holder
			custAttributes.add attribute_holder type_definition.custom_attribute
		)
	)

	
	on edt_Filter changed text do
    (
         update_listbox()
    )
	
	on btn_Add pressed do
	(
		cust_attribute = get_selected_custom_attribute()
		if cust_attribute == undefined then
		(
			messageBox ("Error getting custAttribute" + lbx_attribute_types.selected)
		)
		else
		(
			for obj in selection do
			(
				add_custom_attribute_to_obj obj cust_attribute
				cust_attribute.connection_function()
			)
		)
	)
)
Createdialog RL_Attribute_Dispatcher
RL_Attribute_Dispatcher.initialize_files()
DestroyDialog RL_Attribute_Dispatcher
--RL_Attribute_Dispatcher.update_listbox()
