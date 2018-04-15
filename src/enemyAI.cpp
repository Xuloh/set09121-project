#include "enemyAI.h"

#include "Tilemap.h"
#include "tilemap-system.h"
#include <array>
#include <queue>

using namespace std;
using namespace sf;

class Node
{
private:
	Vector2i _pos;
	int _level;
	int _priority;

public:
	Node() = default;
	Node(const Vector2i& pos, int level, int priority)
		: _pos(pos), _level(level), _priority(priority) {}

	const Vector2i& getPos() const { return _pos; }

	int getLevel() const { return _level; }

	int getPriority() const { return _priority; }

	unsigned int estimate(const Vector2i dest) const
	{
		Vector2i delta = dest - _pos;
		return static_cast<unsigned int>(length(delta));
	}

	void updatePriority(const Vector2i& dest)
	{
		_priority = _level + estimate(dest) * 10;
	}

	void nextLevel() { _level += 10; }

	bool operator<(const Node& other) const
	{
		return _priority > other._priority;
	}
};

vector<Vector2i> pathFind(Vector2i start, Vector2i finish)
{
	static std::array<sf::Vector2i, 4> directions = { Vector2i(1, 0), Vector2i(0, 1), Vector2i(-1, 0), Vector2i(0, -1) };

    const auto& tilemap = tilemap::getTilemap();
	vector<vector<bool>> closed_nodes_map(tilemap->getWidth());
	vector<vector<int>> open_nodes_map(tilemap->getWidth());
	vector<vector<Vector2i>> direction_map(tilemap->getWidth());

	// Queue of nodes to test. Priority ordered.
	// We need two for when we redirect and copy the path
	priority_queue<Node> queue[2];
	// Index of current queue
	size_t queue_index = 0;

	// Initialise the lookup maps. Initially everything looks traversable and
	// no route has been planned.
	for (size_t y = 0; y < tilemap->getHeight(); ++y)
	{
		for (size_t x = 0; x < tilemap->getWidth(); ++x)
		{
			closed_nodes_map[x].push_back(false);
			open_nodes_map[x].push_back(0);
			direction_map[x].emplace_back(Vector2i(0, 0));
		}
	}

	// Add start node to the queue - we will check first
	Node n0(start, 0, 0);
	// Update the priority node
	n0.updatePriority(finish);
	// Push onto the current queue of nodes to check
	queue[queue_index].push(n0);
	// And update the value in the open_nodes_map
	open_nodes_map[start.x][start.y] = n0.getPriority();

	// Loop until queue is empty
	while (!queue[queue_index].empty())
	{
		auto tmp = queue[queue_index].top();
		n0 = Node(tmp.getPos(), tmp.getLevel(), tmp.getPriority());
		auto pos = n0.getPos();

		// Remove node from open list
		queue[queue_index].pop();
		open_nodes_map[pos.x][pos.y] = 0;
		// Mark on closed node list
		closed_nodes_map[pos.x][pos.y] = true;

		// Check if reached goal
		if (pos == finish)
		{
			vector<Vector2i> path;
			while (!(pos == start))
			{
				// Get the direction
				auto dir = direction_map[pos.x][pos.y];
				// Add the current position to the path
				path.push_back(pos);
				pos += dir;
			}
			// Path is inverted
			reverse(begin(path), end(path));
			// Return path
			return path;
		}
		// Not reached goal. Check for directions
		for (auto& dir : directions)
		{
			auto next = pos + dir;

			// Check if nect is valid
			if (!(next.x < 0 || next.x > tilemap->getWidth() || next.y < 0 || next.y > tilemap->getHeight() ||
                tilemap->getTile(Vector2u(next.x, next.y)) == tilemap::WALL ||
				closed_nodes_map[next.x][next.y]))
			{
				// Generate new node
				Node m0(next, n0.getLevel(), n0.getPriority());
				m0.nextLevel();
				m0.updatePriority(finish);

				// Check if new node has no priority
				if (open_nodes_map[next.x][next.y] == 0)
				{
					// Update epriority and add to the queue
					open_nodes_map[next.x][next.y] = m0.getPriority();
					queue[queue_index].push(m0);
					// Put the opposite direction into the direction map.
					// We work backwards
					direction_map[next.x][next.y] = dir * -1;
				}
				// If it has a priority, check if the priority is better on the new route
				else if (open_nodes_map[next.x][next.y] > m0.getPriority())
				{
					// Update priority informations
					open_nodes_map[next.x][next.y] = m0.getPriority();
					// Update the direction map with the inverse direction
					direction_map[next.x][next.y] = dir * -1;
					// Now replace the node via swapping across the lists and ignoring the old one
					while (queue[queue_index].top().getPos() != next)
					{
						queue[1 - queue_index].push(queue[queue_index].top());
						queue[queue_index].pop();
					}
					queue[queue_index].pop();
					if (queue[queue_index].size() > queue[1 - queue_index].size())
						queue_index = 1 - queue_index;
					while (!queue[queue_index].empty())
					{
						queue[1 - queue_index].push(queue[queue_index].top());
						queue[queue_index].pop();
					}
					queue_index = 1 - queue_index;
					queue[queue_index].push(m0);
				}
			}
		}
	}
	return vector<Vector2i>();
}
