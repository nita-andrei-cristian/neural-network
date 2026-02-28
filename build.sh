mkdir build
cd build
clear
cmake -DCMAKE_BUILD_TYPE=Release -DGGML_CUDA=ON -G Ninja ..

cd ..
cmake --build build --config Release -j

cd build

# This helps clang detect the file
mv compile_commands.json ../compile_commands.json

./neuralnetwork
