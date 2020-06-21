#pragma once

#include <array>



namespace elona
{

struct Position;
struct Character;

constexpr int fov_max = 15; // in diameter

extern std::array<std::array<int, 2>, fov_max + 2> fovlist;

// Returns wheather the PC can see  the position or the character.
bool is_in_fov(const Position&);
bool is_in_fov(const Character& chara);

/**
 * Check Line of Sight (LOS) from @a p1 to @a p2.
 *
 * @param p1 The starting position to check LOS.
 * @param p2 The end position to check LOS.
 * @return True if one can see @a p2 standing at @a p1; otherwise false.
 */
bool fov_los(const Position& p1, const Position& p2);

int get_route(int = 0, int = 0, int = 0, int = 0);
void init_fovlist();

int breath_list(const Position& source_pos);
int route_info(int&, int&, int = 0);

} // namespace elona
