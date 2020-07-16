#pragma once

#include <bitset>
#include <memory>
#include <vector>

#include "../util/range.hpp"
#include "consts.hpp"
#include "data/types/type_item.hpp"
#include "enums.hpp"
#include "eobject/eobject.hpp"
#include "position.hpp"
#include "serialization/macros.hpp"
#include "shared_id.hpp"


#define ELONA_OTHER_INVENTORIES_INDEX 1320
#define ELONA_ITEM_ON_GROUND_INDEX 5080
#define ELONA_MAX_ITEMS 5480


namespace elona
{


struct Enchantment
{
    int id = 0;
    int power = 0;

    bool operator==(const Enchantment& other) const noexcept
    {
        return id == other.id && power == other.power;
    }



    template <typename Archive>
    void serialize(Archive& ar)
    {
        /* clang-format off */
        ELONA_SERIALIZATION_STRUCT_BEGIN(ar, "Enchantment");

        ELONA_SERIALIZATION_STRUCT_FIELD(*this, id);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, power);

        ELONA_SERIALIZATION_STRUCT_END();
        /* clang-format on */
    }
};



struct Inventory;



struct InventorySlot
{
    Inventory* inventory;
    size_t index;
};



struct Item
{
private:
    using FlagSet = std::bitset<32>;


    // Index of this item into the global cdata array.
    // Used for communicating with legacy code that takes integer index
    // arguments. New code should pass Item& instead. Not serialized; set on
    // creation and load.
    int _index = -1;

    Inventory* _inventory = nullptr;

    friend Inventory;

public:
    Item();

    int index() const noexcept
    {
        return _index;
    }

    int global_index() const noexcept;

    Inventory* inventory() const noexcept
    {
        return _inventory;
    }

    InventorySlot inventory_slot() const noexcept;

    ObjId obj_id;

private:
    int number_ = 0;

public:
    int value = 0;
    int image = 0;
    ItemId id = ItemId::none;
    Quality quality = Quality::none;

private:
    Position _pos;

public:
    const Position& pos() const noexcept
    {
        return _pos;
    }


    void set_pos(const Position& new_pos);



    int weight = 0;
    IdentifyState identify_state = IdentifyState::unidentified;
    int count = 0;
    int dice_x = 0;
    int dice_y = 0;
    int damage_bonus = 0;
    int hit_bonus = 0;
    int dv = 0;
    int pv = 0;
    int skill = 0;
    CurseState curse_state = CurseState::none;
    int body_part = 0;
    int function = 0;
    int enhancement = 0;
    int own_state = 0;
    int color = 0;
    int subname = 0;
    int material = 0;
    int param1 = 0;
    int param2 = 0;
    int param3 = 0;
    int param4 = 0;
    int difficulty_of_identification = 0;
    int turn = 0;

private:
    FlagSet _flags;

public:
    std::vector<Enchantment> enchantments;


    void clear();

    bool almost_equals(const Item& other, bool ignore_position) const;

    // for identifying the type of a Lua reference
    static std::string lua_type()
    {
        return "LuaItem";
    }

    int number() const
    {
        return number_;
    }

    void set_number(int number_);
    void modify_number(int delta);
    void remove();


    data::InstanceId new_id() const
    {
        return *the_item_db.get_id_from_legacy(itemid2int(this->id));
    }


    void on_create();
    void on_remove();


#define ELONA_ITEM_DEFINE_FLAG_ACCESSOR(name, n) \
    bool name() const \
    { \
        return _flags[n]; \
    } \
    FlagSet::reference name() \
    { \
        return _flags[n]; \
    }

    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_acidproof, 1)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_fireproof, 2)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(has_charge, 4)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_precious, 5)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_aphrodisiac, 6)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(has_cooldown_time, 7)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_blessed_by_ehekatl, 8)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_stolen, 9)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_alive, 10)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_quest_target, 12)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_marked_as_no_drop, 13)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_poisoned, 14)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_eternal_force, 15)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_showroom_only, 16)
    ELONA_ITEM_DEFINE_FLAG_ACCESSOR(is_handmade, 17)

