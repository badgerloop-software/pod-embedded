# Utils

A collection of randomly useful, fully self-contained programs for the purpose of
making other tasks easier. They are not tests, however they can be used for
testing.

### Setup

To start working on your own utility, run the following:

1. `git checkout master`
2. `git checkout -b util-${NAME_OF_YOUR_UTILITY}`
3. Add a file with an `int main();` in the utils folder
4. The rest is up to you

### Building

Either navigate to the base `pod/` directory, or if you are a power user, you
can run `cd ../../ && make utils && cd embedded/utils/` to build and return to
this directory. Up to you!

### Running

All build utils will be built into the `/pod/out/utils/` directory

