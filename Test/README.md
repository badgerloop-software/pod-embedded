# Testing


- Link to GTest Primer [Click here!](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)

## Creating a Test

- Find the directory for the component/subsystem you would like to test, if it does not exist, create one
- Create a file `Test_<Thing you would like to test>.cpp`
- Add your file as a source in CMakeLists.txt
- Write your tests. Follow the test template if needed

## Test Template

```c
/**
 * Author: 
*/

#include <gtest/gtest.h>

TEST(ExampleTest, VerifyWorkingGTest){
    EXPECT_NE(1,0);
    EXPECT_GE(1,1);
    EXPECT_GE(1,0);
    EXPECT_LE(1,1);
    EXPECT_LE(0,1);
    
    ASSERT_EQ(0,0);
}
```