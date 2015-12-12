#include "gtest_emulator.h"
#include "Validator.h"
#include "utils.h"

#define DECLARE_FLAG_RULE(name, arg, mandatory) \
  VALIDATION_RULE name;                         \
  name.argumentName = arg;                      \
  name.flags.isMandatory = mandatory;           \
  name.flags.isFlag = true;                     \
  rules.push_back(&name);

#define DECLARE_MANDATORY_FLAG_RULE(name, arg)  \
  VALIDATION_RULE name;                         \
  name.argumentName = arg;                      \
  name.flags.isMandatory = true;                \
  name.flags.isFlag = true;                     \
  rules.push_back(&name);

#define DECLARE_OPTIONAL_FLAG_RULE(name, arg) \
  VALIDATION_RULE name;                       \
  name.argumentName = arg;                    \
  name.flags.isOptional = true;               \
  name.flags.isFlag = true;                   \
  rules.push_back(&name);

#define DECLARE_NEXTVALUE_RULE(name, arg, nextvalue)  \
  VALIDATION_RULE name;                               \
  name.argumentName = arg;                            \
  name.flags.isMandatory = true;                      \
  name.flags.isFlag = !nextvalue;                     \
  name.flags.hasNextValue = nextvalue;                \
  rules.push_back(&name);

//return true when a result list has an invalid result which is about iText
bool hasFailedResultAbout(const char * iText, const ValidationResultList & iResults)
{
  for(size_t i=0; i<iResults.size(); i++)
  {
    const VALIDATION_RESULT & result = iResults[i];
    if (result.isValid == false)
    {
      size_t pos = result.description.find(iText);
      if (pos != std::string::npos)
      {
        return true;
      }
    }
  }
  return false;
}

bool hasValidResultAbout(const char * iText, const ValidationResultList & iResults)
{
  for(size_t i=0; i<iResults.size(); i++)
  {
    const VALIDATION_RESULT & result = iResults[i];
    if (result.isValid == true && result.rule != NULL)
    {
      size_t pos = result.rule->argumentName.find(iText);
      if (pos != std::string::npos)
      {
        return true;
      }
    }
  }
  return false;
}

bool findRule(const ValidationResultList & iResults, const VALIDATION_RULE * iRule)
{
  for(size_t i=0; i<iResults.size(); i++)
  {
    const VALIDATION_RESULT & result = iResults[i];
    if (result.rule == iRule)
    {
      return true;
    }
  }
  return false;
}

