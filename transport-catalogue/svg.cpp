#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();
    
    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    bool is_first = true;
    out << "<polyline points=\""sv;
    for (const auto& point : points_) {
        if (is_first) {
            is_first = false;
            out << point.x << ',' << point.y;
            continue;
        }
        out << ' ' << point.x << ',' << point.y;
    }
    out << "\" />"sv;
}

Text& Text::SetPosition(Point pos) {
    position_ = pos;
    return *this;
}

Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t size) {
    font_size_ = size;
    return *this;
}

Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

Text& Text::SetData(std::string data) {
    data_ = data;
    return *this;
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text x=\""sv << position_.x << "\" y=\""sv << position_.y << "\""sv;
    out << " dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\""sv;
    out << " font-size=\""sv << font_size_ << "\""sv;
    if (font_family_.size() || font_family_.find_first_not_of(' ') != font_family_.npos) out << " font-family=\""sv << font_family_ << "\""sv;
    if (font_weight_.size() || font_weight_.find_first_not_of(' ') != font_weight_.npos) out << " font-weight=\""sv << font_weight_ << "\""sv; 
    out << ">"sv << data_ << "</text>"sv;
}

void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    elements_.emplace_back(std::move(obj));
}

void Document::Render(std::ostream& out) const {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
    for (const auto& elem : elements_) {
        elem->Render({out, 2, 2});
    }
    out << "</svg>\n"sv;
}

Polyline& Polyline::AddPoint(Point point) {
    points_.emplace_back(point);
    return *this;
}

}  // namespace svg