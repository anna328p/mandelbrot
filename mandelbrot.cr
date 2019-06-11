require "complex"
require "stumpy_png"
require "crymagick"

# Quadratic map function
def qmap(z, c)
  z*z + c
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

  return Complex.new(x_coord, y_coord)
end

IMG_WIDTH = 300
IMG_HEIGHT = 200
THRESHOLD = 25

def frame_iter(x1 : Float64, y1 : Float64, x2 : Float64, y2 : Float64,
               center_x : Float64, center_y : Float64,
               frame : Int32,
               reduction : Float64)
  frame.times do
    x1 = center_x - (center_x - x1) * (1 - reduction)
    y1 = center_y - (center_y - y1) * (1 - reduction)
    x2 = center_x - (center_x - x2) * (1 - reduction)
    y2 = center_y - (center_y - y2) * (1 - reduction)
  end

  return {x1, y1, x2, y2}
end

def generate_image(frame : Int32)
  bounds = frame_iter(-2, 1, 0.75, -1, -1, 0.25, frame, 0.1)
  image = StumpyPNG::Canvas.new(IMG_WIDTH, IMG_HEIGHT)

  IMG_HEIGHT.times do |y|
    IMG_WIDTH.times do |x|
      threshold = THRESHOLD + frame * 5 / 2
      coords = coord_map(*bounds, IMG_WIDTH, IMG_HEIGHT, x, y)
      threshold.downto(frame * 2.5 / 2) do |i|
        if escapes?(coords, i)
          image[x, y] = StumpyPNG::RGBA.from_rgb_n(255, 255, 255, 8)
        else
          red = 255 - (255.0 / threshold) * i
          green = (255.0 / threshold) * i
          blue = (255.0 / threshold) * i / 2.0
          image[x, y] = StumpyPNG::RGBA.from_rgb_n(red, green, blue, 8)
          break
        end
      end
    end
  end

  StumpyPNG.write(image, "output_#{frame}.png")
end

(ARGV[0].to_i..ARGV[1].to_i).each do |i|
  puts "Starting generation of frame #{i}..."
  generate_image(i)
  puts "Generated frame #{i}"
end
# vim: ts=2:sw=2:et:smarttab:
