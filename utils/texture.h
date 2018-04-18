#ifndef TEXTURE_H
#define TEXTURE_H

#include "glutil.h"

typedef struct
{
	GLuint id;
	uint ready;
	int dims;
	uint format;
	uint internal;
	char *name;
	GLubyte *data;
} buffer_t;

typedef struct
{
	char name[256];
	int id;

	uint width;
	uint height;
	uint depth;
	uint depth_buffer;
	void *last_depth;
	uint repeat;

	buffer_t bufs[16]; /* 0 is depth, 1 is color, >= 2 are color buffers */

	int bufs_size;

	float brightness;
	GLuint target;
	GLuint frame_buffer[6];
	char *filename;
	int framebuffer_ready;
	int draw_id;
	int prev_id;
	int mipmaped;
} texture_t;

texture_t *texture_from_file(const char *filename);

texture_t *_texture_new_2D_pre
(
	uint width,
	uint height,
	uint repeat,
	uint mipmaped
);

extern __thread texture_t *_g_tex_creating;
#define texture_new_2D(w, h, r, m, ...) \
	(_texture_new_2D_pre(w, h, r, m),_texture_new(0, ##__VA_ARGS__))

texture_t *_texture_new(int ignore, ...);

texture_t *texture_new_3D
(
	uint width,
	uint height,
	uint depth,
	int dims
);

texture_t *texture_cubemap
(
	uint width,
	uint height,
	uint depth_buffer
);

int texture_2D_resize(texture_t *self, int width, int height);

void texture_bind(texture_t *self, int tex);
int texture_target(texture_t *self, texture_t *depth, int fb);
int texture_target_sub(texture_t *self, int width, int height,
		texture_t *depth, int fb);
void texture_destroy(texture_t *self);

void texture_set_xy(texture_t *self, int x, int y,
		GLubyte r, GLubyte g, GLubyte b, GLubyte a);
void texture_set_xyz(texture_t *self, int x, int y, int z,
		GLubyte r, GLubyte g, GLubyte b, GLubyte a);
void texture_update_gl(texture_t *self);

void texture_update_brightness(texture_t *self);
uint texture_get_pixel(texture_t *self, int buffer, int x, int y,
		float *depth);

void texture_draw_id(texture_t *self, int tex);

int buffer_new(const char *name, int is_float, int dims);

#endif /* !TEXTURE_H */