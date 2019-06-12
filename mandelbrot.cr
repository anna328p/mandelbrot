require "complex"
require "stumpy_png"
require "crymagick"

IMG_WIDTH = 640
IMG_HEIGHT = 480
PIXELS = IMG_HEIGHT * IMG_WIDTH

THRESHOLD = 25
THRESHOLD_INCREASE = 1.025
#THRESHOLD_INCREASE = 1


BEGIN = ARGV[0].to_i
END = ARGV[1].to_i

ANGLE = Random.rand(360)
R = (1 - Math.cos(ANGLE)) / 2
X = R * Math.cos(ANGLE) + 0.25
Y = R * Math.sin(ANGLE)

def iterations(coords, threshold)
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

def frame_bounds(x1 : Float64, y1 : Float64, x2 : Float64, y2 : Float64,
               center_x : Float64, center_y : Float64,
               frame : Int32,
               reduction : Float64)

  width  = (x2 - x1) * (1 - reduction) ** frame
  height = (y2 - y1) * (1 - reduction) ** frame

  x1 = center_x - (width / 2)
  x2 = center_x + (width / 2)
  y1 = center_y - (height / 2)
  y2 = center_y + (height / 2)

  return {x1, y1, x2, y2}
end

def generate_image(frame : Int32)
  bounds = frame_bounds(-2, 1.1, 1, -1.1, X, Y, frame, 0.1)
  image = StumpyPNG::Canvas.new(IMG_WIDTH, IMG_HEIGHT)

  threshold = (THRESHOLD * THRESHOLD_INCREASE ** frame).ceil.to_i

  IMG_HEIGHT.times do |y|
    IMG_WIDTH.times do |x|
      coords = coord_map(*bounds, IMG_WIDTH, IMG_HEIGHT, x, y)
      if (i = iterations(coords, threshold)) == -1
        image[x, y] = StumpyPNG::RGBA.from_rgb_n(0, 0, 0, 8)
      else
        red = (255.0 / threshold) * i
        green = (255.0 / threshold) * i / 2.0
        blue = 255 - (255.0 / threshold) * i
        image[x, y] = StumpyPNG::RGBA.from_rgb_n(red, green, blue, 8)
      end
    end
  end
  return image
end

filenames = [] of String

system("mkdir -p frames")

(BEGIN..END).each do |i|
  puts "Starting generation of frame #{i}..."
  image = generate_image(i)
  filename = "frames/output_#{i}.png"
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
  c.resize "#{IMG_WIDTH / 2}x#{IMG_HEIGHT / 2}"
  c << "output_#{ARGV[2]}.gif"
end

puts "Job #{ARGV[2]} complete"
# vim: ts=2:sw=2:et:smarttab:
