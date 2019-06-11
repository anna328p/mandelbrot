require 'bundler'
Bundler.setup

require 'chunky_png'

# Quadratic map function
def qmap(z, c)
  z**2 + c
end

def recurse_qmap(z, c, count)
  if count.zero?
    qmap(z, c)
  else
    recurse_qmap(qmap(z, c), c, count - 1)
  end
end

def escapes?(c, threshold)
  threshold.times do |i|
    return true if recurse_qmap(0, c, i).abs > 2
  end
  false
end

def coord_map(x1, y1, x2, y2, x_size, y_size, x, y)
  x_dim = x2 - x1
  y_dim = y2 - y1

  x_coord = x1 + (x.to_f / x_size) * x_dim
  y_coord = y1 + (y.to_f / y_size) * y_dim

  return Complex(x_coord, y_coord)
end

IMG_WIDTH = 300
IMG_HEIGHT = 200
THRESHOLD = 25

image = ChunkyPNG::Image.new(IMG_WIDTH, IMG_HEIGHT, ChunkyPNG::Color::TRANSPARENT)

IMG_HEIGHT.times do |y|
  IMG_WIDTH.times do |x|
    coords = coord_map(-2, 1, 1, -1, IMG_WIDTH, IMG_HEIGHT, x, y)
    THRESHOLD.downto(0) do |i|
      if escapes?(coords, i)
        image[x, y] = ChunkyPNG::Color.rgba(255, 255, 255, 255)
      else
        image[x, y] = ChunkyPNG::Color.rgba(i*10, 255-i*10, i*10, 255)
        break
      end
    end
  end
end

image.save('output.png', interlace: false)

# vim: ts=2:sw=2:et:smarttab:
