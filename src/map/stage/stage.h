#ifndef DOOM_DEPTH_C_STAGE_H
#define DOOM_DEPTH_C_STAGE_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../../entities/fight/fight.h"
#include "../../storage/json/json.h"

typedef enum {
    EMPTY,
    FIGHT,
    SHOP,
    TREASURE
} stage_type_t;

typedef struct stage_t stage_t;
struct stage_t {
    stage_type_t type;

    bool is_start; // true if the stage is where the player is placed when entering the map
    bool is_done; // true if the stage is not a fight or if the fight is done

    bool has_linked_map; // true if the user goes to a linked map when stepping on this stage
    char * linked_map_file_path;

    bool counted; // for the get_map_dimensions function

    fight_t *fight; // only if type == FIGHT

    // next stages
    stage_t *top;
    stage_t *right;
    stage_t *bottom;
    stage_t *left;
};

/**
 * @brief Converts a json into a stage (does all the required checks)
 *
 * @param json_map The json to convert into a stage
 * @error If the json is not a correct stage, will return NULL
 * @return The parsed stage or NULL
 */
stage_t * json_to_stage(Json * json_stage);

/**
 * @brief Computes and updates the dimensions of the given stage.
 *
 * @param stage Pointer to the stage whose dimensions are to be computed.
 * @param x Current x-coord of the stage.
 * @param y Current y-coord of the stage.
 * @param max_x Pointer to store the maximum x-coord.
 * @param max_y Pointer to store the maximum y-coord.
 * @param min_x Pointer to store the minimum x-coord.
 * @param min_y Pointer to store the minimum y-coord.
 */
void get_stage_dimensions(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y);

/**
 * @brief Gets a color according to the stage type
 *
 * @param stage The stage for which we are getting the color
 * @return The allocated color object or NULL in case of failure
 */
SDL_Color * get_stage_color(stage_t * stage);

#endif //DOOM_DEPTH_C_STAGE_H