#undef ELONA_ITEM_DEFINE_FLAG_ACCESSOR



    bool operator==(const Item& other) const noexcept
    {
        return this == &other;
    }


    bool operator!=(const Item& other) const noexcept
    {
        return !(*this == other);
    }



    static void copy(const Item& from, Item& to)
    {
        const auto index_save = to._index;
        const auto inventory_save = to._inventory;
        to = from;
        to._index = index_save;
        to._inventory = inventory_save;
    }


private:
    Item(const Item&) = default;
    Item(Item&&) = default;
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;



public:
    template <typename Archive>
    void serialize(Archive& ar)
    {
        /* clang-format off */
        ELONA_SERIALIZATION_STRUCT_BEGIN(ar, "Item");

        ELONA_SERIALIZATION_STRUCT_FIELD(*this, obj_id);
        ELONA_SERIALIZATION_STRUCT_FIELD_WITH_NAME(*this, "number", number_);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, value);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, image);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, id);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, quality);
        ELONA_SERIALIZATION_STRUCT_FIELD_WITH_NAME(*this, "position", _pos);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, weight);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, identify_state);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, count);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, dice_x);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, dice_y);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, damage_bonus);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, hit_bonus);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, dv);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, pv);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, skill);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, curse_state);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, body_part);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, function);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, enhancement);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, own_state);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, color);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, subname);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, material);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, param1);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, param2);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, param3);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, param4);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, difficulty_of_identification);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, turn);
        ELONA_SERIALIZATION_STRUCT_FIELD_WITH_NAME(*this, "flags", _flags);
        ELONA_SERIALIZATION_STRUCT_FIELD(*this, enchantments);

        ELONA_SERIALIZATION_STRUCT_END();
        /* clang-format on */
    }
};



struct Character;



struct Inventory
{
public:
    using storage_type = std::vector<OptionalItemRef>;



    Inventory(size_t inventory_size, int inventory_id);


    const OptionalItemRef& at(size_t index)
    {
        return _storage.at(index);
    }

    void remove(size_t index)
    {
        _storage.at(index) = nullptr;
    }


    bool has_free_slot() const;
    optional<InventorySlot> get_free_slot();


    size_t size() const noexcept
    {
        return _storage.size();
    }


    void clear();



    /**
     * Exchange item @a a and @a b.
     * If they points to the same object, does nothing. For example, assume that
     * @a a is an apinut owned by you and @a b is a fake gold bar owned by an
     * NPC. After calling the function, @a a is owned by the NPC and @a b is
     * owned by you. NOTE: The content of each reference continues to refer to
     * the same object before the call.
     *
     * @param a one item
     * @param b the other item
     */
    static void exchange(const ItemRef& a, const ItemRef& b);


    static const OptionalItemRef& at(const InventorySlot& slot);

    static ItemRef create(const InventorySlot& slot);


    /**
     * Move all items in @a source to @a destination.
     */
    static void move_all(Inventory& source, Inventory& destination);



    struct const_iterator
    {
        friend Inventory;


        bool operator==(const const_iterator& other) const
        {
            return _itr == other._itr;
        }


        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }


        decltype(auto) operator*() const
        {
            return _itr->unwrap();
        }


        const_iterator& operator++()
        {
            ++_itr;
            skip_over_null_elements();
            return *this;
        }



    private:
        storage_type::const_iterator _itr;
        storage_type::const_iterator _end;


        const_iterator(
            storage_type::const_iterator itr,
            storage_type::const_iterator end)
            : _itr(itr)
            , _end(end)
        {
            skip_over_null_elements();
        }


        void skip_over_null_elements()
        {
            while (_itr != _end && _itr->is_null())
            {
                ++_itr;
            }
        }
    };



    const_iterator begin() const
    {
        return const_iterator(_storage.begin(), _storage.end());
    }

    const_iterator end() const
    {
        return const_iterator(_storage.end(), _storage.end());
    }


    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }



    int inventory_id() const noexcept
    {
        return _inventory_id;
    }


    optional<int> capacity() const noexcept
    {
        return _capacity;
    }


    void set_capacity(optional<size_t> new_capacity);



private:
    storage_type _storage;
    int _inventory_id;
    optional<size_t> _capacity;
};



struct AllInventory
{
private:
    using storage_type = std::vector<Inventory>;
    using iterator_type = storage_type::iterator;
    using iterator_pair_type = range::iterator_pair_t<iterator_type>;



public:
    AllInventory();


    ItemRef operator[](int index);


