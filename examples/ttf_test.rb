$:.unshift ".."
require "ggi"

GGI::Visual.openSimple(w=800, h=600) do |vis| 
  loop { 
    break if vis.kbhit
    vis.fg = vis.rgb(rand, rand, rand)
    vis.renderText("/usr/X11R6/lib/X11/fonts/TTF/luximb.ttf", rand(70), rand(800), rand(600), "Hello World")
  }
end
