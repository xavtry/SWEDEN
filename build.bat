emcc main.cpp imgui/*.cpp -Iimgui -I. ^
  -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1 ^
  -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 ^
  -s EXPORTED_FUNCTIONS="['_main']" ^
  -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" ^
  -O3 --shell-file shell_minimal.html ^
  -o sverige.js

echo.
echo SVERIGE.EXE COMPILED! Open index.html
pause
