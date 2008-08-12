$:.unshift ".."
require "ggi"

vis = GGI::Visual.openWithInit
vis.setSimpleMode(w=800, h=600, 1, GGI::GGI_AUTO)#GGI::GT_CONSTRUCT(24, GGI::GT_TRUECOLOR, 32))
vis.with do
  mask = GGI::EmKey | GGI::EmPointer
  setClipping(100, 100, w-100, h-100)

  loop do
    while eventPoll(mask, 0) == 0
      setForegroundColor(mapColor(GGI::Color[(rand*0xFFFF).to_i, (rand*0xFFFF).to_i, (rand*0xFFFF).to_i, 0]))
      drawBox(rand(w), rand(h), rand(w), rand(h)) 
    end

    n = eventsQueued(mask)
    n.times do
      ev = eventRead(mask)
      case ev
      when GGI::Event::PtrButton
        exit
      when GGI::Event::Key
      end
    end
  end
end
