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

def iterations(coords, lower_bound, threshold)
  lower_bound.to_i.upto(threshold) do |i|
    if escapes?(coords, i)
      return i
    end
  end
  return -1
end

def iterations_flat(coords, threshold)
  z : Complex = Complex.new(0, 0)

  threshold.times do |i|
    z = z*z + coords
    return i if z.abs > 2.0
  end

  return -1
end

def coord_map(x1, y1, x2, y2, x_size, y_size, x, y)
  x_dim = x2 - x1
  y_dim = y2 - y1

  x_coord = x1 + (x.to_f / x_size) * x_dim
  y_coord = y1 + (y.to_f / y_size) * y_dim

  return Complex.new(x_coord, y_coord)
end

IMG_WIDTH = 600
IMG_HEIGHT = 400
THRESHOLD = 35

BEGIN = ARGV[0].to_i
END = ARGV[1].to_i

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
      threshold = (THRESHOLD + 5 * (1.05 ** frame)).floor.to_i
      coords = coord_map(*bounds, IMG_WIDTH, IMG_HEIGHT, x, y)
      if (i = iterations_flat(coords, threshold)) == -1
        image[x, y] = StumpyPNG::RGBA.from_rgb_n(0, 0, 0, 8)
      else
        red = (255.0 / threshold) * i / 2.0
        green = (255.0 / threshold) * i
        blue = 255 - (255.0 / threshold) * i
        image[x, y] = StumpyPNG::RGBA.from_rgb_n(red, green, blue, 8)
      end
    end
  end
  return image
end

filenames = [] of String
(BEGIN..END).each do |i|
  puts "Starting generation of frame #{i}..."
  image = generate_image(i)
  filename = "output_#{i}.png"
  StumpyPNG.write(image, filename)
  filenames << filename
  puts "Generated frame #{i}"
end

puts "Splicing..."
CryMagick::Tool::Convert.build do |c|
  c.loop 0
  c.delay 10
  filenames.each do |f|
    c << f
  end
  c.resize "300x200"
  c << "output_#{ARGV[2]}.gif"
end

puts "Job #{ARGV[2]} complete"
# vim: ts=2:sw=2:et:smarttab:
