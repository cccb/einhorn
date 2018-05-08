

-- Simple render test

function render(buffer, t)
    print("Rendering... t: " .. t)

    -- framebuffer.debug(buffer);
    buffer:mappixel(function(x, y)
        local w, h = buffer:getsize()
        return x, y, w, t
    end)

    buffer:debug()
end

