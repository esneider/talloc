(1) Building the library:

    (Make sure you are at the root)
    mkdir build
    cd build
    cmake ..
    make

(2) Build the tests if necessary:

    (Build the library first as described in point 1)
    make test

(3) Installing the library:

    (Build the library first as described in point 1)
    make install

(4) Test the library if necessary

    (Build the tests first as described in point 2)
    cd tests
    valgrind ./talloc_test

(5) When you're done:

    You can get rid of the build/ folder at the root.
