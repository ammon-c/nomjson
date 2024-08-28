//--------------------------------------------------------------------
// nomjson.h
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

#pragma once
#include <string>
#include <vector>
#include <memory>

namespace njson
{

//--------------------------------------------------------------------
// IDs for the six possible types of JSON nodes.
//--------------------------------------------------------------------
enum class JsonType { Number, String, Bool, Array, Group, Null };

//--------------------------------------------------------------------
// Container for one node from a tree of JSON nodes.
//--------------------------------------------------------------------
class JsonNode
{
public:
   std::wstring m_name;                   // The name of this node.
   JsonType     m_type = JsonType::Null;  // The data type of this node.
   std::wstring m_string;                 // Node's string value when m_type==JsonType::String.
   double       m_number = 0.;            // Node's numeric value when m_type==JsonType::Number.
   bool         m_bool = false;           // Node's boolean value when m_type==JsonType::Bool.

   // If m_type is Array or Group, this is the list of child
   // elements in the array or group.
   std::vector<std::shared_ptr<JsonNode>> m_children;

   JsonNode() = default;
   ~JsonNode() = default;
   JsonNode(const JsonNode &j) = default;

   // If this node has a child node with the specified name, returns
   // the child.  Otherwise returns null pointer.
   // Only search immediate children, not recursively.
   std::shared_ptr<JsonNode> FindChildByName(const std::wstring &name)
   {
      for (const auto &child : m_children)
         if (child->m_name == name)
            return child;
      return nullptr;
   }
};

//--------------------------------------------------------------------
// Parses JSON text from the given memory buffer.
// If successful, the root node of the node tree is returned.
// Errors throw.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONFromMemory(const char *data, size_t size);
std::shared_ptr<JsonNode> ParseJSONFromMemory(const std::vector<char> &data);

//--------------------------------------------------------------------
// Parses JSON text from the specified file.
// If successful, the root node of the node tree is returned.
// Errors throw.
//--------------------------------------------------------------------
std::shared_ptr<JsonNode> ParseJSONFromFile(const std::wstring &filename);

} // End namespace njson
