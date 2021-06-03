cd c/leveldesigner
cmake . && make
cd ../..
cd c/solver
cmake . && make
cd ../..
cd app
npm install
cd ..
cp c/leveldesigner/leveldesigner.exe app/
cp c/solver/solver.exe app/