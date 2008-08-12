$:.unshift ".."
require "ggi"

GGI::init

vis = GGI::Visual.open
w = 800
h = 600
vis.setSimpleMode(w, h, 1, GGI::GGI_AUTO)
vis.with do
 
  mask = GGI::EmKey | GGI::EmPointer
  setClipping(100, 100, w-100, h-100)
  loop do
    while eventPoll(mask, 0) == 0
      setForegroundColor(mapColor(GGI::Color[(rand*0xFFFF).to_i, (rand*0xFFFF).to_i, (rand*0xFFFF).to_i, 0]))
      drawLine((rand*w).to_i, (rand*h).to_i, (rand*w).to_i, (rand*h).to_i) 
    end

    n = eventsQueued(mask)
    n.times do
      ev = eventRead(mask)
      case ev
      when GGI::Event::PtrButton
        p ev.button
      when GGI::Event::Key
        exit
      end
    end
  end
end


GGI::exit
