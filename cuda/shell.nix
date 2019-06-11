with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "mandelbrot-cuda";
  buildInputs = [
    cudatoolkit
    imagemagick
    pkgconfig
    gitAndTools.hub cmake
  ];
}
