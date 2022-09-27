#pragma once

#include "Dependency.h"

#include <vector>
#include <string>

class Animator
{
private:
	std::vector<glm::vec4> m_Offsets;
	std::vector<glm::vec4> m_Database;

public:
	std::string state = "idle";
	std::string side = "left";
	int frame = 0;

public:
	Animator() {}
	~Animator() {}

	std::vector<glm::vec4> create_database(const glm::vec4& offset, int count, float speed);
	void change_state(const std::string& new_state);
	void change_side(const std::string& new_side);

private:
	void generate_offsets(glm::vec4 offset, int count);

};