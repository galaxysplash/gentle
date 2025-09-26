clear
rm -rf tests
cmake . -B build
make -C build
mkdir tests
cd tests
echo ""
echo ""
./../build/gentle proj test_project
cd proj
./../build/gentle mod hey
