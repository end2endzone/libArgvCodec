#ifndef CMDPROMPTARGUMENTCODEC_H
#define CMDPROMPTARGUMENTCODEC_H

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT CmdPromptArgumentCodec : public virtual IArgumentEncoder,
    public virtual IArgumentDecoder
  {
  public:
    CmdPromptArgumentCodec();
    virtual ~CmdPromptArgumentCodec();

    //IArgumentEncoder
    virtual std::string encodeArgument(const char * iValue);
    virtual std::string encodeCommandLine(const ArgumentList & iArguments);

    //IArgumentDecoder
    virtual std::string decodeArgument(const char * iValue);
    virtual ArgumentList decodeCommandLine(const char * iValue);

  public:
    /// <summary>Returns true if the given character is an argument separator character.</summary>
    /// <param name="c">The given character to test.</param>
    /// <returns>Returns true if the given character is an argument separator character. Returns false otherwise.</returns>
    virtual bool isArgumentSeparator(const char c);

    /// <summary>Returns true if the given character is a shell character.</summary>
    /// <param name="c">The given character to test.</param>
    /// <returns>Returns true if the given character is a shell character. Returns false otherwise.</returns>
    virtual bool isShellCharacter(const char c);

    /// <summary>Returns true if the given string has at least one shell character.</summary>
    /// <param name="iValue">The given string to test. Must not be NULL.</param>
    /// <returns>Returns true if the given string has at least one shell character. Returns false otherwise.</returns>
    virtual bool hasShellCharacters(const char * iValue);

    /// <summary>Returns true if the current encoder/decoder supports shell characters.</summary>
    /// <returns>Returns true if the current encoder/decoder supports shell characters. Returns false otherwise.</returns>
    virtual bool supportsShellCharacters();

  protected:
    /// <summary>Parses a command line string into a list of arguments.</summary>
    /// <param name="iCmdLine">The command line string to parse.</param>
    /// <param name="oArguments">The output list of arguments.</param>
    /// <returns>Returns true on parse success. Returns false otherwise.</returns>
    bool parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments);

    /// <summary>Returns the character at offset iIndex of the given string. The function is safe as it returns \0 if iIndex is out-of-range.</summary>
    /// <param name="iValue">The given string.</param>
    /// <param name="iIndex">The character offset within the given iValue string.</param>
    /// <returns>Returns the character at offset iIndex of the given string. The function is safe as it returns \0 if iIndex is out-of-range.</returns>
    char getSafeCharacter(const char * iValue, size_t iIndex);

    /// <summary>Verify if a given string starts by the expected value.</summary>
    /// <param name="iValue">The value of the given string.</param>
    /// <param name="iSequenceExpr">The value of the expected string.</param>
    /// <returns>Returns true if the given string starts by the expected value. Returns false otherwise.</returns>
    bool matchesSequence(const char * iValue, const char * iSequenceExpr);

    /// <summary>Verify if a given string at offset iValueOffset, starts by the expected value.</summary>
    /// <param name="iValue">The value of the given string.</param>
    /// <param name="iValueOffset">The offset within iValue.</param>
    /// <param name="iSequenceExpr">The value of the expected string.</param>
    /// <returns>Returns true if the given string starts by the expected value. Returns false otherwise.</returns>
    bool matchesSequence(const char * iValue, size_t iValueOffset, const char * iSequenceExpr);

    bool matchesBackSlashDblQuoteSequence(const char * iValue, size_t iValueOffset, size_t & oNumBlackSlash, size_t & oSkipLength, bool iInString, bool iInCaretString);

    /// <summary>Returns true if the character at offset iOffset in iCmdLine indicate the beginning of a string argument.</summary>
    /// <param name="iCmdLine">The command line string.</param>
    /// <param name="iOffset">The offset in iCmdLine to test for the beginning of a string argument.</param>
    /// <returns>Returns true if the character at offset iOffset in iCmdLine indicate the beginning of a string argument. Returns false otherwise.</returns>
    bool isStringStart(const char * iCmdLine, size_t iOffset);

    /// <summary>Returns true if the character at offset iOffset in iCmdLine indicate the end of a string argument of length iSequenceLength.</summary>
    /// <param name="iCmdLine">The command line string.</param>
    /// <param name="iOffset">The offset in iCmdLine to test for the beginning of a string argument.</param>
    /// <param name="iSequenceLength">The expected length of the string argument.</param>
    /// <returns>Returns true if the character at offset iOffset in iCmdLine indicate the end of a string argument of length iSequenceLength.</returns>
    bool isStringEnd(const char * iCmdLine, size_t iOffset, size_t iSequenceLength);

  };

}; //namespace libargvcodec

#endif //CMDPROMPTARGUMENTCODEC_H
