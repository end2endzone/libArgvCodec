/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#ifndef ARGUMENTLIST_H
#define ARGUMENTLIST_H

#include "libargvcodec/config.h"
#include <string>
#include <vector>

//Private macros
#ifdef _MSC_VER
#   define SAFE_WARNING_DISABLE(value) __pragma( warning(push) ) __pragma( warning(disable: value))
#   define SAFE_WARNING_RESTORE() __pragma( warning(pop) )
#else
#   define SAFE_WARNING_DISABLE(value) ;
#   define SAFE_WARNING_RESTORE() ;
#endif

namespace libargvcodec
{

  /// <summary>ArgumentList class encapsulate a list of command line arguments</summary>
  class LIBARGVCODEC_EXPORT ArgumentList
  {
  public:
    typedef std::vector<std::string> StringList;

    /// <summary>Constructor</summary>
    ArgumentList();
    /// <summary>Copy constructor</summary>
    ArgumentList(const ArgumentList & iArgumentManager);
    /// <summary>Destructor</summary>
    virtual ~ArgumentList();

    /// <summary>Initialize the list with new argument using an argument array.</summary>
    /// <param name="argc">The number of arguments in the given array.</param>
    /// <param name="argv">The pointer to the first array element.</param>
    void init(int argc, char** argv);

    /// <summary>Initialize the list with new argument using an argument list.</summary>
    /// <param name="iArguments">The given list of arguments.</param>
    void init(const StringList & iArguments);

    /// <summary>Get the argument value of the iIndex element of the list.</summary>
    /// <param name="iIndex">The index of the argument within the list. See getArgc() to know how many arguments are stored in the list.</param>
    /// <returns>Returns the value of the requested argument. Returns an empty string if iIndex is out of bounds.</returns>
    const char * getArgument(int iIndex) const;

    /// <summary>Get the number of arguments in the list.</summary>
    /// <returns>Returns the number of arguments in the list.</returns>
    int getArgc() const;

    /// <summary>Get a pointer to the first element of the arguments array.</summary>
    /// <returns>Returns a pointer to the first element of the arguments array.</returns>
    char** getArgv() const;

    //logical methods
    /// <summary>Copy the given ArgumentList to the current instance.</summary>
    /// <returns>Returns the current ArgumentList instance.</returns>
    const ArgumentList & operator = (const ArgumentList & iArgumentManager);

    /// <summary>Compares the given ArgumentList to the current instance.</summary>
    /// <returns>Returns true if all elements of the given ArgumentList are equals to the elements of the current instance. Returns false otherwise.</returns>
    bool operator == (const ArgumentList & iArgumentManager) const;

    /// <summary>Compares the given ArgumentList to the current instance.</summary>
    /// <returns>Returns true if at least one element of the given ArgumentList is different to the elements of the current instance. Returns false otherwise.</returns>
    bool operator != (const ArgumentList & iArgumentManager) const;

    //manipulation methods
    /// <summary>Insert a new argument in the current list at position iIndex.</summary>
    /// <param name="iIndex">The insert position of the new argument within the list. Must not be greater than the current number of arguments.</param>
    /// <param name="iValue">The value of the new argument. Must not be NULL.</param>
    /// <returns>Returns true if the given argument was inserted at the requested position. Returns false otherwise.</returns>
    bool insert(int iIndex, const char * iValue);

    /// <summary>Insert a new argument at the end of the current list.</summary>
    /// <param name="iValue">The value of the new argument. Must not be NULL.</param>
    /// <returns>Returns true if the given argument was inserted. Returns false otherwise.</returns>
    bool insert(const char * iValue);
    
    /// <summary>Removes the argument of the list located at position iIndex.</summary>
    /// <param name="iIndex">The position of the argument to be deleted.</param>
    /// <returns>Returns true if the given argument was removed. Returns false otherwise.</returns>
    bool remove(int iIndex);
    
    /// <summary>Replaces the value of an existing argument with a new value.</summary>
    /// <param name="iIndex">The position of the argument which value must be changed. Must not be greater than the current number of arguments.</param>
    /// <param name="iValue">The new value of the argument. Must not be NULL.</param>
    /// <returns>Returns true if the given argument was modified. Returns false otherwise.</returns>
    bool replace(int iIndex, const char * iValue);

    //parsing methods

