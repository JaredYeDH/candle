#include "light.h"
#include "node.h"
#include "probe.h"
#include "spacial.h"
#include "model.h"
#include "mesh_gl.h"
#include <candle.h>
#include <systems/editmode.h>
#include <systems/window.h>
#include <systems/renderer.h>
#include <stdlib.h>

static fs_t *g_depth_fs = NULL;
entity_t g_light = entity_null;

void c_light_init(c_light_t *self)
{
	self->color = vec4(1.0f);
	self->shadow_size = 512;
	self->radius = 5.0f;

	if(!g_light)
	{
		g_depth_fs = fs_new("depth");

		mesh_t *mesh = mesh_new();
		mesh_lock(mesh);
		mesh_ico(mesh, -0.5f);
		mesh_select(mesh, SEL_EDITING, MESH_FACE, -1);
		mesh_subdivide(mesh, 1);
		mesh_spherize(mesh, 1.0f);

		mesh_unlock(mesh);
		g_light = entity_new(c_node_new(), c_model_new(mesh, NULL, 0, 0));
		c_node(&g_light)->ghost = 1;

	}
}

c_light_t *c_light_new(float radius,
		vec4_t color, int shadow_size)
{
	c_light_t *self = component_new("light");

	self->color = color;
	self->shadow_size = shadow_size;
	self->radius = radius;

	return self;
}

int c_light_render(c_light_t *self)
{
	c_renderer(&SYS)->bound_light = c_entity(self);

	if(!g_light || !c_mesh_gl(&g_light)) return STOP;
	if(self->radius > 0.0f)
	{
		shader_t *shader = vs_bind(g_model_vs);
		if(!shader) return STOP;
		c_node_t *node = c_node(self);
		c_spacial_t *sc = c_spacial(self);

		c_node_update_model(node);

		float rad = self->radius * 1.1f;
		/* float rad = self->radius * 0.5f; */
		mat4_t model = mat4_scale_aniso(node->model,
				vec3(rad / sc->scale.x, rad / sc->scale.y, rad / sc->scale.z));

#ifdef MESH4
		shader_update(shader, &model, node->angle4);
#else
		shader_update(shader, &model);
#endif

		c_mesh_gl_draw(c_mesh_gl(&g_light), 0);
		return CONTINUE;
	}
	else
	{
		return c_window_render_quad(c_window(&SYS), NULL);
	}
}


int c_light_menu(c_light_t *self, void *ctx)
{
	nk_layout_row_dynamic(ctx, 0, 1);

	int ambient = self->radius == -1.0f;
	nk_checkbox_label(ctx, "ambient", &ambient);

	if(!ambient)
	{
		if(self->radius < 0.0f) self->radius = 0.01;
		nk_property_float(ctx, "radius:", 0.01, &self->radius, 1000, 0.1, 0.05);
	}
	else
	{
		self->radius = -1;
	}

	nk_layout_row_dynamic(ctx, 180, 1);

	union { struct nk_colorf *nk; vec4_t *v; } color = { .v = &self->color };
	*color.nk = nk_color_picker(ctx, *color.nk, NK_RGBA);

	return CONTINUE;
}

void c_light_destroy(c_light_t *self)
{
}

int c_light_probe_render(c_light_t *self)
{
	if(self->radius < 0.0f) return CONTINUE;
	c_probe_t *probe = c_probe(self);
	if(!probe)
	{
		entity_add_component(c_entity(self), (c_t*)c_probe_new(self->shadow_size));
		entity_signal(c_entity(self), sig("spacial_changed"), &c_entity(self));
		return STOP;
	}
	if(!g_depth_fs) return STOP;

	fs_bind(g_depth_fs);

	glDisable(GL_CULL_FACE);
	c_probe_render(probe, sig("render_shadows"));
	glEnable(GL_CULL_FACE);
	return CONTINUE;
}

REG()
{
	ct_t *ct = ct_new("light", sizeof(c_light_t), c_light_init,
			c_light_destroy, 1, ref("node"));

	ct_listener(ct, WORLD, sig("offscreen_render"), c_light_probe_render);

	ct_listener(ct, WORLD, sig("component_menu"), c_light_menu);

	ct_listener(ct, WORLD, sig("render_lights"), c_light_render);

	signal_init(sig("render_shadows"), 0);
}
