#include "lorTests.h"
#include "lorPassword.h"

TEST(PasswordTests, BasicHashAndVerifyTest)
{
  const char password[] = "Tr0mbon3$!";
  const char *pPasswordHash = lorPassword_Hash(password);

  //No rehash checks
  EXPECT_TRUE(lorPassword_Verify(password, pPasswordHash));
  EXPECT_FALSE(lorPassword_Verify("password", pPasswordHash));

  //With rehash checks
  bool requiresRehash;
  EXPECT_TRUE(lorPassword_Verify(password, pPasswordHash, &requiresRehash));
  EXPECT_FALSE(requiresRehash);

  EXPECT_FALSE(lorPassword_Verify("password", pPasswordHash, &requiresRehash));
  EXPECT_FALSE(requiresRehash);

  lorFree(pPasswordHash);
}

TEST(PasswordTests, LegacyComplexityVerifyTest)
{
  //This test ensures that 'legacy' password hashes still work
  //This hash was a 16 byte salt, a 16 byte password hash using algorithm 1 and the complexity is only 1

  const char password[] = "your password is too short";
  const char passwordHash[] = "$1$1$EA9602EE52A6AE97ACDC7F9A42EE118C$0114745948669431F82DCFDBA696306A$";
  bool requiredRehash;

  //No Checking Rehash
  EXPECT_TRUE(lorPassword_Verify(password, passwordHash));

  //Check Rehash on success
  EXPECT_TRUE(lorPassword_Verify(password, passwordHash, &requiredRehash));
  EXPECT_TRUE(requiredRehash); //Algorithm is out of date and was valid so should rehash

  //Fail & Check Rehash
  EXPECT_FALSE(lorPassword_Verify("password", passwordHash, &requiredRehash));
  EXPECT_FALSE(requiredRehash);
}

TEST(PasswordTests, UTF8HashAndVerifyTest)
{
  const char password[] = "警察车图给了";
  const char *pPasswordHash = lorPassword_Hash(password);

  EXPECT_TRUE(lorPassword_Verify(password, pPasswordHash));
  EXPECT_FALSE(lorPassword_Verify("password", pPasswordHash));

  lorFree(pPasswordHash);
}
