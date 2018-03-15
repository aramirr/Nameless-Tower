utility MyMCVExporter "MCV Exporter"
(
	
	rollout exporter "The Exporter"
	(
		
		button btn_scene "Export Scene"
		button btn_mesh "Export Mesh"
		button btn_skeleton "Export Skeleton & Meshes"
		button btn_skel_anims "Export Animation" tooltip:"Uses max animation range and frame rate to specify the range to export"
		
		on btn_scene pressed do
		(
			try (
				gc()
				local exporter = TSceneExporter()
				exporter.exportAll()	
				MessageBox "All exported OK"
			) catch (
				MessageBox ("Error Exporting:\n" + getCurrentException())
			)
		)
		
		-- Exports the current selection mesh to file
		on btn_mesh pressed do
		(
			
			try (
				gc()
				local ofilename = undefined  -- to be defined by you.
				exportMeshObject $ ofilename
				MessageBox "Single mesh exported OK"
			) catch (
				MessageBox ("Error Exporting Single Mesh:\n" + getCurrentException())
			)
		)

		-- Exports the current selection mesh to file
		on btn_skeleton pressed do
		(
			
			try (
				gc()
				local se = TSkeletonsExporter()
				se.exportSkelAndMeshes()
				MessageBox "Skeleton And Meshes exported OK"
			) catch (
				MessageBox ("Error Exporting Skeleton:\n" + getCurrentException())
			)
		)
		
		-- Exports the current selection mesh to file
		on btn_skel_anims pressed do
		(
			
			try (
				gc()
				local se = TSkeletonsExporter()
				se.exportAnim()
				MessageBox "Skeleton Animation exported OK"
			) catch (
				MessageBox ("Error Exporting Skeleton Animation:\n" + getCurrentException())
			)
		)		
		
	) -- end rollout creator
		
	-- ...
	on MyMCVExporter open do
	(
		addRollout exporter
	) 
	on MyMCVExporter close do
	(
		removeRollout exporter
	) 
) -- end utility MyUtil 