//--------------------------------------------------------------------
// nomjson.cpp
// A C++ class for reading JSON (JavaScript Object Notation) files.
//
// (C) Copyright 2016-2017 by Ammon R. Campbell
//
// I wrote this code for use in my own educational and experimental
// programs, but you may also freely use it in yours as long as you
// abide by the following terms and conditions:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above
//     copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials
//     provided with the distribution.
//   * The name(s) of the author(s) and contributors (if any) may not
//     be used to endorse or promote products derived from this
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.  IN OTHER WORDS, USE AT YOUR OWN RISK, NOT OURS.  
//--------------------------------------------------------------------

#include "nomjson.h"
//#define TRACE
#include "trace.h"

namespace njson
{

//--------------------------------------------------------------------
// Class to handle tokenizing of a string of JSON text.
//--------------------------------------------------------------------
class JsonScanner
{
public:
   JsonScanner() = default;
   JsonScanner(const JsonScanner &j) = delete;
   ~JsonScanner() = default;

   //--------------------------------------------------------------------
   // Begins tokenizing the given JSON text.  The first token will be
   // available via CurToken() immediately after Start() is called.
   // Errors throw.
   //--------------------------------------------------------------------
   void Start(const char *indata, size_t incount)
   {
      trace("JsonScanner constructing indata=%p incount=%zu\n", indata, incount);

      m_indata.clear();
      m_indata.insert(m_indata.end(), indata, indata + incount);
      m_inpos = 0;
      m_token.clear();
      ScanNextToken();
   }

   //--------------------------------------------------------------------
   // Stops tokenizing.  Any allocated memory is freed.
   //--------------------------------------------------------------------
   void Stop() { m_indata.clear(); m_inpos = 0; m_token.clear(); }

   //--------------------------------------------------------------------
   // Given the character that follows a backslash in a JSON file,
   // returns the corresponding unescaped character code.
   //--------------------------------------------------------------------
   char UnescapeSubstitute(char c)
   {
      switch(c)
      {
         case 'b':      return '\b';
         case 'f':      return '\f';
         case 'n':      return '\n';
         case 'r':      return '\r';
         case 't':      return '\t';
      }
      return c;
   }

   //--------------------------------------------------------------------
   // Scans the next token from the JSON text.  Returns false if there
   // are no more tokens.  The content of the token can be retrieved
   // via the CurToken() member below.
   //
   // Note that the enclosing quotes are removed from quoted tokens.
   //--------------------------------------------------------------------
   bool ScanNextToken()
   {
      m_token.clear();
      m_quote = ' ';
      size_t startingPos = m_inpos;

      // Skip leading whitespace.
      while (iswspace(CurChar()))
         ++m_inpos;

      const std::string singleCharTokens("{}[](),:");
      if (singleCharTokens.find(CurChar()) != std::string::npos)
      {
         // This token is one of the accepted single-character tokens.
         m_token += m_indata[m_inpos++];
      }
      else if (CurChar() == '"' || CurChar() == '\'')
      {
         // This token is surrounded by quotes.
         m_quote = m_indata[m_inpos++];   // Skip leading quote.
         while (CurChar() && CurChar() != m_quote)
         {
            if (CurChar() == '\\')
            {
               // Handle special backslash-escaped characters.
               m_inpos++; // Each the backslash.
               if (CurChar())
                  m_token += UnescapeSubstitute(m_indata[m_inpos++]);
            }
            else
            {
               m_token += m_indata[m_inpos++];  // Normal, non-escaped character.
            }
         }
         if (CurChar() == m_quote)
            m_inpos++;  // Skip trailing quote.
      }
      else
      {
         // This is a normal non-quoted token.
         // Copy characters up to the next whitespace or delimiter.
         while (CurChar() &&
                (!iswspace(CurChar())) &&
                singleCharTokens.find(CurChar()) == std::string::npos)
         {
            m_token += m_indata[m_inpos++];
         }
      }

      // Skip trailing whitespace.
      while (iswspace(CurChar()))
         ++m_inpos;

      trace(L"  m_token '%s'\n", m_token.c_str());

      // If at least one character of the input was processed then
      // the token is valid.  It is possible for the token to be
      // empty and still valid.  For example, when two quotes in
      // the JSON text have nothing inside "" then the corresponding
      // token is an empty string.
      return (m_inpos != startingPos);
   }

