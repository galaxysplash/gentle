clear
rm -rf tests
cmake . -B build
make -C build
mkdir tests
cd tests
echo ""
echo ""
./../build/gentle proj test_project
cd test_project
./../../build/gentle mod hey
