#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <ecm.h>
#include <systems/physics.h>

typedef struct
{
	vec3_t normal;
	float depth;
} contact_t;

typedef struct
{
	c_t super; /* extends c_t */

	collider_cb costum;
} c_rigid_body_t;

DEF_CASTER(ct_rigid_body, c_rigid_body, c_rigid_body_t)

c_rigid_body_t *c_rigid_body_new(collider_cb costum);
void c_rigid_body_register(ecm_t *ecm);
int c_rigid_body_intersects(c_rigid_body_t *self, c_rigid_body_t *other,
		contact_t *contact);

#endif /* !RIGID_BODY_H */
