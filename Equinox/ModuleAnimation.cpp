#include "ModuleAnimation.h"

ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
}

ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::CleanUp()
{
	AnimationsMap::iterator it = _animations.begin();
	for (auto element : _animations)
	{
		for (Channel* channel : element.second->Channels)
		{
			RELEASE(channel);
		}
		element.second.reset();
	}

	_animations.clear();

	return true;
}

std::shared_ptr<Animation> ModuleAnimation::CreateAnimation(const std::string& name)
{
	std::shared_ptr<Animation> animation = std::make_shared<Animation>();
	_animations[name] = animation;
	return animation;
}
