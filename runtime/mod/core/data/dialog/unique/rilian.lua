local Chara = ELONA.require("core.Chara")
local GUI = ELONA.require("core.GUI")
local Internal = ELONA.require("core.Internal")
local Item = ELONA.require("core.Item")

local common = require("../common.lua")

return {
   root = "core.talk.unique.rilian",
   nodes = {
      __start = function()
         local flag = Internal.get_quest_flag("puppys_cave")
         if flag == 1000 then
            return "quest_completed"
         elseif flag == 0 then
            return "quest_ask"
         elseif flag == 1 then
            return "quest_check"
         end

         return "__IGNORED__"
      end,
      quest_completed = {
         text = {
            {"complete"},
         },
      },
      quest_ask = {
         text = {
            {"quest.dialog"},
         },
         choices = {
            {"quest_yes", "quest.choices.do_it"},
            {"quest_no", "__BYE__"}
         },
         default_choice = "quest_no"
      },
      quest_yes = {
         text = {
            common.journal_updated,
            {"quest.do_it"},
         },
         on_finish = function()
            Internal.set_quest_flag("puppys_cave", 1)
         end
      },
      quest_no = {
         text = {
            {"quest.bye"},
         },
      },
      quest_check = function()
         if Chara.find("core.poppy", "allies") == nil then
            return "quest_waiting"
         end

         return "quest_finish"
      end,
      quest_waiting = {
         text = {
            {"quest.waiting"}
         }
      },
      quest_finish = {
         text = {
            {"quest.end"},
         },
         on_finish = function()
            Item.create(Chara.player().position, "core.cooler_box", 0)
            Item.create(Chara.player().position, "core.gold_piece", 2500)
            Item.create(Chara.player().position, "core.platinum_coin", 2)

            common.quest_completed()

            Internal.set_quest_flag("puppys_cave", 1000)

            Chara.find("core.poppy", "allies"):vanquish()
            local poppy = Chara.create(31, 4, "core.poppy")
            poppy.role = 3
         end
      }
   }
}
