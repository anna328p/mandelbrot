with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "mandelbrot";
  buildInputs = [ env pkgs.crystal gitAndTools.hub];
}
