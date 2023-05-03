#include "json.h"

using namespace std;
namespace json {

namespace {

Node LoadNode(std::istream& input);
Node LoadString(std::istream& input);

std::string LoadLiteral(std::istream& input) {
    std::string s;
    while (std::isalpha(input.peek())) {
        s.push_back(static_cast<char>(input.get()));
    }
    return s;
}

Node LoadArray(std::istream& input) {
    std::vector<Node> result;
    
    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }
    if (!input) {
        throw ParsingError("Array parsing error"s);
    }
    return Node(std::move(result));
}

Node LoadDict(std::istream& input) {
    Dict dict;
    
    for (char c; input >> c && c != '}';) {
        if (c == '"') {
            std::string key = LoadString(input).AsString();
            if (input >> c && c == ':') {
                if (dict.find(key) != dict.end()) {
                    throw ParsingError("Duplicate key '"s + key + "' have been found");
                }
                dict.emplace(std::move(key), LoadNode(input));
            } else {
                throw ParsingError(": is expected but '"s + c + "' has been found"s);
            }
        } else if (c != ',') {
            throw ParsingError(R"(',' is expected but ')"s + c + "' has been found"s);
        }
    }
    if (!input) {
        throw ParsingError("Dictionary parsing error"s);
    }
    return Node(std::move(dict));
}

Node LoadString(std::istream& input) {
    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            ++it;
            break;
        } else if (ch == '\\') {
            ++it;
            if (it == end) {
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            throw ParsingError("Unexpected end of line"s);
        } else {
            s.push_back(ch);
        }
        ++it;
    }
    
    return Node(std::move(s));
}

Node LoadBool(std::istream& input) {
    const auto s = LoadLiteral(input);
    if (s == "true"sv) {
        return Node{true};
    } else if (s == "false"sv) {
        return Node{false};
    } else {
        throw ParsingError("Failed to parse '"s + s + "' as bool"s);
    }
}

Node LoadNull(std::istream& input) {
    if (auto literal = LoadLiteral(input); literal == "null"sv) {
        return Node{nullptr};
    } else {
        throw ParsingError("Failed to parse '"s + literal + "' as null"s);
    }
}

Node LoadNumber(std::istream& input) {
    std::string parsed_num;
    
    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };
    
    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };
    
    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }
    
    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }
    
    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }
    
    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return std::stoi(parsed_num);
            } catch (...) {
                // В случае неудачи, например, при переполнении
                // код ниже попробует преобразовать строку в double
            }
        }
        return std::stod(parsed_num);
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

Node LoadNode(std::istream& input) {
    char c;
    if (!(input >> c)) {
        throw ParsingError("Unexpected EOF"s);
    }
    switch (c) {
        case '[':
            return LoadArray(input);
        case '{':
            return LoadDict(input);
        case '"':
            return LoadString(input);
        case 't':
            [[fallthrough]];
        case 'f':
            input.putback(c);
            return LoadBool(input);
        case 'n':
            input.putback(c);
            return LoadNull(input);
        default:
            input.putback(c);
            return LoadNumber(input);
    }
}

}  // namespace

bool operator==(const Node& left, const Node& right) {
    return left.GetBase() == right.GetBase();
}

bool operator!=(const Node& left, const Node& right) {
    return !(left == right);
}

bool operator==(const Document& left, const Document& right) {
    return left.GetRoot() == right.GetRoot();
}

bool operator!=(const Document& left, const Document& right) {
    return !(left == right);
}

Document::Document(Node root)
    : root_(std::move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

template<>
void PrintValue<std::nullptr_t>(const std::nullptr_t&, std::ostream& out) {
    out << "null"sv;
}

template<>
void PrintValue<std::string>(const std::string& s, std::ostream& out) {
    out << '"';
    for (const char c : s) {
        switch (c) {
            case '\r':
                out << "\\r"sv;
                break;
            case '\n':
                out << "\\n"sv;
                break;
            case '"':
                [[fallthrough]];
            case '\\':
                out.put('\\');
                [[fallthrough]];
            default:
                out.put(c);
                break;
        }
    }
    out << '"';
}

template<>
void PrintValue<Array>(const Array& a, std::ostream& out) {
    out << "[\n"sv;
    bool first = true;
    for (const Node& node : a) {
        if (first) {
            first = false;
        } else {
            out << ",\n"sv;
        }
        PrintNode(node, out);
    }
    out << '\n';
    out << ']';
}

template<>
void PrintValue<Dict>(const Dict& d, std::ostream& out) {
    out << "{\n"sv;
    bool first = true;
    for (const auto& [key, node] : d) {
        if (first) {
            first = false;
        } else {
            out << ",\n"sv;
        }
        PrintValue(key, out);
        out << ": "sv;
        PrintNode(node, out);
    }
    out << '\n';
    out << '}';
}

template<>
void PrintValue<bool>(const bool& b, std::ostream& out) {
    out << std::boolalpha << b;
}

template<>
void PrintValue<Node>(const Node& n, std::ostream& out) {
    PrintNode(n, out);
}

void PrintNode(const Node &node, ostream &out) {
    visit([&out](const auto &value) { PrintValue(value, out); },
          node.GetBase());
}

void Print(const json::Document &document, ostream &out) {
    PrintNode(document.GetRoot(), out);
}

}  // namespace json