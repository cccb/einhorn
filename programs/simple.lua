
function render(buffer, t)
    buffer:setpixel(1, 1, 1, 1, math.sin(t))
    buffer:setpixel(1, 2, 0, 1, 0)
    buffer:setpixel(1, 3, 1, 0, 1)
    buffer:setpixel(1, 5, 1, 0, 0)
    buffer:setpixel(1, 6, 0, 1, 0)
    buffer:setpixel(1, 7, 0, 0, 1)
    buffer:setpixel(1, 8, 0, 0, 0, 1)
    buffer:setpixel(1, 9, 0.4, 0, 0, 0)
end

