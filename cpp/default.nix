with (import <nixpkgs> {});
gcc9Stdenv.mkDerivation {
  name = "md-cpu";
  src = ./.;

  buildInputs = [
    imagemagick
    pkgconfig
    gitAndTools.hub gnumake
    meson ninja
    boost
    glib gnome3.glade gtkmm3
  ];

  installPhase = ''
	mkdir -p $out/bin
	cp md-cpu $out/bin
  '';
}
