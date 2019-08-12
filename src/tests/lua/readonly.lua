require_relative("tests/lua/support/minctest")

lrun("test API tables are readonly", function()
        local Rand = require("game.Rand")
        lok(not pcall(function() Rand.rnd = nil end), "API table wasn't read only")
end)
