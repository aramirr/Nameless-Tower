attribute_file_dir_path = (getDir #userStartupScripts) + "/../Types/"

struct AttributeExporter
(
	public 
	list_items = #(),
	my_attributes = #(),
	
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
	),
	
	fn get_object_exportable_modifiers obj=
	(
		mods_indexes = #()
		for modifier in obj.modifiers do
		(
			index = findItem list_items modifier.name 
			if index != 0 then
			(
				append mods_indexes index
			)
		)
		return mods_indexes
	),
	
	fn export_object_process obj fs=
	(
		indexes = get_object_exportable_modifiers obj
		if indexes.count == 0 then 
		(
			return()
		)
		for index in indexes do 
		(
			a_mod = my_attributes[index]
			modifier_identifier = execute("#" + list_items[index])
			a_mod.export_function fs obj.modifiers[modifier_identifier]
		)
	)
)

AttributeExporter()