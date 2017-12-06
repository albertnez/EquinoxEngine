#pragma once

#include "ModuleAnimation.h"

#include <memory>

class Level;
class DataImporter
{
public:
	DataImporter() = default;
	~DataImporter() = default;

	std::shared_ptr<Level> ImportLevel(const char* path, const char* file) const;
	std::shared_ptr<Animation> ImportAnimation(const char* path, const char* file) const;
};
