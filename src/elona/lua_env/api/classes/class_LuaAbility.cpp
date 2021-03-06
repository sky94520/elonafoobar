#include "class_LuaAbility.hpp"

#include "../../../ability.hpp"
#include "../../../character.hpp"



LUA_API_OPTOUT_SOL_AUTOMAGIC(elona::lua::api::classes::LuaAbility)



/**
 * @luadoc
 *
 * A reference to a skill on a LuaCharacter.
 */
namespace elona::lua::api::classes::class_LuaAbility
{

#define ELONA_LUA_SKILL_PROPERTY_READONLY(name) \
    sol::property([](classes::LuaAbility& a) { \
        if (!a.is_valid()) \
        { \
            return 0; \
        } \
        auto& ref = *a.get_ref(); \
        return ref.get_skill(a.skill_id).name; \
    })

#define ELONA_LUA_SKILL_PROPERTY(name) \
    sol::property( \
        [](classes::LuaAbility& a) { \
            if (!a.is_valid()) \
            { \
                return 0; \
            } \
            auto& ref = *a.get_ref(); \
            return ref.get_skill(a.skill_id).name; \
        }, \
        [](classes::LuaAbility& a, int i) { \
            if (!a.is_valid()) \
            { \
                return; \
            } \
            auto& ref = *a.get_ref(); \
            ref.get_skill(a.skill_id).name = i; \
        })

void bind(sol::state& lua)
{
    auto LuaAbility = lua.new_usertype<classes::LuaAbility>(
        "LuaAbility", sol::no_constructor);

    /**
     * @luadoc experience field num
     *
     * [R] The skill's current experience. To modify it, use
     * LuaCharacter.gain_skill_exp, which will modify level and potential
     * appropriately.
     * @see LuaCharacter.gain_skill_exp
     */
    LuaAbility.set("experience", ELONA_LUA_SKILL_PROPERTY_READONLY(experience));

    /**
     * @luadoc level field num
     *
     * [RW] The skill's current level.
     */
    LuaAbility.set("level", ELONA_LUA_SKILL_PROPERTY(level));

    /**
     * @luadoc base_level field num
     *
     * [RW] The skill's base level.
     */
    LuaAbility.set("base_level", ELONA_LUA_SKILL_PROPERTY(base_level));

    /**
     * @luadoc potential field num
     *
     * [RW] The skill's potential.
     */
    LuaAbility.set("potential", ELONA_LUA_SKILL_PROPERTY(potential));
}

#undef ELONA_LUA_SKILL_PROPERTY

} // namespace elona::lua::api::classes::class_LuaAbility
