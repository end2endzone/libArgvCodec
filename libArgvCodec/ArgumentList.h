#pragma once

#include "libargvcodec.h"
#include "macros.h"
#include <string>
#include <vector>

namespace libargvcodec
{

  class LIBARGVCODEC_API ArgumentList
  {
  public:
    typedef std::vector<std::string> StringList;

    ArgumentList();
    ArgumentList(const ArgumentList & iArgumentManager);
    virtual ~ArgumentList();

    void init(int argc, char** argv);
    void init(const StringList & iArguments);

    char * getArgument(int iIndex) const;

    int getArgc() const;
    char** getArgv() const;

    //logical methods
    const ArgumentList & operator = (const ArgumentList & iArgumentManager);
    bool operator == (const ArgumentList & iArgumentManager) const;
    bool operator != (const ArgumentList & iArgumentManager) const;

    //manipulation methods
    bool insert(int iIndex, const char * iValue);
    bool insert(const char * iValue);
    bool remove(int iIndex);
    bool replace(int iIndex, const char * iValue);

    //parsing methods

    //
    // Description:
    //  Finds the exact iValue within all parameters.
    // Parameters:
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    // Return:
    //  Returns the index of the parameter if found. Returns -1 otherwise.
    //
    int findIndex(const char * iValue) const;
    int findIndex(const char * iValue, bool iCaseSensitive) const;

    //
    // Description:
    //  Tells if the exact iValue can be found within all parameters.
    // Parameters:
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    // Return:
    //  Returns the true if iValue can be found. Returns false otherwise.
    //
    bool contains(const char * iValue) const;
    bool contains(const char * iValue, bool iCaseSensitive) const;

    //
    // Description:
    //  Tells if the option iValue is specified as a parameter.
    // Parameters:
    //  iValue: The option we are looking for.
    //          May include any prefix like the following characters: "-name", "--name", "/name".
    //          If the option is not found, the function will also try finding a value using the list
    //          of known prefixes. See getOptionPrefixes() for more information.
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    //  oIndex: The index where the option was found. Set to -1 if option iValue if not found.
    // Return:
    //  Returns the true if iValue is found. Returns false otherwise.
    //
    bool findOption(const char * iValue) const;
    bool findOption(const char * iValue, int & oIndex) const;
    bool findOption(const char * iValue, bool iCaseSensitive, int & oIndex) const;
  
    //
    // Description:
    //  Tells if the argument named value iValue is specified as a parameter.
    //  Named values are specified with the following format: [prefix]name=value (where prefix is optional)
    // Parameters:
    //  iValueName: The option's name we are looking for.
    //              The prefix is optional. Can be as simple as "name=value"
    //              May include any prefix like the following characters: "-name=", "--name=", "/name=".
    //              If the option is not found, the function will also try finding a value using the list
    //              of known prefixes. See getOptionPrefixes() for more information.
    //              The ending equal sign in the value name is optional. "/name" is identical to "/name="
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    //  oIndex: The index where the option was found. Set to -1 if option if not found.
    //  oValue: The value of the option name. Set to empty string (or 0) if not found.
    // Return:
    //  Returns the true if the option's name is specified. Returns false otherwise.
    //
    bool findValue(const char * iValueName, int & oIndex, std::string & oValue) const;
    bool findValue(const char * iValueName, int & oIndex, int & oValue) const;
    bool findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;
    bool findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const;

    //
    // Description:
    //  Finds the value that follows the exact named argument.
    //  Named values are specified with the following format: [prefix]name value (where prefix is optional)
    //  Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    // Parameters:
    //  iValueName: The option's name are looking for.
    //              The prefix is optional. Can be as simple as "name"
    //              May include any prefix like the following characters: "-name", "--name", "/name".
    //              If the option is not found, the function will also try finding a value using the list
    //              of known prefixes. See getOptionPrefixes() for more information.
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    //  oIndex: The index where the option was found. Set to -1 if option if not found.
    //  oValue: The value of the option name. Set to empty string (or 0) if not found.
    // Return:
    //  Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.
    //
    bool findNextValue(const char * iValueName, int & oIndex, std::string & oValue) const;
    bool findNextValue(const char * iValueName, int & oIndex, int & oValue) const;
    bool findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;
    bool findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const;

    //
    // Description:
    //  Same as: bool findOption(char* iValue);
    //  Removes the option from the argument list if found.
    //
    bool extractOption(const char * iValue);
    bool extractOption(const char * iValue, bool iCaseSensitive);

