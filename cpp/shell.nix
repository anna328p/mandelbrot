with (import <nixpkgs> {});
gcc9Stdenv.mkDerivation {
  name = "mandelbrot-cuda";
  buildInputs = [
    cudatoolkit
    imagemagick
    pkgconfig
    gitAndTools.hub cmake clang
  ];
}