   //--------------------------------------------------------------------
   // Retrieves a reference to the current token.
   //--------------------------------------------------------------------
   const std::wstring & CurToken() const { return m_token; }

   //--------------------------------------------------------------------
   // Indicates whether the current token was quoted.  The return value
   // will be one of the following:  space ' ' to indicate a regular
   // unquoted token, or a single quote '\'', or a double quote '"' to
   // indicate what kind of quotes the token was quoted with.
   //--------------------------------------------------------------------
   char CurQuoted() { return m_quote; }

   //--------------------------------------------------------------------
   // Returns true if the given string matches the token.
   // The comparison is case-insensitive.
   //--------------------------------------------------------------------
   bool TokenIs(const wchar_t *text) { return (_wcsicmp(text, m_token.c_str()) == 0); }
   bool TokenIs(const std::wstring &text) { return TokenIs(text.c_str()); }

   //--------------------------------------------------------------------
   // Returns true if all of the JSON input has been tokenized (no more
   // tokens left).
   //--------------------------------------------------------------------
   bool EndOfInput() { return (m_inpos >= m_indata.size()); }

private:
   //--------------------------------------------------------------------
   // Return current character from the JSON input text, or '\0' if
   // we're already at the end of the input text.
   //--------------------------------------------------------------------
   char CurChar()
   {
      if (m_inpos >= m_indata.size())
         return '\0';
      return m_indata[m_inpos];
   }

