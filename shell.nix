with import <nixpkgs> {}; {
    cppEnv = stdenvNoCC.mkDerivation {
        name = "cpp-environment";
        buildInputs = with pkgs; [
            ninja
            cmake
            cmake-format
            gcc11
            clang-tools
        ];
        shellHook = ''source ~/.bashrc'';
    };
}