    Inventory& pc();
    Inventory& ground();
    Inventory& for_chara(const Character& chara);
    Inventory& by_index(int index);

    iterator_pair_type all();
    iterator_pair_type global();
    iterator_pair_type map_local();



private:
    std::vector<Inventory> _inventories;
};



extern AllInventory g_inv;



IdentifyState item_identify(const ItemRef& item, IdentifyState level);
IdentifyState item_identify(const ItemRef& item, int power);

void item_checkknown(const ItemRef& item);

/**
 * TODO
 */
ItemRef
item_separate(const ItemRef& item, const InventorySlot& slot, int number);

/**
 * TODO
 */
ItemRef item_copy(const ItemRef& item, const InventorySlot& slot);

void item_acid(const Character& owner, OptionalItemRef item = nullptr);

void itemturn(const ItemRef& item);
OptionalItemRef itemfind(int inventory_id, int matcher, int matcher_type = 0);
int itemusingfind(const ItemRef& item, bool disallow_pc = false);

enum class ItemFindLocation
{
    player_inventory,
    ground,
    player_inventory_and_ground,
};
OptionalItemRef item_find(
    int matcher,
    int matcher_type = 0,
    ItemFindLocation = ItemFindLocation::player_inventory_and_ground);

/**
 * Separate @a item's stack.
 * @param item the item to separate
 */
ItemRef item_separate(const ItemRef& stacked_item);

struct ItemStackResult
{
    // If `stacked` is false, `stacked_item` is set to `base_item`.
    bool stacked;
    ItemRef stacked_item;
};
ItemStackResult item_stack(
    int inventory_id,
    const ItemRef& base_item,
    bool show_message = false,
    optional<int> number = none);

void item_dump_desc(const ItemRef&);

bool item_fire(int owner, const OptionalItemRef& burned_item = nullptr);
void mapitem_fire(optional_ref<Character> arsonist, int x, int y);
bool item_cold(int owner, const OptionalItemRef& destroyed_item = nullptr);
void mapitem_cold(int x, int y);

bool inv_find(ItemId id, int owner);

int inv_getowner(const ItemRef& item);


void remain_make(const ItemRef& remain, const Character& chara);


void item_drop(
    const ItemRef& item_in_inventory,
    int num,
    bool building_shelter = false);
void item_build_shelter(const ItemRef& shelter);


enum class ItemDescriptionType : int
{
    normal = 0, // rgb(0, 0, 0)
    raises_skill = 1, // rgb(0, 100, 0)
    raises_stat = 2, // rgb(0, 0, 100)
    raises_resist = 3, // rgb(80, 100, 0)
    enchantment = 4, // rgb(80, 50, 0)
    weapon_info = 5, // rgb(0, 0, 0)
    armor_info = 6, // rgb(0, 0, 0)
    text = 7, // rgb(0, 0, 0)
    maintains_skill = 8, // rgb(0, 100, 100)
    negative_effect = 9, // rgb(180, 0, 0)

    small_font = -1,
    small_font_italic = -2,
};

size_t item_load_desc(const ItemRef& item);


int iequiploc(const ItemRef& item);

void item_db_set_basic_stats(const ItemRef& item, int legacy_id);
void item_db_get_description(const ItemRef& item, int legacy_id);
void item_db_get_charge_level(const ItemRef& item, int legacy_id);
void item_db_set_full_stats(const ItemRef& item, int legacy_id);
void item_db_on_read(Character& reader, const ItemRef& item, int legacy_id);
void item_db_on_zap(const ItemRef& item, int legacy_id);
void item_db_on_drink(
    Character& chara,
    const OptionalItemRef& item,
    int legacy_id);


std::vector<int> item_get_inheritance(const ItemRef& item);


void auto_identify();
void begintempinv();
void exittempinv();
bool cargocheck(const ItemRef& item);
ItemRef item_convert_artifact(
    const ItemRef& artifact,
    bool ignore_external_container = false);
void damage_by_cursed_equipments(Character& chara);
void dipcursed(const ItemRef& item);
int efstatusfix(int = 0, int = 0, int = 0, int = 0);
void equip_melee_weapon(Character& chara);
OptionalItemRef mapitemfind(const Position& pos, ItemId id);
std::string
itemname(const ItemRef& item, int number = 0, bool with_article = true);

} // namespace elona
