with (import <nixpkgs> {});
gcc9Stdenv.mkDerivation {
  name = "mandelbrot-cuda";
  buildInputs = [
    imagemagick
    pkgconfig
    gitAndTools.hub cmake  # clang
    glib gnome3.glade gtkmm3
  ];
}
