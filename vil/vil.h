#ifndef VISUAL_LOGIC_H
#define VISUAL_LOGIC_H

#include <ecs/ecm.h>
#include <utils/mafs.h>
#include <utils/khash.h>

typedef struct vitype_t vitype_t;
typedef struct vicall_t vicall_t;
typedef struct vil_t vil_t;

typedef void(*vitype_gui_cb)(vicall_t *call, void *data, void *nk);
typedef void(*visolver_cb)(vicall_t *call);

enum {
	V_IN = 0x001,
	V_OUT = 0x002,
	V_COL = 0x004,
	V_ALL = 0x007
} vicall_flags;

KHASH_MAP_INIT_INT(vitype, vitype_t *)

typedef struct vil_t
{
	khash_t(vitype) *types;
	void *user_data;
	visolver_cb solver;
} vil_t;

typedef union
{
	struct { uint8_t depth; uint8_t _[7]; };
	uint64_t val;
} slot_t;

void vil_context_init(vil_t *self);
vitype_t *vil_add_type(vil_t *ctx, const char *name,
		vitype_gui_cb builtin_gui, uint32_t builtin_size);
vitype_t *vil_get(vil_t *ctx, uint32_t ref);

void vicall_link(vicall_t *root, slot_t in_slot, slot_t out_slot,
		uint32_t field);

const char *vicall_name(const vicall_t *call);
void vicall_color(vicall_t *self, vec4_t color);

uint32_t vitype_gui(vitype_t *type, void *nk);
vicall_t *vitype_add(vitype_t *parent, vitype_t *type,
		const char *name, vec2_t pos, uint32_t flags);
void vitype_add_solver(vitype_t *type, const char *solvespace,
		visolver_cb solver);

#endif /* !VISUAL_LOGIC_H */
