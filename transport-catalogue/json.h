#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <initializer_list>
#include <variant>
#include <cassert>
#include <utility>

namespace json {

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class Node : std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string> {
 public:
    using variant::variant;

    const variant &GetBase() const { return *this; }

    bool IsNull() const {
        return std::holds_alternative<std::nullptr_t>(*this);
    }

    bool IsArray() const {
        return std::holds_alternative<Array>(*this);
    }

    const auto &AsArray() const {
        try {
            return std::get<Array>(*this);
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    }

    bool IsMap() const {
        return std::holds_alternative<Dict>(*this);
    }

    const auto &AsMap() const {
        try {
            return std::get<Dict>(*this);
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    } 

    bool IsBool() const {
        return std::holds_alternative<bool>(*this);
    }

    bool AsBool() const {
        try {
            return std::get<bool>(*this);
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    }

    bool IsInt() const {
        return std::holds_alternative<int>(*this);
    }

    int AsInt() const {
        try {
            return std::get<int>(*this);
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    }

    bool IsPureDouble() const {
        return std::holds_alternative<double>(*this);
    }

    bool IsDouble() const {
        return IsPureDouble() || IsInt();
    }

    double AsDouble() const {
        try {
            return IsPureDouble() ? std::get<double>(*this) : AsInt();
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    }

    bool IsString() const {
        return std::holds_alternative<std::string>(*this);
    }

    const auto &AsString() const {
        try {
            return std::get<std::string>(*this);
        } catch (...) {
            throw std::logic_error("Logical Error");
        }
    }
};

bool operator==(const Node& left, const Node& right);
bool operator!=(const Node& left, const Node& right);

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

bool operator==(const Document& left, const Document& right);
bool operator!=(const Document& left, const Document& right);

Document Load(std::istream& input);

struct PrintContext {
    std::ostream& out;
    int indent_step = 4;
    int indent = 0;

    void PrintIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    // Возвращает новый контекст вывода с увеличенным смещением
    PrintContext Indented() const {
        return {out, indent_step, indent_step + indent};
    }
};

// Шаблон, подходящий для вывода double и in
template <typename Value>
void PrintValue(const Value& value, const PrintContext& ctx) {
    ctx.out << value;
}

template<>
void PrintValue<std::nullptr_t>(const std::nullptr_t&, const PrintContext& ctx);

template<>
void PrintValue<bool>(const bool& b, const PrintContext& ctx);

template<>
void PrintValue<std::string>(const std::string& s, const PrintContext& ctx);

template<>
void PrintValue<Dict>(const Dict& d, const PrintContext& ctx);

template<>
void PrintValue<Array>(const Array& a, const PrintContext& ctx);

template<>
void PrintValue<Node>(const Node& n, const PrintContext& ctx);

void PrintNode(const Node& node, const PrintContext& ctx);

void Print(const Document& doc, std::ostream& out);

}  // namespace json