    /// <summary>Finds the exact iValue within all parameters.</summary>
    /// <returns>Returns the index of the parameter if found. Returns -1 otherwise.</returns>
    int findIndex(const char * iValue) const;

    /// <summary>Finds the exact iValue within all parameters.</summary>
    /// <param name="iValue">The option we are looking for.</param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <returns>Returns the index of the parameter if found. Returns -1 otherwise.</returns>
    int findIndex(const char * iValue, bool iCaseSensitive) const;

    /// <summary>Tells if the exact iValue can be found within all parameters.</summary>
    /// <returns>Returns the true if iValue can be found. Returns false otherwise.</returns>
    bool contains(const char * iValue) const;

    /// <summary>Tells if the exact iValue can be found within all parameters.</summary>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="iValue">The option we are looking for.</param>
    /// <returns>Returns the true if iValue can be found. Returns false otherwise.</returns>
    bool contains(const char * iValue, bool iCaseSensitive) const;

    /// <summary>Tells if the option iValue is specified as a parameter.</summary>
    /// <param name="iValue">
    ///   The option we are looking for.
    ///   May include any prefix like the following characters: "-name", "--name", "/name".
    ///   If the option is not found, the function will also try finding a value using the list
    ///   of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <returns>Returns the true if iValue is found. Returns false otherwise.</returns>
    bool findOption(const char * iValue) const;

    /// <summary>Tells if the option iValue is specified as a parameter.</summary>
    /// <param name="iValue">
    ///   The option we are looking for.
    ///   May include any prefix like the following characters: "-name", "--name", "/name".
    ///   If the option is not found, the function will also try finding a value using the list
    ///   of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option iValue if not found.</param>
    /// <returns>Returns the true if iValue is found. Returns false otherwise.</returns>
    bool findOption(const char * iValue, int & oIndex) const;

    /// <summary>Tells if the option iValue is specified as a parameter.</summary>
    /// <param name="iValue">
    ///   The option we are looking for.
    ///   May include any prefix like the following characters: "-name", "--name", "/name".
    ///   If the option is not found, the function will also try finding a value using the list
    ///   of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option iValue if not found.</param>
    /// <returns>Returns the true if iValue is found. Returns false otherwise.</returns>
    bool findOption(const char * iValue, bool iCaseSensitive, int & oIndex) const;
  
    /// <summary>
    /// Tells if the argument named value iValue is specified as a parameter.
    /// Named values are specified with the following format: [prefix]name=value (where prefix is optional)
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool findValue(const char * iValueName, int & oIndex, std::string & oValue) const;
  
    /// <summary>
    /// Tells if the argument named value iValue is specified as a parameter.
    /// Named values are specified with the following format: [prefix]name=value (where prefix is optional)
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool findValue(const char * iValueName, int & oIndex, int & oValue) const;
  
    /// <summary>
    /// Tells if the argument named value iValue is specified as a parameter.
    /// Named values are specified with the following format: [prefix]name=value (where prefix is optional)
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;
  
    /// <summary>
    /// Tells if the argument named value iValue is specified as a parameter.
    /// Named values are specified with the following format: [prefix]name=value (where prefix is optional)
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const;

    /// <summary>
    /// Finds the value that follows the exact named argument.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    ///</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool findNextValue(const char * iValueName, int & oIndex, std::string & oValue) const;

    /// <summary>
    /// Finds the value that follows the exact named argument.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    ///</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool findNextValue(const char * iValueName, int & oIndex, int & oValue) const;

    /// <summary>
    /// Finds the value that follows the exact named argument.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    ///</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const;

    /// <summary>
    /// Finds the value that follows the exact named argument.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    ///</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oIndex">The index where the option was found. Set to -1 if option if not found.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const;

    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findOption().</summary>
    /// <param name="iValue">
    ///   The option we are looking for.
    ///   May include any prefix like the following characters: "-name", "--name", "/name".
    ///   If the option is not found, the function will also try finding a value using the list
    ///   of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <returns>Returns the true if iValue is found. Returns false otherwise.</returns>
    bool extractOption(const char * iValue);

    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findOption().</summary>
    /// <param name="iValue">
    ///   The option we are looking for.
    ///   May include any prefix like the following characters: "-name", "--name", "/name".
    ///   If the option is not found, the function will also try finding a value using the list
    ///   of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <returns>Returns the true if iValue is found. Returns false otherwise.</returns>
    bool extractOption(const char * iValue, bool iCaseSensitive);

    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findValue().</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool extractValue(const char * iValueName, std::string & oValue);
    
    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findValue().</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool extractValue(const char * iValueName, int & oValue);
    
    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findValue().</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool extractValue(const char * iValueName, bool iCaseSensitive, std::string & oValue);
    
