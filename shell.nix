with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "mandelbrot";
  buildInputs = [ pkgs.crystal pkgs.shards gitAndTools.hub cmake ];
}
