#include "TestValidator.h"
#include "TestUtils.h"
#include "Validator.h"
#include "rapidassist/strings.h"

using namespace libargvcodec;
using namespace libargvcodec::validation;

#define DECLARE_FLAG_RULE(name, arg, mandatory)         \
  ValidationRule * name = NULL;                         \
  {                                                     \
    ValidationRule::FLAGS f;                            \
    if (mandatory)                                      \
      f = ValidationRule::FlagFactory::mandatoryFlag(); \
    else                                                \
      f = ValidationRule::FlagFactory::optionalFlag();  \
    name = new ValidationRule(arg, f);                  \
  }                                                     \
  rules.addRule(name);

#define DECLARE_MANDATORY_FLAG_RULE(name, arg)  DECLARE_FLAG_RULE(name, arg, true);
#define DECLARE_OPTIONAL_FLAG_RULE(name, arg)   DECLARE_FLAG_RULE(name, arg, false);

#define DECLARE_NEXTVALUE_RULE(name, arg, nextvalue)          \
  ValidationRule * name = NULL;                               \
  {                                                           \
    ValidationRule::FLAGS f;                                  \
    if (nextvalue)                                            \
      f = ValidationRule::FlagFactory::mandatoryNextValue();  \
    else                                                      \
      f = ValidationRule::FlagFactory::mandatoryFlag();       \
    name = new ValidationRule(arg, f);                        \
  }                                                           \
  rules.addRule(name);

////return true when a result list has an invalid result which is about iText
//bool hasFailedResultAbout(const char * iText, const ValidationRuleList & iRules)
//{
//  for(size_t i=0; i<iRules.getRules().size(); i++)
//  {
//    const ValidationRule * rule = iRules.getRules()[i];
//    const ValidationRule::RESULT & result = rule->getResult();
//    if (!result.validity)
//    {
//      size_t pos = rule->getResult().errorDescription.find(iText);
//      if (pos != std::string::npos)
//      {
//        return true;
//      }
//    }
//  }
//  return false;
//}

//bool hasValidResultAbout(const char * iText, const ValidationRuleList & iRules)
//{
//  for(size_t i=0; i<iRules.getRules().size(); i++)
//  {
//    const ValidationRule * rule = iRules.getRules()[i];
//    const ValidationRule::RESULT & result = rule->getResult();
//    if (result.validity == true)
//    {
//      size_t pos = rule->getName().find(iText);
//      if (pos != std::string::npos)
//      {
//        return true;
//      }
//    }
//  }
//  return false;
//}

bool findRule(const ValidationRuleList & iRules, const ValidationRule * iRule)
{
  for(size_t i=0; i<iRules.getRules().size(); i++)
  {
    const ValidationRule * rule = iRules.getRules()[i];
    if (rule == iRule)
    {
      return true;
    }
  }
  return false;
}

void TestValidator::SetUp()
{
}

void TestValidator::TearDown()
{
}

TEST_F(TestValidator, testValidatorWithOrphan)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/x"), mkstr("/y"), mkstr("/z"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;

  DECLARE_FLAG_RULE(ruleX, "/x", true);
  DECLARE_FLAG_RULE(ruleY, "/y", true);

  Validator v;
  v.setUnknownArgumentAsError(true);
  v.validate(m, rules);

  //expecting ruleX and ruleY found in result lists
  ASSERT_TRUE( findRule(rules, ruleX) );
  ASSERT_TRUE( findRule(rules, ruleY) );

  ASSERT_EQ(rules.getRules().size(), 2); //only the 2 given rules are found as results
  ASSERT_TRUE( rules.hasValidRuleAbout("/x") );
  ASSERT_TRUE( rules.hasValidRuleAbout("/y") );
  ASSERT_TRUE( rules.isAllRulesValid() ); //assert all rules are valid
}

TEST_F(TestValidator, testValidatorWithoutOrphan)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/x"), mkstr("/y"), mkstr("/z"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;

  DECLARE_FLAG_RULE(ruleX, "/x", true);
  DECLARE_FLAG_RULE(ruleY, "/y", true);

  Validator v;
  v.setUnknownArgumentAsError(false);
  v.validate(m, rules);

  //expecting ruleX and ruleY found in result lists
  ASSERT_TRUE( findRule(rules, ruleX) );
  ASSERT_TRUE( findRule(rules, ruleY) );

  ASSERT_EQ(rules.getRules().size(), 3);
  ASSERT_TRUE( rules.hasValidRuleAbout( "/x") );
  ASSERT_TRUE( rules.hasValidRuleAbout( "/y") );
  ASSERT_TRUE( rules.hasFailedRuleAbout("/z") ); //expect a failed rule about /z argument
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

  ASSERT_EQ(ruleX, rules.getRules()[0]);
  ASSERT_EQ(ruleY, rules.getRules()[1]);
  ASSERT_EQ(ruleA, rules.getRules()[2]);
  ASSERT_EQ(ruleZ, rules.getRules()[3]);
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

  ASSERT_EQ(ruleX, rules.getRules()[0]);
  ASSERT_EQ(ruleY, rules.getRules()[1]);
  ASSERT_EQ(ruleA, rules.getRules()[2]);
  ASSERT_EQ(ruleZ, rules.getRules()[3]);
}

TEST_F(TestValidator, testMissingMandatoryArgument)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/a"), mkstr("/b"), mkstr("/c"), mkstr("/x"), mkstr("/y"), mkstr("/z"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;
  
  DECLARE_MANDATORY_FLAG_RULE(ruleB, "/b");
  DECLARE_MANDATORY_FLAG_RULE(ruleX, "/x");
  DECLARE_MANDATORY_FLAG_RULE(ruleD, "/h");
  DECLARE_MANDATORY_FLAG_RULE(ruleZ, "/z");

  Validator v;
  v.setUnknownArgumentAsError(true);
  v.validate(m, rules);

  ASSERT_EQ(rules.getRules().size(), 4);
  ASSERT_TRUE( rules.hasValidRuleAbout( "/b") );
  ASSERT_TRUE( rules.hasValidRuleAbout( "/x") );
  ASSERT_TRUE( rules.hasValidRuleAbout( "/z") );
  ASSERT_TRUE( rules.hasFailedRuleAbout("/h") ); //expect a failed rule about missing /h argument
}

TEST_F(TestValidator, testMissingOptionalArgument)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/a"), mkstr("/b"), mkstr("/c"), mkstr("/x"), mkstr("/y"), mkstr("/z"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  ValidationRuleList rules;
  
  DECLARE_OPTIONAL_FLAG_RULE(ruleB, "/b");
  DECLARE_OPTIONAL_FLAG_RULE(ruleX, "/x");
  DECLARE_OPTIONAL_FLAG_RULE(ruleD, "/h");
  DECLARE_OPTIONAL_FLAG_RULE(ruleZ, "/z");

  Validator v;
  v.setUnknownArgumentAsError(true);
  v.validate(m, rules);

  ASSERT_EQ(rules.getRules().size(), 4);
  ASSERT_TRUE( rules.hasValidRuleAbout("/b") );
  ASSERT_TRUE( rules.hasValidRuleAbout("/x") );
  ASSERT_TRUE( rules.hasValidRuleAbout("/z") );
  ASSERT_TRUE( rules.hasValidRuleAbout("/h") ); //expect a valid rule even if argument /h is missing
}
