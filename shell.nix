with (import <nixpkgs> {});
let
  env = bundlerEnv {
    name = "mandelbrot-bundler-env";
    inherit ruby;
    gemfile  = ./Gemfile;
    lockfile = ./Gemfile.lock;
    gemset   = ./gemset.nix;
  };
in stdenv.mkDerivation {
  name = "mandelbrot";
  buildInputs = [ env pkgs.crystal ];
}
