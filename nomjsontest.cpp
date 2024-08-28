//--------------------------------------------------------------------
// nomjsontest.cpp
// Command-line tool to excersize the NomJSON module.  This program
// reads a JSON file, parses it into JSON objects, and dumps the JSON
// objects to the console in a human-readable form.
//
// (C) Copyright 2016-2017 Ammon R. Campbell.
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>

namespace {

//--------------------------------------------------------------------
// Displays the contents of the given JSON node (and its children)
// to the console.
//--------------------------------------------------------------------
void DumpJsonNode(const std::shared_ptr<njson::JsonNode> &node, size_t indent)
{
   if (!node)
      return;

   // Start by indenting the indicated amount.
   for (size_t ndx = 0; ndx < indent; ++ndx)
      wprintf(L"  ");

   if (node->m_name.empty())
      wprintf(L"(unnamed):  ");
   else
      wprintf(L"%s:  ", node->m_name.c_str());

   switch(node->m_type)
   {
      case njson::JsonType::Number:
         wprintf(L"number:  %G", node->m_number);
         break;
      case njson::JsonType::String:
         wprintf(L"string:  \"%s\"", node->m_string.c_str());
         break;
      case njson::JsonType::Bool:
         wprintf(L"bool:  %s", node->m_bool ? L"true" : L"false");
         break;
      case njson::JsonType::Array:
         wprintf(L"array:  [%zu elements]", node->m_children.size());
         break;
      case njson::JsonType::Group:
         wprintf(L"group  {%zu elements}:", node->m_children.size());
         break;
      case njson::JsonType::Null:
         wprintf(L"null");
         break;
      default:
         wprintf(L"OTHER/UNKNOWN!");
         break;
   }
   wprintf(L"\n");

   // Recursively handle any children of this node.
   if (!node->m_children.empty())
   {
      // Display opening bracket.
      for (size_t ndx = 0; ndx < indent; ++ndx)
         wprintf(L"  ");
      wprintf(L"%c\n", node->m_type == njson::JsonType::Group ? '{' : '[');

      // Display the children between the brackets.
      for (const auto &child : node->m_children)
         DumpJsonNode(child, indent + 1);

      // Display closing bracket.
      for (size_t ndx = 0; ndx < indent; ++ndx)
         wprintf(L"  ");
      wprintf(L"%c\n", node->m_type == njson::JsonType::Group ? '}' : ']');
   }
}

} // End anon namespace

//--------------------------------------------------------------------
// Application entry point.  Takes standard arguments.
// Returns EXIT_SUCESS if no errors occur.
//--------------------------------------------------------------------
int
wmain(int argc, wchar_t **argv)
{
   if (argc != 2)
   {
      wprintf(L"Usage:  nomjsontest filename.json\n");
      return EXIT_FAILURE;
   }

   try
   {
      auto rootNode = njson::ParseJSONFromFile(argv[1]);
      DumpJsonNode(rootNode, 0);
   }
   catch(const std::wstring &exc)
   {
      wprintf(L"Aborted due to error:\n%s\n", exc.c_str());
      return EXIT_FAILURE;
   }
   catch(...)
   {
      wprintf(L"Aborted due to unhandled exception.\n");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

