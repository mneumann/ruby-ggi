$:.unshift ".."
require "ggi"

GGI::Visual.openSimpleWith(w=800, h=600) do 
  mask = GGI::EmKey | GGI::EmPointer
  setClipping(100, 100, w-100, h-100)
  loop {
    while eventPoll(mask, 0) == 0
      setForegroundColor rgb(rand, rand, rand)
      drawLine((rand*w).to_i, (rand*h).to_i, (rand*w).to_i, (rand*h).to_i) 
    end

    n = eventsQueued(mask)
    n.times {
      ev = eventRead(mask)
      case ev
      when GGI::Event::PtrButton
        p ev.button
      when GGI::Event::Key
        exit
      end
    }
  }
end
