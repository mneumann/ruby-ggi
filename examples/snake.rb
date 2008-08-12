# translation from Python GGI binding Piggy

$:.unshift ".."
require "ggi"

def clamp(x, a, b)
  return b if x > b 
  return a if x < a 
  return x
end

def drawBox(v, x1, y1, x2, y2, c)
  v.fg = c
  v.hline x1, y1, x2-x1
  v.hline x1, y2, x2-x1
  v.vline x1, y1, y2-y1
  v.vline x2, y1, y2-y1
end

class Snake
  def initialize(x, y, x1, y1, x2, y2, length=10, speed=10)
    @max = length
    @body = [ [x, y, x, y, rand(0xffffff)] ]
    @speed = speed
    @xmin, @xmax, @ymin, @ymax = x1, x2, y1, y2
  end

  def draw(v)
    if @body.length > @max
      i = @body.shift
      v.fg = 0
      v.line(*i[0..-2])
    end
    for i in @body
      v.fg = i[4]
      v.line(*i[0..-2])
    end
  end

  def evolve
    ex, ey = @body[-1][2..3]
    @body << [ex, ey, clamp(ex+rand(2*@speed)-@speed, @xmin, @xmax),
                      clamp(ey+rand(2*@speed)-@speed, @ymin, @ymax),
                      rand(0xffffff)]
  end
end

vis = GGI::Visual.openSimple(w=400, h=400)
mask = GGI::EmKey | GGI::EmPointer

snakes = [ Snake.new(30,30, 20, 60, 150, 200, 10, 30),
           Snake.new(200, 200, 0,0, w, h, 20) ]  

px = w/2
py = h/2
selected = nil

box = nil
updatebox = false 

loop do
  if vis.eventPoll(mask, 0) != 0
    vis.eventsQueued(mask).times do
      ev = vis.eventRead(mask)
      case ev
      when GGI::Event::Key
        exit
      when GGI::Event::PtrMove
        case ev.type
        when GGI::EvPtrAbsolute 
          next if ev.x >= w or ev.y >= h
          px, py = ev.x, ev.y 
        when GGI::EvPtrRelative
          px += ev.x
          py += ev.y
        else
          raise
        end
        updatebox = true if selected 
      when GGI::Event::PtrButton
        case ev.type
        when GGI::EvPtrButtonPress
          selected = [px, py]
        when GGI::EvPtrButtonRelease
          if selected and box
            snakes << Snake.new(box[0], box[1], box[0], box[1], box[2], box[3])
            updatebox = true
            selected = nil
          end
        end
      end
    end 
  end # event handler

  if updatebox and box
    drawBox(vis, box[0], box[1], box[2], box[3], 0)
    updatebox = false
  end
  if selected
    box = [ [px, selected[0]].min, [py, selected[1]].min,
            [px, selected[0]].max, [py, selected[1]].max ]
    drawBox(vis, box[0], box[1], box[2], box[3], 0xffffff)
  end

  for i in snakes
    i.evolve
    i.draw(vis)
  end
end
