

-- Initial port of the smooth colors
-- shader from the Treppe2000 examples

function render(buffer, t)
    buffer:mappixel(function (x, y)
        offset = y / 6.5 * math.pi

        r = 0.5 + 0.5 * math.cos(2.0 * t + 2.0 + offset)
        g = 0.5 + 0.5 * math.cos(2.0 * t + 3.0 + offset)
        b = 0.5 + 0.5 * math.cos(2.0 * t + 4.0 + offset)
        w = 0.1 

        return r, g, b, w
    end)
end

