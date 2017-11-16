#include "Globals.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Entity.h"
#include <SDL.h>

using namespace std;

bool ShouldColide[6][6] =
{
	{ false, false, true,  true,  true,  false },
	{ false, false, true,  false, false, false },
	{ true,  true,  false, false, true,  false },
	{ true,  false, false, false, false, false },
	{ true,  false, true,  false, false, false },
	{ false, false, false, false, false, false }
};

ModuleCollision::ModuleCollision()
{
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate(float DeltaTime)
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update(float DeltaTime)
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		Collider* col = *it;
		for (list<Collider*>::iterator it2 = ++it; it2 != colliders.end(); ++it2)
		{
			Collider* other = *it2;
			if (ShouldColide[col->type][other->type] && col->CheckCollision(other->rect))
			{
				if (col->attached)
					col->attached->OnCollision(*col, *other);
				if (other->attached)
					other->attached->OnCollision(*other, *col);
			}
		}
	}
	// After making it work, review that you are doing the minumum checks possible

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		SDL_Color color = { 255, 0, 0, 0 };
		if ((*it)->type == PLAYER_ATTACK)
			color.g = 255;
		//App->renderer->DrawQuad((*it)->rect, color.r, color.g, color.b, 80);
		//we will move all the module to be compatible with OpenGL
	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const iRectangle3& rect, Entity* attached)
{
	Collider* ret = new Collider(rect);
	ret->attached = attached;

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const iRectangle3& r) const
{
	// Return true if the argument and the own rectangle are intersecting
	return rect.Overlaps(r);
}
