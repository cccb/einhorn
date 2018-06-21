

Vec2 = {}
Vec2.__index = Vec2

function Vec2:__add(b)
    return vec2(self.x + b.x, self.y + b.y)
end

function Vec2:__sub(b)
    return vec2(self.x - b.x, self.y - b.y)
end

function Vec2:length()
    return math.sqrt(self.x^2 + self.y^2)
end

function Vec2:normalize()
    return self / self:length()
end

function Vec2:__tostring()
    return "(" .. self.x .. ", " .. self.y .. ")"
end


function vec2(x, y)
    local v = {x=x or 0, y=y or 0}
    setmetatable(v, Vec2)

    return v
end


v1 = vec2(23, 42)
v2 = vec2(1)

print(v1:length())
print(v2:length())
