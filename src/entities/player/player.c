#include "player.h"

player_t * create_player(char *name) {
    player_t * player = malloc(sizeof(player_t));
    player->name = malloc(sizeof(char)*strlen(name)+1);
    strcpy(player->name, name);

    player->hp = 200u;
    player->hp_max = 200u;
    player->mana = 100u;
    player->mana_max = 100u;
    player->xp = 0u;
    player->level = 1u;
    player->base_attack = 5u;
    player->base_defense = 2u;
    player->gold = 100u;
    player->action_points = (unsigned short)3;

    // TODO : add one spell at least for the beginning, see with @TomBrd
    player->offensive_spell = NULL;
    player->defensive_spell = NULL;
    player->healing_spell = NULL;

    player->chosen_armor = NULL;
    player->chosen_weapon = NULL;

    player->inventory = create_inventory();
    player->stats = create_stats();

    return player;
}

unsigned int compute_xp_needed(unsigned int level){
    int total_xp = 0;
    for (int i = 1; i <= level; ++i) {
        total_xp += 100 * i;
    }
    return (unsigned int)total_xp;
}

void level_up(player_t * player) {

}

void check_level_up(player_t * player){
    if(compute_xp_needed(player->level+1) <= player->xp){
        level_up(player);
    }
}

int save_player(sqlite3 *db, player_t *player) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    // TODO: update the player's stats in the database
    // TODO: update the player's inventory in the database
    // TODO: update the player's weapons in the database : weapons_in_inventory
    // TODO: update the player's armors in the database : armors_in_inventory


    /* This function take in parameter the database,
     * the sql command to update the player,
     * the length of the sql command (if -1, the command is read up to the first null terminator),
     * a pointer to the statement object and
     * a pointer to the unused portion of the sql command
    */
    int rc = sqlite3_prepare_v2(db, update_player_sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    sqlite3_bind_int64(stmt, 1, player->hp);
    sqlite3_bind_int64(stmt, 2, player->hp_max);
    sqlite3_bind_int64(stmt, 3, player->mana);
    sqlite3_bind_int64(stmt, 4, player->mana_max);
    sqlite3_bind_int64(stmt, 5, player->xp);
    sqlite3_bind_int64(stmt, 6, player->level);
    sqlite3_bind_int64(stmt, 7, player->base_attack);
    sqlite3_bind_int64(stmt, 8, player->base_defense);
    sqlite3_bind_int64(stmt, 9, player->gold);
    sqlite3_bind_int64(stmt, 10, 1);
    sqlite3_bind_int64(stmt, 11, 1);
    sqlite3_bind_int64(stmt, 12, 1);
    sqlite3_bind_int64(stmt, 13, player->inventory->capacity);
    sqlite3_bind_int64(stmt, 14, player->inventory->nb_weapons);
    sqlite3_bind_int64(stmt, 15, player->inventory->nb_armors);
    sqlite3_bind_int64(stmt, 16, player->inventory->nb_mana_potions);
    sqlite3_bind_int64(stmt, 17, player->inventory->nb_health_potions);
    sqlite3_bind_int64(stmt, 18, player->id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;

}

void give_exp(player_t * player, unsigned int amount){
    player->xp += amount;
    check_level_up(player);
}