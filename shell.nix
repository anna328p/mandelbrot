with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "mandelbrot";
  buildInputs = [ pkgs.crystal gitAndTools.hub];
}