TEST_F(TestValidator, testValidatorOrphan)
{
  char* argv[] = {"test.exe", "/x", "/y", "/z", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;
  VALIDATION_RULE ruleX;
  ruleX.argumentName = "/x";
  ruleX.flags.isMandatory = true;
  ruleX.flags.isFlag = true;
  rules.push_back(&ruleX);
  VALIDATION_RULE ruleY;
  ruleY.argumentName = "/y";
  ruleY.flags.isMandatory = true;
  ruleY.flags.isFlag = true;
  rules.push_back(&ruleY);
  
  Validator v;
  v.setOrphanArgumentsAccepted(true);
  ValidationResultList orphansAcceptedResults = v.validate(m, rules);
  v.setOrphanArgumentsAccepted(false);
  ValidationResultList noOrphansResults = v.validate(m, rules);

  //expecting ruleX and ruleY found in result lists
  ASSERT_TRUE( findRule(orphansAcceptedResults,   &ruleX) );
  ASSERT_TRUE( findRule(orphansAcceptedResults,   &ruleY) );
  ASSERT_TRUE( findRule(noOrphansResults,         &ruleX) );
  ASSERT_TRUE( findRule(noOrphansResults,         &ruleY) );

  ASSERT_EQ(orphansAcceptedResults.size(), 2); //only the 2 given rules are found as results
  ASSERT_TRUE( hasValidResultAbout( "/x", orphansAcceptedResults) );
  ASSERT_TRUE( hasValidResultAbout( "/y", orphansAcceptedResults) );
  ASSERT_TRUE( isValid(orphansAcceptedResults) ); //assert all rules are valid

  ASSERT_EQ(noOrphansResults.size(), 3);
  ASSERT_TRUE( hasValidResultAbout( "/x", noOrphansResults) );
  ASSERT_TRUE( hasValidResultAbout( "/y", noOrphansResults) );
  ASSERT_TRUE( hasFailedResultAbout("/z", noOrphansResults) ); //expect a failed rule about /z argument
}

TEST_F(TestValidator, testSortMandatoryFirst)
{
  ValidationRuleList rules;

  DECLARE_FLAG_RULE(ruleA, "/a", false);
  DECLARE_FLAG_RULE(ruleX, "/x", true);
  DECLARE_FLAG_RULE(ruleY, "/y", true);
  DECLARE_FLAG_RULE(ruleZ, "/z", false);

  Validator v;
  v.sortRules(rules);

  ASSERT_EQ(&ruleX, rules[0]);
  ASSERT_EQ(&ruleY, rules[1]);
  ASSERT_EQ(&ruleA, rules[2]);
  ASSERT_EQ(&ruleZ, rules[3]);
}

TEST_F(TestValidator, testSortNextValueFirst)
{
  ValidationRuleList rules;

  DECLARE_NEXTVALUE_RULE(ruleA, "/a", false);
  DECLARE_NEXTVALUE_RULE(ruleX, "/x", true);
  DECLARE_NEXTVALUE_RULE(ruleY, "/y", true);
  DECLARE_NEXTVALUE_RULE(ruleZ, "/z", false);

  Validator v;
  v.sortRules(rules);

  ASSERT_EQ(&ruleX, rules[0]);
  ASSERT_EQ(&ruleY, rules[1]);
  ASSERT_EQ(&ruleA, rules[2]);
  ASSERT_EQ(&ruleZ, rules[3]);
}

TEST_F(TestValidator, testMissingMandatoryArgument)
{
  char* argv[] = {"test.exe", "/a", "/b", "/c", "/x", "/y", "/z", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;
  
  DECLARE_MANDATORY_FLAG_RULE(ruleB, "/b");
  DECLARE_MANDATORY_FLAG_RULE(ruleX, "/x");
  DECLARE_MANDATORY_FLAG_RULE(ruleD, "/h");
  DECLARE_MANDATORY_FLAG_RULE(ruleZ, "/z");

  Validator v;
  v.setOrphanArgumentsAccepted(true);
  ValidationResultList results = v.validate(m, rules);

  ASSERT_EQ(results.size(), 4);
  ASSERT_TRUE( hasValidResultAbout( "/b", results) );
  ASSERT_TRUE( hasValidResultAbout( "/x", results) );
  ASSERT_TRUE( hasValidResultAbout( "/z", results) );
  ASSERT_TRUE( hasFailedResultAbout("/h", results) ); //expect a failed rule about missing /h argument
}

TEST_F(TestValidator, testMissingOptionalArgument)
{
  char* argv[] = {"test.exe", "/a", "/b", "/c", "/x", "/y", "/z", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;
  
  DECLARE_OPTIONAL_FLAG_RULE(ruleB, "/b");
  DECLARE_OPTIONAL_FLAG_RULE(ruleX, "/x");
  DECLARE_OPTIONAL_FLAG_RULE(ruleD, "/h");
  DECLARE_OPTIONAL_FLAG_RULE(ruleZ, "/z");

  Validator v;
  v.setOrphanArgumentsAccepted(true);
  ValidationResultList results = v.validate(m, rules);

  ASSERT_EQ(results.size(), 4);
  ASSERT_TRUE( hasValidResultAbout( "/b", results) );
  ASSERT_TRUE( hasValidResultAbout( "/x", results) );
  ASSERT_TRUE( hasValidResultAbout( "/z", results) );
  ASSERT_TRUE( hasValidResultAbout( "/h", results) ); //expect a valid rule even if argument /h is missing
}
