require "ggi.so"

module GGI
  class Color
    def self.[](*a)
      new(*a)
    end

    def inspect
      "#{ type }[#{ r }, #{ g }, #{ b }, #{ a }]"
    end
  end

  class Visual
    alias setForegroundColor setGCForeground
    alias setBackgroundColor setGCBackground
    alias getForegroundColor getGCForeground
    alias getBackgroundColor getGCBackground

    alias fg= setGCForeground
    alias fg  getGCForeground
    alias bg= setGCBackground
    alias bg  getGCBackground

    alias setClipping setGCClipping
    alias getClipping getGCClipping

    alias fillScreen fillscreen

    alias hline drawHLine
    alias vline drawVLine
    alias line drawLine

    def with(&block)
      instance_eval(&block)
    end

    @@ggi_init_called = false

    class << self
      alias __old_open open
      def open(display=nil)
        __old_open(display)
      end

      def openWithInit
        unless @@ggi_init_called
          GGI::init
          at_exit { GGI::exit }
          @@ggi_init_called = true
        end
        vis = open
        if block_given?
          begin
            block.call(vis)
          ensure
            vis.close
          end
        else
          vis
        end
      end

      def openSimpleWith(*a, &block)
        openSimple(*a) do |vis|
          vis.with(&block)
        end
      end

      def openSimple(width=400, height=400, &block)
        vis = openWithInit
        vis.setSimpleMode(width, height, 1, GGI::GGI_AUTO)
        if block_given?
          begin
            block.call(vis)
          ensure
            vis.close
          end
        else
          vis
        end
      end
    end

    def rgb(r=0.0, g=0.0, b=0.0)
      mapColor(GGI::Color[(0xFFFF*r).to_i, (0xFFFF*g).to_i, (0xFFFF*b).to_i, 0])
    end

    # flips double-buffer
    def flip
      setWriteFrame((getWriteFrame()+1) & 1)
      setDisplayFrame((getDisplayFrame()+1) & 1)
    end

  end # class Visual

end