   std::vector<char> m_indata;      // The JSON text currently being parsed.
   size_t            m_inpos = 0;   // The current position in the JSON text.
   std::wstring      m_token;       // The current token from the JSON text.
   char              m_quote = ' '; // If the token was quoted, this contains
                                    // the kind of quote it was quoted with
                                    // (single or double quote).
};

namespace {

//--------------------------------------------------------------------
// Parses the next JSON node from the given JSON tokenizer object.
// Returns empty/null object if there are no more nodes.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONNode(JsonScanner &lex, bool noNamePrefix)
{
   trace("ParseJSONNode\n");

   JsonNode node;

   // The object will either start with an object name or it will
   // go right into a group object or array object with no name.
   if (!noNamePrefix && !lex.TokenIs(L"{") && !lex.TokenIs(L"["))
   {
      // This token is assumed to be the name of the JSON object.
      node.m_name = lex.CurToken();
      trace(L"  node name = '%s'\n", node.m_name.c_str());

      if (!lex.ScanNextToken())   // Eat the name token.
         return nullptr;
      trace(L"  colon = '%s'\n", lex.CurToken().c_str());

      // There should be a colon between the name and the object's value.
      if (!lex.TokenIs(L":"))
         throw std::wstring(L"JSON malformed:  Colon missing between object name and value");
      if (!lex.ScanNextToken())
         throw std::wstring(L"JSON malformed:  Missing object value");
   }
   else
   {
      trace("  node has no name.\n");
   }

   // Now parse the object's value(s).
   double val = _wtof(lex.CurToken().c_str());
   if (lex.TokenIs(L"{"))
   {
      trace("  node type is group.\n");

      // The data for this JSON object is a group of other JSON objects.
      node.m_type = JsonType::Group;

      if (!lex.ScanNextToken()) // Eat the "{"
         return nullptr;

      // Parse the objects in the group.
      while (!lex.EndOfInput() && lex.CurToken() != L"}")
         node.m_children.push_back(ParseJSONNode(lex, false));

      if (lex.TokenIs(L"}"))
         lex.ScanNextToken();  // Eat the "}"

      trace("  Done with group.\n");
   }
   else if (lex.TokenIs(L"["))
   {
      trace("  node type is array.\n");

      // The data for this JSON object is an array of JSON objects.
      node.m_type = JsonType::Array;

      if (!lex.ScanNextToken()) // Eat the "["
         return nullptr;

      // Parse the objects in the array.
      while (!lex.EndOfInput() && lex.CurToken() != L"]")
         node.m_children.push_back(ParseJSONNode(lex, true));

      if (lex.TokenIs(L"]"))
         lex.ScanNextToken();  // Eat the "]"

      trace("  Done with array.\n");
   }
   else if (val != 0. || lex.CurToken()[0] == '0')
   {
      trace("  node type is number, value is %G.\n", val);

      // The data for this JSON object is a number.
      node.m_type = JsonType::Number;
      node.m_number = val;

      lex.ScanNextToken();  // Eat the number.
   }
   else if (lex.CurQuoted() == ' ' && (lex.TokenIs(L"null") || lex.TokenIs(L",")))
   {
      trace("  node type is null.\n");

      // This object's value is "null" or absent, so this is a null JSON object.
      node.m_type = JsonType::Null;

      if (lex.TokenIs(L"null"))
         lex.ScanNextToken();  // Eat the "null".
   }
   else if (lex.TokenIs(L"true") || lex.TokenIs(L"false"))
   {
      trace("  node type is bool, value is %c\n", lex.TokenIs(L"true") ? 'Y' : 'N');

      // The data for this JSON object is boolean.
      node.m_type = JsonType::Bool;
      node.m_bool = lex.TokenIs(L"true");

      lex.ScanNextToken();  // Eat the bool value.
   }
   else
   {
      trace(L"  node type is string, value is '%s'\n", lex.CurToken().c_str());

      // The data for this JSON object is a string.
      node.m_type = JsonType::String;
      node.m_string = lex.CurToken();

      lex.ScanNextToken();  // Eat the string.
   }

   // Skip the comma that is expected between this object and the
   // next one.  Some badly formed JSON files omit commas, so we
   // won't consider it an error if there's no comma here.  Also,
   // there shouldn't be a comma after the last object in the JSON
   // file, but in some badly formed JSON files there is a comma
   // after the last object.  We won't consider that an error
   // here either.
   if (lex.TokenIs(L","))
      lex.ScanNextToken();

   return std::make_shared<JsonNode>(node);
}

} // End anon namespace

//--------------------------------------------------------------------
// Parses JSON text from the given memory buffer.
// If successful, the root node of the node tree is returned.
// Errors throw.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONFromMemory(const char *data, size_t size)
{
   trace("ParseJSONFromMemory data=%p size=%zu\n", data, size);

   JsonScanner lex;

   // Determine if the input data is in JSON or JSONP format.
   // If the second token is a left parenthesis, assume it's JSONP.
   lex.Start(data, size);
   lex.ScanNextToken();       // Eat the jsonp function name.
   bool formatJSONP = lex.TokenIs(L"(");
   if (formatJSONP)
      lex.ScanNextToken();    // Eat the "("
   else
      lex.Start(data, size);  // Restart for regular JSON, not JSONP.

   trace("formatJSONP=%c\n", formatJSONP ? 'Y' : 'N');

   // This does most of the work.      
   auto rootNode = ParseJSONNode(lex, false);

   // Handle the trailing parenthesis if we're reading JSONP format.
   if (formatJSONP && !lex.TokenIs(L")"))
      throw std::wstring(L"Missing right parenthesis at end of JSONP function");

   return rootNode;
}

//--------------------------------------------------------------------
// Overload of above, takes vector of chars.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONFromMemory(const std::vector<char> &data)
{
   if (data.empty())
      return nullptr;

   return ParseJSONFromMemory(data.data(), data.size());
}

//--------------------------------------------------------------------
// Parses JSON text from the specified file.
// If successful, the root node of the node tree is returned.
// Errors throw.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONFromFile(const std::wstring &filename)
{
   trace(L"ParseJSONFromFile filename='%s'\n", filename.c_str());

   std::vector<char> data;

   FILE *file = nullptr;
   if (_wfopen_s(&file, filename.c_str(), L"rb") || file == nullptr)
      throw L"File could not be opened for reading";

   try
   {
      fseek(file, 0, SEEK_END);
      long fileSize = ftell(file);
      fseek(file, 0, SEEK_SET);
      if (fileSize < 1)
         throw L"File is empty";

      trace("File size is %ld\n", fileSize);

      data.resize(fileSize);
      if (fread(&data[0], fileSize, 1, file) != 1)
         throw L"Failed reading data from file";

      fclose(file);
   }
   catch(...)
   {
      fclose(file);
      throw;
   }

   // This does most of the work.
   return ParseJSONFromMemory(data);
}

} // End namespace njson
