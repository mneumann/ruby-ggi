$:.unshift ".."
require "ggi"

GGI::Visual.openSimple(w=800, h=600) do |vis| 
  loop {
    vis.fg = vis.rgb(rand, rand, rand) 
    x = [rand*800, rand*800, rand*800] 
    y = [rand*600, rand*600, rand*600] 
    vis.filledPoly(x, y, 3) 
    break if vis.kbhit
  }
end
