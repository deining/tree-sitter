#ifndef COMPILER_BUILD_TABLES_PARSE_ITEM_H_
#define COMPILER_BUILD_TABLES_PARSE_ITEM_H_

#include <set>
#include <string>
#include <unordered_map>
#include "compiler/rules/symbol.h"
#include "compiler/build_tables/item.h"

namespace tree_sitter {
    namespace build_tables {
        class ParseItem : public Item {
        public:
            ParseItem(const rules::Symbol &lhs, rules::rule_ptr rule, const size_t consumed_symbol_count);
            bool operator==(const ParseItem &other) const;
            size_t consumed_symbol_count;
        };

        std::ostream& operator<<(std::ostream &stream, const ParseItem &item);

        typedef std::unordered_map<ParseItem, std::set<rules::Symbol>> ParseItemSet;
    }
}

namespace std {
    template<>
    struct hash<tree_sitter::build_tables::ParseItem> {
        size_t operator()(const tree_sitter::build_tables::ParseItem &item) const {
            return
                hash<tree_sitter::rules::Symbol>()(item.lhs) ^
                hash<tree_sitter::rules::rule_ptr>()(item.rule) ^
                hash<size_t>()(item.consumed_symbol_count);
        }
    };

    template<>
    struct hash<const tree_sitter::build_tables::ParseItemSet> {
        size_t operator()(const tree_sitter::build_tables::ParseItemSet &set) const {
            size_t result = hash<size_t>()(set.size());
            for (auto &pair : set) {
                result ^= hash<tree_sitter::build_tables::ParseItem>()(pair.first);
                result ^= hash<size_t>()(pair.second.size());
                for (auto &symbol : pair.second)
                    result ^= hash<tree_sitter::rules::Symbol>()(symbol);
            }
            return result;
        }
    };
}

#endif  // COMPILER_BUILD_TABLES_PARSE_ITEM_H_
