#include "entities.h"

array_node_t  *weapons = NULL;
array_node_t  *spells = NULL;
array_node_t  *armors = NULL;
array_node_t  *monsters = NULL;

void init_entities(sqlite3 *db) {

    weapons = create_struct_from_db(db, "SELECT * FROM WEAPON", create_weapon_from_db, sizeof (weapon_t));
    spells = create_struct_from_db(db, "SELECT * FROM SPELL", create_spell_from_db, sizeof (spell_t));
    armors = create_struct_from_db(db, "SELECT * FROM ARMOR", create_armor_from_db, sizeof (armor_t));
    monsters = create_struct_from_db(db, "SELECT * FROM MONSTER", create_monster_from_db, sizeof (monster_t));

}