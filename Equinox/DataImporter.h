#pragma once
#include <memory>

class Level;
class DataImporter
{
public:
	DataImporter() = default;
	~DataImporter() = default;

	std::shared_ptr<Level> ImportLevel(const char* path, const char* file) const;
};
