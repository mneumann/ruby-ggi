$:.unshift ".."
require "ggi"

def scan(x1, y1, x2, y2, ybuffer)
  dx = x2 - x1
  dy = y2 - y1 
  return if dy == 0.0
  gradient = dx/dy.to_f

  x = x1
  for y in y1.to_i .. y2.to_i
    ybuffer[y] = [] if ybuffer[y].nil?
    ybuffer[y] << (x+0.5).to_i
    x += gradient
  end
end 


# Floats:  x1, y1, x2, y2, gradient, ex, ey, ax, ay(?), bx, by(?)
# Integer: height, ay, by
def perfect_scan(x1, y1, x2, y2, ybuffer)
  dx = x2 - x1
  dy = y2 - y1
  return if dy == 0.0
  gradient = dx/dy

  ey = (y1+1.0).to_i - y1
  ex = gradient * ey 

  ax = x1 + ex
  ay = (y1+1.0).to_i

  by = y2.to_i

  # scanning
  x = ax
  for y in ay..by
    ybuffer[y] = [] if ybuffer[y].nil?
    ybuffer[y] << (x  + 0.5) . to_i
    x += gradient
  end

  ybuffer[y1] = [] if ybuffer[y1].nil?
  ybuffer[y1] << (x1  + 0.5) . to_i
end


Vertex = Struct.new("Vertex", :x, :y)

def poly(vis, vertices, edges, scan_method=:perfect_scan)
  ybuffer = []

  edges.each do |a, b|
    v1, v2 = 
    if vertices[a].y > vertices[b].y
      [vertices[b], vertices[a]]
    else
      [vertices[a], vertices[b]]
    end

    send(scan_method, v1.x, v1.y, v2.x, v2.y, ybuffer)
  end

  ybuffer.each_with_index do |arr, y|
    next if arr.nil?
    min_x, max_x = arr.min, arr.max
    vis.hline(min_x, y, max_x-min_x+1) 
  end
end

GGI::Visual.openSimple(w=800, h=600) do |vis| 
  loop {
    vis.fg = vis.rgb(rand, rand, rand) 
    #poly(vis, [ Vertex[rand*800, rand*600], Vertex[rand*800, rand*600], Vertex[rand*800, rand*600] ], [ [0,1], [1,2], [2,0] ], :scan)
    poly(vis, [ Vertex[rand*800, rand*600], Vertex[rand*800, rand*600], Vertex[rand*800, rand*600], Vertex[rand*800, rand*600] ], [ [0,1], [1,2], [2,3], [3,0] ], :scan)
    break if vis.kbhit
  }
end
