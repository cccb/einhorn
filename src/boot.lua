
local fblib = debug.getregistry().framebuffer
function fblib:mappixel(fn)
    local width, height = self:getsize()
    for y = 1, height do
        for x = 1, width do
            self:setpixel(x, y, fn(x, y, self:getpixel(x,y)))
        end
    end
end


-- GLSL Functions
function fract(x)
    return x - math.floor(x)
end




