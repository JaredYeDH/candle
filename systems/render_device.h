#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <utils/glutil.h>
#include <utils/material.h>
#include <utils/texture.h>
#include <utils/mesh.h>
#include <utils/shader.h>
#include <utils/drawable.h>
#include <ecs/ecm.h>

typedef struct pass_t pass_t;

typedef struct uniform_t uniform_t;

struct gl_light
{
	vec4_t color;
	ivec2_t pos;
	uint32_t lod;
	float radius;
};

struct gl_property
{
	vec4_t color;
	uvec2_t size;
	float blend;
	float scale;
	vec3_t padding;
	uint32_t layer;
};

struct gl_material
{
	struct gl_property albedo;
	struct gl_property roughness;
	struct gl_property metalness;
	struct gl_property transparency;
	struct gl_property normal;
	struct gl_property emissive;
};

struct gl_pass
{
	vec2_t screen_size;
	vec2_t padding;
};

struct gl_scene
{
	struct gl_material materials[128];
	struct gl_light lights[62];
	vec4_t test_color;
};

struct gl_bones
{
	mat4_t bones[30];
};

typedef struct c_render_device_t
{
	c_t super;

	void(*bind_function)(void *usrptr, shader_t *shader);
	void *usrptr;

	fs_t *frag_bound;
	shader_t *shader;
	uint32_t ubo;
	struct gl_scene scene;
	int32_t updates_ram;
	int32_t updates_ubo;
	int32_t frame;
	uint32_t bound_ubos[32];
} c_render_device_t;

DEF_CASTER("render_device", c_render_device, c_render_device_t)

c_render_device_t *c_render_device_new(void);
void c_render_device_rebind(
		c_render_device_t *self,
		void(*bind_function)(void *usrptr, shader_t *shader),
		void *usrptr);
void world_changed(void);
void c_render_device_bind_ubo(c_render_device_t *self, uint32_t base,
                              uint32_t ubo);


#endif /* !RENDER_DEVICE_H */