    /// <summary>Finds the given option. Removes the option from the argument list if found. See also findValue().</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified. Returns false otherwise.</returns>
    bool extractValue(const char * iValueName, bool iCaseSensitive, int & oValue);

    /// <summary>Extracts a name=value pair from the list of parametes. Removes the value from the argument list if found.</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <returns>Returns the option's value if found. Returns an empty string otherwise.</returns>
    std::string extractValue(const char * iValueName);

    /// <summary>Extracts a name=value pair from the list of parametes. Removes the value from the argument list if found.</summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name=value"
    /// May include any prefix like the following characters: "-name=", "--name=", "/name=".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// The ending equal sign in the value name is optional. "/name" is identical to "/name="
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <returns>Returns the option's value if found. Returns an empty string otherwise.</returns>
    std::string extractValue(const char * iValueName, bool iCaseSensitive);

    /// <summary>Finds the next option. Removes the option and its value from the argument list if found. See also findNextValue().</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool extractNextValue(const char * iValueName, std::string & oValue);
    
    /// <summary>Finds the next option. Removes the option and its value from the argument list if found. See also findNextValue().</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool extractNextValue(const char * iValueName, int & oValue);
    
    /// <summary>Finds the next option. Removes the option and its value from the argument list if found. See also findNextValue().</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool extractNextValue(const char * iValueName, bool iCaseSensitive, std::string & oValue);
    
    /// <summary>Finds the next option. Removes the option and its value from the argument list if found. See also findNextValue().</summary>
    /// <param name="iValueName">
    /// The option's name are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <param name="oValue">The value of the option name. Set to empty string (or 0) if not found.</param>
    /// <returns>Returns the true if the option's name is specified followed by a valid value. Returns false otherwise.</returns>
    bool extractNextValue(const char * iValueName, bool iCaseSensitive, int & oValue);

    /// <summary>
    /// Extracts the value that follows the exact named argument from the list of parametes.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    /// Removes the option and its value from the argument list if found.
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <returns>Returns the option's value if found. Returns an empty string otherwise.</returns>
    std::string extractNextValue(const char * iValueName);

    /// <summary>
    /// Extracts the value that follows the exact named argument from the list of parametes.
    /// Named values are specified with the following format: [prefix]name value (where prefix is optional)
    /// Note that argument names and values are not separated by an equal sign but specified as 2 single parameters.
    /// Removes the option and its value from the argument list if found.
    /// </summary>
    /// <param name="iValueName">
    /// The option's name we are looking for.
    /// The prefix is optional. Can be as simple as "name"
    /// May include any prefix like the following characters: "-name", "--name", "/name".
    /// If the option is not found, the function will also try finding a value using the list
    /// of known prefixes. See getOptionPrefixes() for more information.
    /// </param>
    /// <param name="iCaseSensitive">Defines if the search is case sensitive or not. If not specified, the search is case sensitive.</param>
    /// <returns>Returns the option's value if found. Returns an empty string otherwise.</returns>
    std::string extractNextValue(const char * iValueName, bool iCaseSensitive);

    /// <summary>Returns the list of configured prefixes.</summary>
    /// <returns>Returns the list of configured prefixes.</returns>
    const StringList & getOptionPrefixes();

    /// <summary>Add the given prefix to the list of configured prefixes.</summary>
    /// <param name="iValue">A new argument option prefix. ie "-", "--" or "/"</param>
    /// <returns>Returns true if the prefix was added to the list. Returns false otherwise.</returns>
    bool addOptionPrefix(const char * iValue);

    /// <summary>Removes the given prefix from the list of configured prefixes.</summary>
    /// <param name="iValue">The argument option prefix to remove. ie "-", "--" or "/"</param>
    /// <returns>Returns true if the prefix was removed from the list. Returns false otherwise.</returns>
    bool removeOptionPrefix(const char * iValue);

    /// <summary>Clears/Empty the list of configured prefixes.</summary>
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

#endif //ARGUMENTLIST_H
