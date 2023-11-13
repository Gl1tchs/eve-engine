// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "widgets/menu_bar.h"

#include <cstdint>

#include <imgui.h>

MenuItem::MenuItem(std::string p_name, std::function<void(void)> p_callback,
                   std::string p_shortcut)
    : name(p_name), shortcut(p_shortcut), callback(p_callback) {}

MenuItemGroup::MenuItemGroup(std::function<bool()> show_func)
    : show_condition(show_func) {}

void MenuItemGroup::PushMenuItem(const MenuItem& item) {
  items.push_back(item);
}

Menu::Menu(std::string p_name) : name(p_name) {}

size_t Menu::GetSize() const {
  return item_groups.size();
}

void Menu::PushItemGroup(const MenuItemGroup& item_group) {
  item_groups.push_back(item_group);
}

void MenuBar::PushMenu(const Menu& menu) {
  menus_.push_back(menu);
}

void MenuBar::Draw() {
  if (ImGui::BeginMenuBar()) {

    for (auto& menu : menus_) {
      if (ImGui::BeginMenu(menu.name.c_str())) {

        uint32_t index = 0;
        // initialized to 1 to not substract one again
        uint32_t skipped_group_count = 1;
        for (auto& item_group : menu) {
          if (!item_group.show_condition()) {
            skipped_group_count++;
            continue;
          }

          for (auto& menu_item : item_group) {
            if (ImGui::MenuItem(menu_item.name.c_str(),
                                menu_item.shortcut.c_str())) {
              if (menu_item.callback) {
                menu_item.callback();
              }
            }
          }

          if (index < menu.GetSize() - skipped_group_count) {
            ImGui::Separator();
          }

          index++;
        }

        ImGui::EndMenu();
      }
    }

    ImGui::EndMenuBar();
  }
}
