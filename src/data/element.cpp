#include <gimgui/data/element.hpp>
#include <gimgui/assert.hpp>
#include <algorithm>

namespace gim
{
    Element::Element():
        mParent(nullptr)
    {
    }

    Element::Element(const TagSet& tags) :
        mTags(tags),
        mParent(nullptr)
    {
    }

    Element& Element::append(Element&& child)
    {
        child.mParent = this;
        mChildren.push_back(std::unique_ptr<Element>(new Element(std::move(child))));
        return *mChildren.back();
    }

    Element& Element::prepend(Element&& child)
    {
        child.mParent = this;
        mChildren.push_front(std::unique_ptr<Element>(new Element(std::move(child))));
        return *mChildren.front();
    }

    Element& Element::insert(size_t index, Element&& child)
    {
        GIM_ASSERT(index <= mChildren.size(), "Index out of bounds. Trying to insert element at " + std::to_string(index) + " but element only has " + std::to_string(mChildren.size()) + " children");
        child.mParent = this;
        auto iterator = mChildren.insert(mChildren.begin() + index, std::unique_ptr<Element>(new Element(std::move(child))));
        return **iterator;
    }

    ElementList& Element::children()
    {
        return mChildren;
    }

    const ElementList& Element::children() const
    {
        return mChildren;
    }
            
    const TagSet& Element::tags() const
    {
        return mTags;
    }

    Element* Element::parent()
    {
        return mParent;
    }

    const Element* Element::parent() const
    {
        return mParent;
    }

    ElementPtrList Element::find(const TagSet& tags)
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        return findHelper(tags);
    }

    ElementConstPtrList Element::find(const TagSet& tags) const
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        ElementPtrList elements = findHelper(tags);

        return ElementConstPtrList(elements.begin(), elements.end());
    }

    ElementPtrList Element::recursiveFind(const TagSet& tags)
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        return recursiveFindHelper(tags);
    }

    ElementConstPtrList Element::recursiveFind(const TagSet& tags) const
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        ElementPtrList elements = recursiveFindHelper(tags);

        return ElementConstPtrList(elements.begin(), elements.end());
    }

    ElementPtrList Element::findHelper(const TagSet& tags) const
    {
        ElementPtrList result;

        for(auto& child : mChildren)
        {
            if(std::includes(child->mTags.begin(), child->mTags.end(), tags.begin(), tags.end()))
            {
                result.push_back(&*child);
            }
        }

        return result;
    }
    
    ElementPtrList Element::recursiveFindHelper(const TagSet& tags) const
    {
        ElementPtrList result;

        if(std::includes(mTags.begin(), mTags.end(), tags.begin(), tags.end()))
        {
            result.push_back(&*const_cast<Element*>(this));
        }

        for(auto& child : mChildren)
        {
            ElementPtrList childResults = child->recursiveFindHelper(tags);
            result.insert(result.end(), childResults.begin(), childResults.end());
        }

        return result;
    }

    bool Element::hasAttribute(const std::string& name)
    {
        return mAttributes.count(name) != 0;
    }
}
