#pragma once
#include <gimgui/data/types.hpp>

namespace gim
{
    class Element
    {
        public:
            Element(const TagSet& tags);
            Element* append(Element&& child);
            ElementPtrList find(const TagSet& tags);
        private:
            TagSet mTags;
            ElementList mChildren;
    };
}