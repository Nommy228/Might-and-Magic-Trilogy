#pragma once
#include <list>
#include <algorithm>

class UIControl
{
  public:
    virtual void Show() = 0;

    virtual bool Focused() = 0;

    // Events
    virtual bool OnKey(int key)
    {
      for (auto i = children.begin(); i != children.end(); ++i)
        if ((*i)->OnKey(key))
          return true;
      return false;
    }

    virtual bool OnMouseLeftClick(int x, int y)
    {
      for (auto i = children.begin(); i != children.end(); ++i)
        if ((*i)->OnMouseLeftClick(x, y))
          return true;
      return false;
    }

    virtual bool OnMouseRightClick(int x, int y)
    {
      for (auto i = children.begin(); i != children.end(); ++i)
        if ((*i)->OnMouseRightClick(x, y))
          return true;
      return false;
    }

    virtual bool OnMouseEnter()
    {
      for (auto i = children.begin(); i != children.end(); ++i)
        if ((*i)->OnMouseEnter())
          return true;
      return false;
    }

    virtual bool OnMouseLeave()
    {
      for (auto i = children.begin(); i != children.end(); ++i)
        if ((*i)->OnMouseLeave())
          return true;
      return false;
    }

    // Container
    virtual bool AddControl(UIControl *ctrl)
    {
      if (std::find(children.begin(), children.end(), ctrl) == children.end())
      {
        children.push_back(ctrl);
        return true;
      }
      return false;
    }

    virtual bool RemoveControl(UIControl *ctrl)
    {
      auto i = std::find(children.begin(), children.end(), ctrl);

      children.remove(ctrl);
      if (i != children.end())
        return true;
      return false;
    }

  protected:
    std::list<UIControl *> children;
};