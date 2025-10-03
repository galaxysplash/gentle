clear
rm -rf tests
cmake . -B build
make -C build
mkdir tests
cd tests

test_gentle() {
  echo ""
  echo ""
  ./../build/gentle $1 $2
  echo ""
  echo ""
  cd $2
  ./../../build/gentle mod hey
  ./../../build/gentle sub my_sub
  ./../../build/gentle run
}

test_gentle proj test_project
test_gentle asm asm_proj

