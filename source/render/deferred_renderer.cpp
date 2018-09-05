#include "mcv_platform.h"
#include "deferred_renderer.h"
#include "render_manager.h"
#include "render_utils.h"
#include "render_objects.h"
#include "resources/resources_manager.h"
#include "components/lights/comp_light_dir.h"
#include "components/juan/comp_transform.h"
#include "components/lights/comp_light_point.h"
#include "components/juan/comp_transform.h"
#include "components/postfx/comp_render_ao.h"
#include "ctes.h"
#include "components/ia/puzzles/ai_torch.h"
#include "render/texture/render_to_cube.h"

void CDeferredRenderer::renderGBuffer() {
	CTraceScoped gpu_scope("Deferred.GBuffer");

	// Disable the gbuffer textures as we are going to update them
	// Can't render to those textures and have them active in some slot...
	CTexture::setNullTexture(TS_DEFERRED_ALBEDOS);
	CTexture::setNullTexture(TS_DEFERRED_NORMALS);
	CTexture::setNullTexture(TS_DEFERRED_SELF_ILLUM);
	CTexture::setNullTexture(TS_DEFERRED_LINEAR_DEPTH);
	CTexture::setNullTexture(TS_DEFERRED_ALPHA);
	CTexture::setNullTexture(TS_DEFERRED_CELL);
  CTexture::setNullTexture(TS_DEFERRED_SUBLIME);

  // Activate el multi-render-target MRT
  const int nrender_targets = 7;
  ID3D11RenderTargetView* rts[nrender_targets] = {
    rt_albedos->getRenderTargetView(),
    rt_normals->getRenderTargetView(),
	rt_depth->getRenderTargetView(),
	rt_alpha->getRenderTargetView(),
		rt_self_illum->getRenderTargetView(),
		rt_cell->getRenderTargetView(),
    rt_sublime->getRenderTargetView()
  };

	// We use our 3 rt's and the Zbuffer of the backbuffer
	Render.ctx->OMSetRenderTargets(nrender_targets, rts, rt_acc_light->getDepthStencilView());
	rt_albedos->activateViewport();   // Any rt will do...

	// Clear output buffers, some can be removed if we intend to fill all the screen
	// with new data.
	rt_albedos->clear(VEC4(1, 0, 0, 1));
	rt_normals->clear(VEC4(0, 0, 1, 1));
	rt_depth->clear(VEC4(1, 1, 1, 1));
	rt_self_illum->clear(VEC4(0, 0, 0, 1));
	rt_alpha->clear(VEC4(0, 0, 0, 1));
	rt_cell->clear(VEC4(0, 0, 0, 1));
  rt_sublime->clear(VEC4(0, 0, 0, 1));

	 // Clear ZBuffer with the value 1.0 (far)
	Render.ctx->ClearDepthStencilView(rt_acc_light->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the solid objects that output to the G-Buffer
	CRenderManager::get().renderCategory("gbuffer");

	// Disable rendering to all render targets.
	ID3D11RenderTargetView* rt_nulls[nrender_targets];
	for (int i = 0; i < nrender_targets; ++i) rt_nulls[i] = nullptr;
	Render.ctx->OMSetRenderTargets(nrender_targets, rt_nulls, nullptr);

	// Activate the gbuffer textures to other shaders
	rt_albedos->activate(TS_DEFERRED_ALBEDOS);
	rt_normals->activate(TS_DEFERRED_NORMALS);
	rt_self_illum->activate(TS_DEFERRED_SELF_ILLUM);
	rt_depth->activate(TS_DEFERRED_LINEAR_DEPTH);
	rt_alpha->activate(TS_DEFERRED_ALPHA);
	rt_cell->activate(TS_DEFERRED_CELL);
  rt_sublime->activate(TS_DEFERRED_SUBLIME);
}

// --------------------------------------------------------------
void CDeferredRenderer::renderGBufferDecals() {
	CTraceScoped gpu_scope("Deferred.GBuffer.Decals");

	// Disable the gbuffer textures as we are going to update them
	// Can't render to those textures and have them active in some slot...
	CTexture::setNullTexture(TS_DEFERRED_ALBEDOS);
	//CTexture::setNullTexture(TS_DEFERRED_NORMALS);

	// Activate el multi-render-target MRT
	const int nrender_targets = 2;
	ID3D11RenderTargetView* rts[nrender_targets] = {
		rt_albedos->getRenderTargetView(),
		//rt_normals->getRenderTargetView()
		// No Z as we need to read to reconstruct the position
	};

	// We use our 3 rt's and the Zbuffer of the backbuffer
	Render.ctx->OMSetRenderTargets(nrender_targets, rts, rt_acc_light->getDepthStencilView());
	rt_albedos->activateViewport();   // Any rt will do...

	// Render blending layer on top of gbuffer before adding lights
	CRenderManager::get().renderCategory("gbuffer_decals");

	// Disable rendering to all render targets.
	ID3D11RenderTargetView* rt_nulls[nrender_targets];
	for (int i = 0; i < nrender_targets; ++i) rt_nulls[i] = nullptr;
	Render.ctx->OMSetRenderTargets(nrender_targets, rt_nulls, nullptr);

	// Activate the gbuffer textures to other shaders
	//rt_normals->activate(TS_DEFERRED_NORMALS);
	rt_albedos->activate(TS_DEFERRED_ALBEDOS);
}

// -----------------------------------------------------------------
bool CDeferredRenderer::create(int xres, int yres, const char* prefix) {

	char name[64];

	rt_albedos = new CRenderToTexture;
	sprintf(name, "%s_albedos", prefix);
	if (!rt_albedos->createRT(name, xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	rt_normals = new CRenderToTexture;
	sprintf(name, "%s_normals", prefix);
	//if (!rt_normals->createRT(name, xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM))
	if (!rt_normals->createRT(name, xres, yres, DXGI_FORMAT_R16G16B16A16_UNORM))     // For improved normal quality storage.
		return false;

	rt_depth = new CRenderToTexture;
	sprintf(name, "%s_depths", prefix);
	if (!rt_depth->createRT(name, xres, yres, DXGI_FORMAT_R32_FLOAT))
		return false;

	rt_acc_light = new CRenderToTexture;
	sprintf(name, "%s_acc_light", prefix);
	bool use_back_buffer = (xres == Render.width && yres == Render.height);
	DXGI_FORMAT depth_fmt = use_back_buffer ? DXGI_FORMAT_UNKNOWN : DXGI_FORMAT_R32_TYPELESS;
	if (!rt_acc_light->createRT(name, xres, yres, DXGI_FORMAT_R16G16B16A16_FLOAT, depth_fmt, use_back_buffer))
		return false;

	rt_self_illum = new CRenderToTexture;
	sprintf(name, "%s_self_illum", prefix);
	if (!rt_self_illum->createRT(name, xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	rt_alpha = new CRenderToTexture;
	sprintf(name, "%s_alpha", prefix);
	if (!rt_alpha->createRT(name, xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	rt_cell = new CRenderToTexture;
	sprintf(name, "%s_cell", prefix);
	if (!rt_cell->createRT(name, xres, yres, DXGI_FORMAT_R8_UNORM))
		return false;

  rt_sublime = new CRenderToTexture;
  if (!rt_sublime->createRT("g_sublime.dds", xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM))
    return false;

	return true;
}

// -----------------------------------------------------------------
void CDeferredRenderer::renderAmbientPass() {
	CTraceScoped gpu_scope("renderAmbientPass");
	renderFullScreenQuad("pbr_ambient.tech", nullptr);
}

// -------------------------------------------------------------------------

void CDeferredRenderer::renderSkyBox() const {
	CTraceScoped gpu_scope("renderSkyBox");
	renderFullScreenQuad("pbr_skybox.tech", nullptr);
}

// -------------------------------------------------------------------------
void CDeferredRenderer::renderAccLight() {
	CTraceScoped gpu_scope("Deferred.AccLight");
	rt_acc_light->activateRT();
	rt_acc_light->clear(VEC4(0, 0, 0, 0));
	renderAmbientPass();
	renderPointLights();
	renderDirectionalLights();

	renderSkyBox();
}

// -------------------------------------------------------------------------
void CDeferredRenderer::renderPointLights() {
	CTraceScoped gpu_scope("renderPointLights");

	// Activate tech for the light dir 
	auto* tech = Resources.get("pbr_point_lights.tech")->as<CRenderTechnique>();
	tech->activate();

	// All light directional use the same mesh
	auto* mesh = Resources.get("data/meshes/UnitSphere.mesh")->as<CRenderMesh>();
	mesh->activate();

	// Para todas las luces... pintala
	getObjectManager<TCompLightPoint>()->forEach([mesh](TCompLightPoint* c) {

		if (c->have_shadows()) {
			for (int i = 0; i < 6; i++) {
				c->activate(i);

				setWorldTransform(c->getWorld());

				// mandar a pintar una geometria que refleje los pixeles que potencialmente
				// puede iluminar esta luz.... El Frustum solido
				mesh->render();
			}
		}
		else {
			c->activate(0);

			setWorldTransform(c->getWorld());

			// mandar a pintar una geometria que refleje los pixeles que potencialmente
			// puede iluminar esta luz.... El Frustum solido
			mesh->render();
		}
		// subir las contantes de la posicion/dir
		// activar el shadow map...
		
	});

	//Pintar luz de las antorchas
	getObjectManager<CAITorch>()->forEach([mesh](CAITorch* c) {

		// subir las contantes de la posicion/dir
		// activar el shadow map...
		c->simulateLight();

		setWorldTransform(c->getWorld());

		// mandar a pintar una geometria que refleje los pixeles que potencialmente
		// puede iluminar esta luz.... El Frustum solido
		mesh->render();
	});
}


// -------------------------------------------------------------------------
void CDeferredRenderer::renderDirectionalLights() {
	CTraceScoped gpu_scope("renderDirectionalLights");

	// Activate tech for the light dir 
	auto* tech = Resources.get("pbr_dir_lights.tech")->as<CRenderTechnique>();
	tech->activate();

	// All light directional use the same mesh
	auto* mesh = Resources.get("data/meshes/UnitFrustum.mesh")->as<CRenderMesh>();
	mesh->activate();

	// Para todas las luces... pintala
	getObjectManager<TCompLightDir>()->forEach([mesh](TCompLightDir* c) {

		// subir las contantes de la posicion/dir
		// activar el shadow map...
		c->activate();

		setWorldTransform(c->getViewProjection().Invert());

		// mandar a pintar una geometria que refleje los pixeles que potencialmente
		// puede iluminar esta luz.... El Frustum solido
		mesh->render();
	});
}

// --------------------------------------
void CDeferredRenderer::renderAO(CHandle h_camera) const {
	CEntity* e_camera = h_camera;
	if (e_camera) {
		TCompRenderAO* comp_ao = e_camera->get<TCompRenderAO>();

		if (comp_ao) {
			// As we are going to update the RenderTarget AO
			// it can NOT be active as a texture while updating it.
			CTexture::setNullTexture(TS_DEFERRED_AO);
			auto ao = comp_ao->compute(rt_depth);
			// Activate the updated AO texture so everybody else can use it
			// Like the AccLight (Ambient pass or the debugger)
			ao->activate(TS_DEFERRED_AO);
			return;
		}
	}

	// As there is no comp AO, use a white texture as substitute
	const CTexture* white_texture = Resources.get("data/textures/white.dds")->as<CTexture>();
	white_texture->activate(TS_DEFERRED_AO);
	//if (!comp_ao) {
	//	// As there is no comp AO, use a white texture as substitute
	//	const CTexture* white_texture = Resources.get("data/textures/white.dds")->as<CTexture>();
	//	white_texture->activate(TS_DEFERRED_AO);
	//	return;
	//}
	//// As we are going to update the RenderTarget AO
	//// it can NOT be active as a texture while updating it.
	//CTexture::setNullTexture(TS_DEFERRED_AO);
	//auto ao = comp_ao->compute(rt_depth);
	//// Activate the updated AO texture so everybody else can use it
	//// Like the AccLight (Ambient pass or the debugger)
	//ao->activate(TS_DEFERRED_AO);
}

// --------------------------------------
void CDeferredRenderer::render(CRenderToTexture* rt_destination, CHandle h_camera) {
	assert(rt_destination);

	renderGBuffer();
	renderGBufferDecals();
	renderAO(h_camera);

	// Do the same with the acc light
	CTexture::setNullTexture(TS_DEFERRED_ACC_LIGHTS);
	renderAccLight();

	// Now dump contents to the destination buffer.
	rt_destination->activateRT();
	rt_acc_light->activate(TS_DEFERRED_ACC_LIGHTS);

	// Combine the results
	renderFullScreenQuad("gbuffer_resolve.tech", nullptr);
}

// --------------------------------------
void CDeferredRenderer::renderToCubeFace(CRenderToCube* rt_destination, int face_idx) {
	assert(rt_destination);

	CCamera camera;
	rt_destination->getCamera(face_idx, &camera);
	activateCamera(camera, rt_destination->getWidth(), rt_destination->getHeight());

	renderGBuffer();
	renderGBufferDecals();
	renderAO(CHandle());

	// Do the same with the acc light
	CTexture::setNullTexture(TS_DEFERRED_ACC_LIGHTS);
	renderAccLight();

	// Now dump contents to the destination buffer.
	rt_destination->activateFace(face_idx, &camera);
	rt_acc_light->activate(TS_DEFERRED_ACC_LIGHTS);

	// Combine the results
	renderFullScreenQuad("gbuffer_resolve_face.tech", nullptr);
}
