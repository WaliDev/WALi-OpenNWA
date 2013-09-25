#include <cassert>
#include "gtest/gtest.h"

#include <wali/util/ConfigurationVar.hpp>

#ifdef _MSC_VER
  // These are actually untested.
  int
  setenv(char const * var, char const * val, int)
  {
    assert(var);
    assert(val);
    assert(strlen(val) > 0u);
    assert(strchr(var, '=') == NULL);

    std::string s = std::string(var) + "=" + val;
    return _putenv(s.c_str());
  }

  int
  unsetenv(char const * var)
  {
    assert(var);
    assert(strchr(var, '=') == NULL);

    std::string s = std::string(var) + "=";
    return _putenv(s.c_str());
  }
#endif
  
namespace wali
{
  namespace util
  {
    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         integerBasicAccessorTests)
    {
      ConfigurationVar<int> v("MyVar", 1);

      EXPECT_EQ(1, v.getDefault());
      EXPECT_EQ("MyVar", v.getEnvVarName());
    }

    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         stringBasicAccessorTests)
    {
      ConfigurationVar<std::string> v("MyVar", "MyDefault");

      EXPECT_EQ("MyDefault", v.getDefault());
      EXPECT_EQ("MyVar", v.getEnvVarName());
    }

    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         mappingAccessorTests)
    {
      ConfigurationVar<int> v("MyVar", 1);
      v("A", 0)("B", 1)("C", 2);

      // Should not modify the default or name
      EXPECT_EQ(1, v.getDefault());
      EXPECT_EQ("MyVar", v.getEnvVarName());

      EXPECT_EQ(0, v.getValueOf("A"));
      EXPECT_EQ(1, v.getValueOf("B"));
      EXPECT_EQ(2, v.getValueOf("C"));
    }

    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         invalidValueMappingAccessorTest)
    {
      ConfigurationVar<int> v("MyVar", 1);
      v("A", 0)("B", 1)("C", 2);

      EXPECT_EQ(1, v.getValueOf("D"));
    }

    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         withoutMappingActualUse)
    {
      int ret = unsetenv("MyVar");
      ASSERT_EQ(0, ret);

      int should_be_default = ConfigurationVar<int>("MyVar", 1);
      
      EXPECT_EQ(1, should_be_default);
    }

    TEST(wali$util$$ConfigurationVar$constructor$getDefault$getEnvVarName,
         actualUseTest)
    {
      int ret;

      ret = unsetenv("MyVar");
      ASSERT_EQ(0, ret);
      int var_was_undef = ConfigurationVar<int>("MyVar", 10)("A", 0)("B", 1)("C", 2);
      
      ret = setenv("MyVar", "A", 1);
      ASSERT_EQ(0, ret);
      int var_was_A = ConfigurationVar<int>("MyVar", 10)("A", 0)("B", 1)("C", 2);

      ret = setenv("MyVar", "B", 1);
      ASSERT_EQ(0, ret);
      int var_was_B = ConfigurationVar<int>("MyVar", 10)("A", 0)("B", 1)("C", 2);

      ret = setenv("MyVar", "C", 1);
      ASSERT_EQ(0, ret);
      int var_was_C = ConfigurationVar<int>("MyVar", 10)("A", 0)("B", 1)("C", 2);

      ret = setenv("MyVar", "D", 1);
      ASSERT_EQ(0, ret);
      int var_was_D = ConfigurationVar<int>("MyVar", 10)("A", 0)("B", 1)("C", 2);

      ret = unsetenv("MyVar");
      ASSERT_EQ(0, ret);
      
      EXPECT_EQ(10, var_was_undef);
      EXPECT_EQ(0, var_was_A);
      EXPECT_EQ(1, var_was_B);
      EXPECT_EQ(2, var_was_C);
      EXPECT_EQ(10, var_was_D);
    }

  }
}
