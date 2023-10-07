#include "inventory_screen.h"
#include "../sdl_utils/sdl_utils.h"
#include "../utils/router.h"
#include "math.h"
#include "../utils/array.h"

int display_inventory(game_window_t *game_window, inventory_t *inventory, section_options active_section, category_options active_category, action_options active_action, unsigned short active_item);
int display_nothing_to_see(SDL_Renderer *renderer, int rect_x, int rect_y);

int inventory_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        printf("Cannot display inventory : no game window\n");
        return EXIT_FAILURE;
    }
    if (!player) {
        printf("Cannot display inventory : no player\n");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool quit = false;
    unsigned short active_section = CATEGORIES;
    unsigned short active_category = WEAPONS;
    unsigned short active_action = USE;
    unsigned short active_item = 0;
    while (!quit){
        // TODO: link to next screens
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT;
            }
            if (e.type == SDL_KEYDOWN){
                switch (active_section){
                    case CATEGORIES:
                        // navigating through the categories
                        if (e.key.keysym.sym == SDLK_DOWN && active_category != MANA_POTIONS) {
                            active_category++;
                            active_item = 0;
                        }
                        if (e.key.keysym.sym == SDLK_UP && active_category != GO_BACK) {
                            active_category--;
                            active_item = 0;
                        }
                        // accessing the chosen category
                        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                            switch (active_category) {
                                case GO_BACK:
                                    return EXIT_SUCCESS;

                                case ARMORS:
                                case WEAPONS:
                                    active_section++;
                                    break;

                                case MANA_POTIONS:
                                case HEALTH_POTIONS:
                                    active_section = ACTIONS;
                                    break;
                            }
                        }
                        break;

                    case ITEMS:
                        if (e.key.keysym.sym == SDLK_LEFT) {
                            // going back to categories when pressing the left arrow on the left column
                            if (active_item % 3 == 0) {
                                active_section--;
                            } else {
                                active_item--;
                            }
                        }
                        if (e.key.keysym.sym == SDLK_RIGHT) {
                            if (active_item % 3 != 2) {
                                active_item++;
                            }
                        }
                        if (e.key.keysym.sym == SDLK_DOWN) {
                            if ((active_category == WEAPONS && active_item + 3 < player->inventory->nb_weapons) ||
                                (active_category == ARMORS && active_item + 3 < player->inventory->nb_armors)) {
                                active_item += 3;
                            }
                        }
                        if (e.key.keysym.sym == SDLK_UP) {
                            if ((active_category == WEAPONS && active_item - 3 >= 0) ||
                                (active_category == ARMORS && active_item - 3 >= 0)) {
                                active_item -= 3;
                            }
                        }
                        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                            active_section++;
                        }
                        break;

                    case ACTIONS:
                        if (e.key.keysym.sym == SDLK_LEFT) {
                            active_section = CATEGORIES;
                        }
                        if (e.key.keysym.sym == SDLK_DOWN && active_action != THROW_AWAY) {
                            active_action++;
                        }
                        if (e.key.keysym.sym == SDLK_UP && active_action != USE) {
                            active_action--;
                        }
                        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                            if (active_action == USE) {
                                if (active_category == WEAPONS) {
                                    player->chosen_weapon = get_value_at_index(player->inventory->weaponsHead, active_item);
                                }
                                if (active_category == ARMORS) {
                                    player->chosen_armor = get_value_at_index(player->inventory->armorsHead, active_item);
                                }
                            }
                            if (active_action == THROW_AWAY) {
                                if (active_category == WEAPONS) {
                                    delete_node(&(player->inventory->weaponsHead), active_item);
                                }
                                if (active_category == ARMORS) {
                                    delete_node(&(player->inventory->armorsHead), active_item);
                                }
                            }
                        }
                        break;
                }
            }
        }
        display_inventory(game_window, player->inventory, active_section, active_category, active_action, active_item);
    }
    return EXIT_SUCCESS;
}

int display_inventory(game_window_t *game_window, inventory_t *inventory, section_options active_section, category_options active_category, action_options active_action, unsigned short active_item) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );

    int unit = min(window_width, window_height) / 3;
    int unit_padding = unit / 10;

    int category_size = unit / 4;

    const int categories_count = 5;
    SDL_Rect categories[5];
    for (int i = 0; i < categories_count; i++) {
        categories[i] = (SDL_Rect) {
            unit_padding,
            i * category_size + unit_padding * (2*i + 1),
            category_size,
            category_size
        };
    }

    draw_image_in_rectangle(game_window->renderer, categories[GO_BACK], "../assets/inventory_categories/go_back.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[WEAPONS], "../assets/inventory_categories/weapons.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[ARMORS], "../assets/inventory_categories/armors.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[HEALTH_POTIONS], "../assets/inventory_categories/health_potions.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[MANA_POTIONS], "../assets/inventory_categories/mana_potions.png", NORTH);

    if (active_section == CATEGORIES) {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
        SDL_RenderPresent(game_window->renderer);
        return EXIT_SUCCESS;
    } else {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 0, 0, 255}, game_window->renderer);
    }

    SDL_Rect items_container = (SDL_Rect) {
            category_size + 2 * unit_padding,
            unit_padding,
            3 * unit - 2 * unit_padding,
            3 * unit - 2 * unit_padding
    };

    const int items_count = 9;
    SDL_Rect *items = get_rectangle_grid(9, &items_container);
//    for (int i = 0; i < items_count; i++) {
//        draw_thick_rect(items[i], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
//    }

    int first_item_to_print = (active_item / items_count) * items_count;
    switch (active_category) {
        case WEAPONS:
            if (inventory->nb_weapons == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            for (int i = 0; i < items_count; i++) {
                weapon_t *weapon_to_print = get_value_at_index(inventory->weaponsHead, first_item_to_print + i);
                if (weapon_to_print == NULL) {
                    break;
                }
                draw_thick_rect(items[i], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
                draw_image_in_rectangle(game_window->renderer, items[i], weapon_to_print->image_path, NORTH);
            }
            break;

        case ARMORS:
            if (inventory->nb_armors == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armorsHead, first_item_to_print + i);
                if (armor_to_print == NULL) {
                    break;
                }
                draw_thick_rect(items[i], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
                draw_image_in_rectangle(game_window->renderer, items[i], armor_to_print->image_path, NORTH);
            }
            break;

        case HEALTH_POTIONS:
            if (inventory->nb_health_potions == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            break;

        case MANA_POTIONS:
            if (inventory->nb_mana_potions == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            break;
    }


    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

int display_nothing_to_see(SDL_Renderer *renderer, int rect_x, int rect_y) {
    SDL_Texture *nothing_to_see_texture = get_string_texture(
            renderer,
            "Nothing to see here",
            "../assets/PixelifySans-Regular.ttf",
            24,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!nothing_to_see_texture) {
        return EXIT_FAILURE;
    }

    int nothing_to_see_width = 0;
    int nothing_to_see_height = 0;
    SDL_QueryTexture(nothing_to_see_texture, NULL, NULL, &nothing_to_see_width, &nothing_to_see_height);

    SDL_Rect nothing_to_see_rect = (SDL_Rect) {
            rect_x,
            rect_y,
            nothing_to_see_width,
            nothing_to_see_height
    };

    SDL_RenderCopy(renderer, nothing_to_see_texture, NULL, &nothing_to_see_rect);
    return EXIT_SUCCESS;
}