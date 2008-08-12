$:.unshift ".."
require "ggi"

class ButtonMorph
  attr_accessor :state

  def initialize(x, y, w, h, text)
    @x, @y, @w, @h = x, y, w, h
    @text = text
    @state = :up
  end

  def drawButton(visual, x, y, w, h, border_color, fill_color, highlight_color, shadow_color)
    visual.setForegroundColor border_color
    visual.drawRect x, y, w, h

    visual.setForegroundColor fill_color
    visual.drawBox x+1, y+1, w-2, h-2

    visual.setForegroundColor highlight_color
    visual.drawHLine x+1, y+1, w-2
    visual.drawVLine x+1, y+1, h-2

    visual.setForegroundColor shadow_color
    visual.drawHLine x+1, y+h-2, w-2
    visual.drawVLine x+w-2, y+1, h-2

    visual.setBackgroundColor fill_color
    visual.setForegroundColor visual.rgb(0.0, 0.0, 0.0)

    cx, th = visual.getCharSize
    tw = @text.size * cx
    if @state == :up
      visual.puts(x+(w - tw)/2, y+(h - th)/2, @text)
    else
      visual.puts(x+(w - tw)/2+1, y+(h - th)/2+1, @text)
    end
  end

  def includes?(x, y)
    x >= @x && x < @x+@w && y >= @y && y < @y+@h
  end

  def drawOn(visual)
    visual.setClipping @x, @y, @x+@w, @y+@h

    border_color = visual.rgb 0.0, 0.0, 0.0
    fill_color = visual.rgb 0.8, 0.8, 0.8
    highlight_color = visual.rgb 0.9, 0.9, 0.9
    shadow_color = visual.rgb 0.5, 0.5, 0.5

    case @state
    when :up
      drawButton(visual, @x, @y, @w, @h, border_color, fill_color, highlight_color, shadow_color)
    when :down
      drawButton(visual, @x, @y, @w, @h, border_color, fill_color, shadow_color, highlight_color)
    end
  end
end

# if button needs redraw, check the whole scene, which else needs to be
# redrawn. 
#
# just issue a area_needs_redraw(self.area): this should trigger the redraw
#
# * process events:
# * check if a morph needs redraw

GGI::Visual.openSimpleWith(w=800, h=600) do 
  #setBackgroundColor(rgb 0.1,0.1,0.1)
  setForegroundColor(rgb 1.0,1.0,1.0)
  fillScreen

  but = ButtonMorph.new(100,100,100,25, "Hallo Leute")
  but.drawOn(self)

  mask = GGI::EmKey | GGI::EmPtrMove | GGI::EmPtrButton
  loop {
    eventPoll(mask, nil)
    n = eventsQueued(mask)
    n.times {
      ev = eventRead(mask)
      case ev
      when GGI::Event::PtrMove
        if but.includes?(ev.x, ev.y)
          but.state = :down
          but.drawOn(self)
        else
          if but.state == :down
            but.state = :up
            but.drawOn(self)
          end
        end
        #print "#{ ev.x } #{ ev.y }\n"
      when GGI::Event::PtrButton
        exit
        #p ev.button
      when GGI::Event::Key
        #exit
      end
    }
  }
end
