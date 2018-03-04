rollout mcv_exporter_tool_rollout "MCV exporter tool"
(
	-- Global vars
	local export_path = (getDir #userStartupScripts) + "/../data" 
	
	-- UI 
	GroupBox gbConfigs "Asset Configs: " pos:[10,10] width:148 height:95
	radiobuttons rbAssetType "Asset Type:" pos:[20,28] enabled:true labels:#("chr", "env") default:1
	label lbMainDesc "Asset Name:" pos:[20,65]
	edittext etMainDesc pos:[15,80] witdth: 148
	button bExportScn "Export Scene " pos:[10,110] width:148 height:22
	button bExportSel "Export Sel " pos:[10,135] width:148 height:22
	
	--Functions 
	fn isCollider obj = (
		if obj.layer.name != "collisions" and obj.layer.name != "triggers" then return false
		if classof obj != Box and classof obj != Sphere then throw "Collisions must be Spheres or Boxes."
		-- if name starts with col_...
		return true
	)	

	fn material_check=
	(
		rgx = dotnetObject "System.Text.RegularExpressions.Regex" ""
		invalid_mats = #()
		for mat in sceneMaterials do
		(
			found = rgx.IsMatch mat.name "Material #[0-9]+$"
			if found == true do
			(
				append invalid_mats mat.name
			)
		)
		
		if invalid_mats.count == 0 then
		(
			return true
		)
		else
		(
			warning_message = "Check Material naming for:\n"
			for i = 1 to invalid_mats.count do 
			(
				warning_message += invalid_mats[i] + "\n"
			)
			messageBox (warning_message)
			return false
		)
	)
	
	fn scene_export_process= 
	(
		asset_name_text = etMainDesc.text
		if asset_name_text =="" then
		(
			messageBox ("Input a name for the asset exported")
			return()
		)
		
		materials_ok = material_check()
		if materials_ok == false then
			return ()
		
		asset_type = undefined
		if rbAssetType.state == 1 then
		(
			asset_type = "chr"
		)
		
		if rbAssetType.state == 2 then
		(
			asset_type = "env"
		)
		
		renamed_objects = #()
		for s_obj in $* do
		(
			if (isCollider s_obj) or (classOf s_obj == Editable_mesh) then
			(
				s_obj.name = asset_type + "_" + asset_name_text + "_" + s_obj.name
				append renamed_objects s_obj
			)
		)
		
		exporter = TSceneExporter()
		try
		(
			exporter.exportAll()
		)
		catch
		(
			for obj in renamed_objects do
			(
				obj.name = substituteString obj.name (asset_type + "_" + asset_name_text + "_") ""
			)
		)
		
		for obj in renamed_objects do
		(
			obj.name = substituteString obj.name (asset_type + "_" + asset_name_text + "_") ""
		)
		
		
	)
	
	fn selection_export_process= 
	(
		asset_name_text = etMainDesc.text
		if asset_name_text =="" then
		(
			messageBox ("Input a name for the asset exported")
			return()
		)
		
		materials_ok = material_check()
		if materials_ok == false then
			return ()
		
		asset_type = undefined
		if rbAssetType.state == 1 then
		(
			asset_type = "chr"
		)
		
		if rbAssetType.state == 2 then
		(
			asset_type = "env"
		)
		
		renamed_objects = #()
		for s_obj in $* do
		(
			if (isCollider s_obj) or (classOf s_obj == Editable_mesh) then
			(
				s_obj.name = asset_type + "_" + asset_name_text + "_" + s_obj.name
				append renamed_objects s_obj
			)
		)
		
		exporter = TSceneExporter()
		try
		(
			exporter.exportSelection()
		)
		catch
		(
			for obj in renamed_objects do
			(
				obj.name = substituteString obj.name (asset_type + "_" + asset_name_text + "_") ""
			)
		)
		
		for obj in renamed_objects do
		(
			obj.name = substituteString obj.name (asset_type + "_" + asset_name_text + "_") ""
		)
		
		
	)
	
	fn initialize_export_paths=
	(
		export_cathegories = #("materials", "meshes", "scenes", "textures")
		for i = 1 to export_cathegories.count do
		(
			if not doesFileExist(export_path + "/" + export_cathegories[i]) do 
			(
				makedir (export_path + "/" + export_cathegories[i]) all:true
			)
		)
	)
	
	on bExportScn pressed do
	(
		scene_export_process()
	)
	
	on bExportSel pressed do
	(
		selection_export_process()
	)
	
	on mcv_exporter_tool_rollout open do
	(
		initialize_export_paths()
	)
)

--Execution of the UI
fn launch_mcv_exporter_tool_rollout=
(
	gc()
	createDialog mcv_exporter_tool_rollout lockwidth:false lockheight:false 
	clearListener()
)

--launch_mcv_exporter_tool_rollout()
