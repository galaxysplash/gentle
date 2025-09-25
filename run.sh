rm -rf tests
cmake . -B build
make -C build
mkdir tests
cd tests
./../build/gentle test_project
