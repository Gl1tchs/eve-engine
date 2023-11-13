// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include <vector>

#include "core/utils/functional.h"

struct MenuItem {
  std::string name;
  std::string shortcut;
  std::function<void(void)> callback;

  MenuItem(std::string name, std::function<void(void)> callback,
           std::string shortcut = "");
};

struct MenuItemGroup {
  using MenuItemVector = std::vector<MenuItem>;

  MenuItemVector items;
  std::function<bool()> show_condition;

  MenuItemGroup(std::function<bool()> show = []() -> bool { return true; });

  void PushMenuItem(const MenuItem& item);

  MenuItemVector::iterator begin() { return items.begin(); }
  MenuItemVector::iterator end() { return items.end(); }
  MenuItemVector::reverse_iterator rbegin() { return items.rbegin(); }
  MenuItemVector::reverse_iterator rend() { return items.rend(); }

  MenuItemVector::const_iterator begin() const { return items.begin(); }
  MenuItemVector::const_iterator end() const { return items.end(); }
  MenuItemVector::const_reverse_iterator rbegin() const {
    return items.rbegin();
  }
  MenuItemVector::const_reverse_iterator rend() const { return items.rend(); }
};

struct Menu {
  using ItemGroupVector = std::vector<MenuItemGroup>;

  std::string name;
  ItemGroupVector item_groups;

  Menu(std::string name);

  [[nodiscard]] size_t GetSize() const;

  void PushItemGroup(const MenuItemGroup& item_group);

  ItemGroupVector::iterator begin() { return item_groups.begin(); }
  ItemGroupVector::iterator end() { return item_groups.end(); }
  ItemGroupVector::reverse_iterator rbegin() { return item_groups.rbegin(); }
  ItemGroupVector::reverse_iterator rend() { return item_groups.rend(); }

  ItemGroupVector::const_iterator begin() const { return item_groups.begin(); }
  ItemGroupVector::const_iterator end() const { return item_groups.end(); }
  ItemGroupVector::const_reverse_iterator rbegin() const {
    return item_groups.rbegin();
  }
  ItemGroupVector::const_reverse_iterator rend() const {
    return item_groups.rend();
  }
};

class MenuBar final {
 public:
  MenuBar() = default;

  void PushMenu(const Menu& menu);

  void Draw();

 private:
  std::vector<Menu> menus_;
};
