#include "Animator.h"


std::vector<glm::vec4> Animator::create_database(const glm::vec4& offset, int count, float speed)
{
	// Clearing old datas
	m_Offsets.clear();
	m_Database.clear();

	generate_offsets(offset, count);

	// Creating the database
	float time = speed / 10.0f;
	for (float i = 0.0f; i < m_Offsets.size(); i += time)
	{
		m_Database.push_back(m_Offsets[(int)i]);
	}

	return m_Database;
}

void Animator::generate_offsets(glm::vec4 offset, int count)
{
	for (int i = 0; i < count; i++)
	{
		m_Offsets.push_back({ offset.x, offset.y, offset.z, offset.w });
		offset.x++;
	}
}

void Animator::change_state(const std::string& new_state)
{
	if (state != new_state)
	{
		state = new_state;
		frame = 0;
	}
}

void Animator::change_side(const std::string& new_side)
{
	if (side != new_side)
	{
		side = new_side;
		frame = 0;
	}
}