    //
    // Description:
    //  Same as: bool findValue(char* iValueName, std::string & oValue);
    //  Removes the option from the argument list if found.
    //
    bool extractValue(const char * iValueName, std::string & oValue);
    bool extractValue(const char * iValueName, int & oValue);
    bool extractValue(const char * iValueName, bool iCaseSensitive, std::string & oValue);
    bool extractValue(const char * iValueName, bool iCaseSensitive, int & oValue);

    //
    // Description:
    //  Extracts a name=value pair from the list of parametes.
    //  Removes the value from the argument list if found.
    // Parameters:
    //  iValueName: The option's name we are looking for.
    //              The prefix is optional. Can be as simple as "name=value"
    //              May include any prefix like the following characters: "-name=", "--name=", "/name=".
    //              If the option is not found, the function will also try finding a value using the list
    //              of known prefixes. See getOptionPrefixes() for more information.
    //              The ending equal sign in the value name is optional. "/name" is identical to "/name="
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    // Returns:
    //  Returns the option's value if found. Returns an empty string otherwise
    //
    std::string extractValue(const char * iValueName);
    std::string extractValue(const char * iValueName, bool iCaseSensitive);

    //
    // Description:
    //  Same as: bool findNextValue(char* iValueName, std::string & oValue);
    //  Removes the option and its value from the argument list if found.
    //
    bool extractNextValue(const char * iValueName, std::string & oValue);
    bool extractNextValue(const char * iValueName, int & oValue);
    bool extractNextValue(const char * iValueName, bool iCaseSensitive, std::string & oValue);
    bool extractNextValue(const char * iValueName, bool iCaseSensitive, int & oValue);

    //
    // Description:
    //  Extracts the value that follows the exact named argument from the list of parametes.
    //  Named values are specified with the following format: [prefix]name value (where prefix is optional)
    //  Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    //  Removes the option and its value from the argument list if found.
    // Parameters:
    //  iValueName: The option's name we are looking for.
    //              The prefix is optional. Can be as simple as "name"
    //              May include any prefix like the following characters: "-name", "--name", "/name".
    //              If the option is not found, the function will also try finding a value using the list
    //              of known prefixes. See getOptionPrefixes() for more information.
    //  iCaseSensitive: Defines if the search is case sensitive or not. If not specified, the search is case sensitive.
    // Returns:
    //  Returns the option's value if found. Returns an empty string otherwise
    //
    std::string extractNextValue(const char * iValueName);
    std::string extractNextValue(const char * iValueName, bool iCaseSensitive);

    //
    // Description:
    //  Returns the list of configured prefixes
    // Returns:
    //  Returns the list of configured prefixes
    //
    const StringList & getOptionPrefixes();

    //
    // Description:
    //  Add the given prefix to the list of configured prefixes.
    // Parameters:
    //  iValue: A new argument option prefix. ie "-", "--" or "/"
    // Returns:
    //  Returns true if the prefix was added to the list. Returns false otherwise.
    //
    bool addOptionPrefix(const char * iValue);

    //
    // Description:
    //  Removes the given prefix from the list of configured prefixes.
    // Parameters:
    //  iValue: The argument option prefix to remove. ie "-", "--" or "/"
    // Returns:
    //  Returns true if the prefix was removed from the list. Returns false otherwise.
    //
    bool removeOptionPrefix(const char * iValue);

    //
    // Description:
    //  Clears/Empty the list of configured prefixes.
    //
    void clearOptionPrefixes();

  private:

    void rebuildArgv();
    char** mArgv;
    bool isValid(int iIndex) const;
    static std::string equalize(const std::string & iValue);
    static std::string equalize(const char * iValue);
    static void equalize(std::string & iValue);
    std::string rebuildArgumentPrefix(const char * iValueName, const std::string & iNewPrefix) const;

    //find methods that searches for the exact iValue, iValueName disregarding the prefixes
    bool findOption2(const char * iValue, bool iCaseSensitive, int & oIndex) const;
    bool findValue2(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;
    bool findNextValue2(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;

    SAFE_WARNING_DISABLE(4251); //warning C4251: 'ArgumentList::mArguments' : // warning C4251: 'foo' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'bar'
    StringList mArguments;
    StringList mPrefixes;
    SAFE_WARNING_RESTORE();

  };

}; //namespace libargvcodec
