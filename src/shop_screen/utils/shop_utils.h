#ifndef DOOM_DEPTH_C_SHOP_UTILS_H
#define DOOM_DEPTH_C_SHOP_UTILS_H

#include "../shop_screen.h"
#include "stdbool.h"
#include "../../utils/items_management/types.h"

/**
 * @brief Returns the number of items existing in the game in a given category
 *
 * @param category
 * @returns number of items in category or -1
 */
int get_items_count(category_options category);

/**
 * @brief Returns the quantity of gold a player has in a string
 *
 * @param player
 */
char *player_gold_to_string(player_t *player);

#endif //DOOM_DEPTH_C_SHOP_UTILS_H