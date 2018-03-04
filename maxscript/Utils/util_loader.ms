utility export_utility "MCV utils"
(
	--button btn_launch_exporter "Launch Exporter"
	button attr_holder_manager "Launch Attribute Manager" width:150
	button btn_export_scene "Export Scene" width:150
	button btn_export_sel "Export Sel" width:150
	button btn_clear_data "Clear Data" width:150
	
	button reset_memory "Clear Memory" width:150
	
	/*on btn_launch_exporter pressed d
	(
		gc()
		createDialog mcv_exporter_tool_rollout lockwidth:false lockheight:false 
		clearListener()
	)*/
	on btn_export_scene pressed do
	(
		exporter = TSceneExporter()
		exporter.exportAll()
	)
	
	on btn_export_sel pressed do
	(
		exporter = TSceneExporter()
		exporter.exportSelection()
	)
	
	on attr_holder_manager pressed do 
	(
		Createdialog RL_Attribute_Dispatcher
		clearListener()
		RL_Attribute_Dispatcher.initialize_files()
		RL_Attribute_Dispatcher.update_listbox()
	)
	on btn_clear_data pressed do
	(
		attribute_data_path = (getDir #userStartupScripts) + "/../data/"	
		dirs = #("meshes", "textures","scenes", "materials")
		for dir in dirs do
		(
			current_dir = attribute_data_path + "/" + dir
			files = getFiles (current_dir + "/*.*")
			for _file in files do
			(
				deleteFile _file
			)
		)
 	)
			
	on reset_memory pressed do
	(
		gc()
	)
)