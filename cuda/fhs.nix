{ pkgs ? import <nixpkgs> {} }:

(pkgs.buildFHSUserEnv {
  name = "mandelbrot-cuda-fhs";
  targetPkgs = pkgs: (with pkgs;
    [ cudatoolkit imagemagick pkgconfig
      gitAndTools.hub cmake clang
      gcc9Stdenv.cc
      gcc9Stdenv.cc.cc.lib
    ]);
  profile = "export LC_ALL=C; unset LANGUAGE";
  runScript = "bash";
}).env
