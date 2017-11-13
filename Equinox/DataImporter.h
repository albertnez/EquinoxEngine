#pragma once

class Level;
class DataImporter
{
public:
	DataImporter() = default;
	~DataImporter() = default;

	Level* ImportLevel(const char* path, const char* file) const;
};
