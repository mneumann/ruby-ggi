$:.unshift ".."
require "ggi"

def scan(x1, y1, x2, y2, left_ybuffer, right_ybuffer)
  dx = x2 - x1
  dy = y2 - y1 
  return if dy == 0.0
  gradient = dx/dy.to_f

  x = x1
  for y in y1.to_i .. y2.to_i
    v = (x+0.5).to_i
    if left_ybuffer[y] > v
      left_ybuffer[y] = v
    elsif right_ybuffer[y] < v
      right_ybuffer[y] = v
    end
    x += gradient
  end
end 

def poly(vis, vertices, edges)
  yarr = vertices.map {|v| v[1]}
  ymin, ymax = yarr.min.to_i, yarr.max.to_i
  #h = ymax-ymin+1

  left_ybuffer = Array.new(600, 1000_000_000)
  right_ybuffer = Array.new(600, 0) 

  edges.each do |a, b|
    if vertices[a][1] > vertices[b][1]
      v1, v2 = vertices[b], vertices[a]
    else
      v1, v2 = vertices[a], vertices[b]
    end

    scan(v1[0], v1[1], v2[0], v2[1], left_ybuffer, right_ybuffer)
  end

  for i in ymin..ymax
    min_x, max_x = left_ybuffer[i], right_ybuffer[i]
    w = [1, max_x-min_x+1].max
    vis.hline(min_x, i, w)
  end
end

GGI::Visual.openSimple(w=800, h=600) do |vis| 
  loop {
    vis.fg = vis.rgb(rand, rand, rand) 
    poly(vis, [ [rand*800, rand*600], [rand*800, rand*600], [rand*800, rand*600] ], [ [0,1], [1,2], [2,0] ])
    break if vis.kbhit
  }
end
