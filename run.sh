TEST_FOLDER = tests
PROGRAM_NAME = gentle

rm -rf $TEST_FOLDER
cmake . -B build
make -C build
mkdir $TEST_FOLDER
cd $TEST_FOLDER
./../build/$PROGRAM_NAME test_project
